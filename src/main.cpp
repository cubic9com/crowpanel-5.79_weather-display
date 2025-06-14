/**
 * A weather forecast display system
 * using the Elecrow CrowPanel ESP32 E-Paper HMI 5.79-inch Display,
 * showing every 3 hours forecast for the next 12 hours.
 * 
 * The weather forecast data is obtained using the OpenWeatherMap API.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "EPD.h"
#include "icons.h"
#include "config.h"
#include "../test/testdata.h" // data for offline test

//=============================================================================
// Configuration Constants
//=============================================================================

// Test Settings
#define TEST_MODE            false // Test mode (uses test data instead of the API when set to true)

// Display Settings
#define FORECAST_COUNT       5     // Number of forecast periods to display
#define COLUMN_WIDTH         158   // Width of each forecast column in pixels
#define TEXT_BUFFER_SIZE     40    // Size of text buffer for formatting

// E-Paper Settings
#define EPD_POWER_PIN        7     // GPIO pin for E-Paper power control
#define EPD_BUFFER_SIZE      27200 // Size of E-Paper display buffer

// Network Settings
#define HTTP_TIMEOUT_MS      10000 // HTTP request timeout in milliseconds
#define MAX_RETRY_COUNT      3     // Maximum number of retry attempts

// Power Management Settings
#define DEEP_SLEEP_PIN       GPIO_NUM_33 // GPIO pin for deep sleep wakeup

//=============================================================================
// Type Definitions
//=============================================================================

/**
 * Weather icon enumeration
 * Maps to indices in the Weather_Num array in icons.h
 */
enum WeatherIconNumber {
  ICON_CLEAR_DAY = 0,
  ICON_CLEAR_NIGHT = 1,
  ICON_CLOUDS = 2,
  ICON_RAIN = 3,
  ICON_THUNDERSTORM = 4,
  ICON_SNOW = 5,
  ICON_MIST = 6
};

/**
 * Mapping between OpenWeatherMap icon codes and our icon enumeration
 */
struct WeatherMapping {
  String iconCode;   // icon code in OpenWeatherMap
  int iconNumber;    // Corresponding icon number in our system
};

/**
 * Forecast information structure
 * Stores weather forecast data for a specific time period
 */
struct ForecastInfo {
  String time;       // Time (HH:MM)
  int iconNumber;    // Weather icon number
  float temperature; // Temperature (C or F depending on TEMPERATURE_UNIT)
  float pop;         // Probability of precipitation (%)
};

//=============================================================================
// Global Constants
//=============================================================================

/**
 * Weather mapping table
 * Maps OpenWeatherMap icon codes to our icon enumeration
 */
const WeatherMapping WEATHER_MAPPINGS[] = {
  {"01d", ICON_CLEAR_DAY},
  {"01n", ICON_CLEAR_NIGHT},
  {"02d", ICON_CLOUDS},
  {"02n", ICON_CLOUDS},
  {"03d", ICON_CLOUDS},
  {"03n", ICON_CLOUDS},
  {"04d", ICON_CLOUDS},
  {"04n", ICON_CLOUDS},
  {"09d", ICON_RAIN},
  {"09n", ICON_RAIN},
  {"10d", ICON_RAIN},
  {"10n", ICON_RAIN},
  {"11d", ICON_THUNDERSTORM},
  {"11n", ICON_THUNDERSTORM},
  {"13d", ICON_SNOW},
  {"13n", ICON_SNOW},
  {"50d", ICON_MIST},
  {"50n", ICON_MIST}
};

#define WEATHER_MAPPINGS_COUNT (sizeof(WEATHER_MAPPINGS) / sizeof(WeatherMapping))

//=============================================================================
// Global Variables
//=============================================================================

// E-Paper Display Buffer
uint8_t ImageBW[EPD_BUFFER_SIZE];

// API Related Variables
String jsonBuffer;
int httpResponseCode = 0;

// Array to Store Forecast Data
ForecastInfo hourlyForecasts[FORECAST_COUNT];


//=============================================================================
// Deep-sleep Functions
//=============================================================================

/**
 * Function to Enter Deep-Sleep Mode
 */
void enterDeepSleep() {
  Serial.println("Entering Deep-sleep mode. Will wake up later.");
  Serial.flush();
  
  // Put EPD in Sleep Mode Before Entering Deep-Sleep Mode
  EPD_DeepSleep();

  delay(4000);
  
  // Enter Deep-Sleep Mode (Wake Up After n minutes)
  esp_sleep_enable_timer_wakeup(INTERVAL_IN_MINUTES * 60UL * 1000UL * 1000); // microseconds
  esp_deep_sleep_start();
}

//=============================================================================
// Display Functions
//=============================================================================

/**
 * Displays weather forecast on the E-Paper display
 * 
 * Renders time, weather icon, temperature and probability of precipitation
 * for each forecast period in a column layout
 */
void displayWeatherForecast()
{
  char buffer[TEXT_BUFFER_SIZE];

  // Initialize Display
  Paint_NewImage(ImageBW, EPD_W, EPD_H, Rotation, WHITE);
  Paint_Clear(WHITE);
  EPD_FastMode1Init();
  EPD_Display_Clear();
  EPD_Update();
  EPD_Clear_R26A6H();

  // Display Each Forecast Data
  for (int i = 0; i < FORECAST_COUNT; i++) {
    if (hourlyForecasts[i].time.length() > 0) {
      // Calculate x position for this column
      int baseX = COLUMN_WIDTH * i;
      
      // Display Time
      memset(buffer, 0, sizeof(buffer));
      snprintf(buffer, sizeof(buffer), "%s ", hourlyForecasts[i].time);
      EPD_ShowString(26 + baseX, 18, buffer, 44, BLACK);

      // Display Weather Icon
      EPD_ShowPicture(16 + baseX, 60, 128, 128, Weather_Num[hourlyForecasts[i].iconNumber], WHITE);

      // Display Temperature with appropriate unit
      memset(buffer, 0, sizeof(buffer));
      if (TEMPERATURE_UNIT == 0) {
        snprintf(buffer, sizeof(buffer), "%3d C", (int)round(hourlyForecasts[i].temperature));
      } else {
        snprintf(buffer, sizeof(buffer), "%3d F", (int)round(hourlyForecasts[i].temperature));
      }
      EPD_ShowString(30 + baseX, 190, buffer, 36, BLACK);
      EPD_DrawCircle(100 + baseX, 201, 2, BLACK, false);
      EPD_DrawCircle(100 + baseX, 201, 3, BLACK, false);

      if (i != 0) {
        // Display Probability of precipitation
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%3d %%", (int)round(100 * hourlyForecasts[i].pop));
        EPD_ShowString(30 + baseX, 225, buffer, 36, BLACK);
      }
    }
  }

  // Draw Separator Lines
  for (int i = 1; i < FORECAST_COUNT; i++) {
    EPD_DrawLine(2 + COLUMN_WIDTH * i, 0, 2 + COLUMN_WIDTH * i, 271, BLACK);
  }

  // Update Display
  EPD_Display(ImageBW);
  EPD_PartUpdate();
  
  Serial.println("Weather forecast displayed successfully");
}

/**
 * Displays an error message on the E-Paper display
 * 
 * @param message Error message to display
 */
void displayErrorMessage(const char* message) {
  // Initialize Display
  Paint_NewImage(ImageBW, EPD_W, EPD_H, Rotation, WHITE);
  Paint_Clear(WHITE);
  EPD_FastMode1Init();
  EPD_Display_Clear();
  EPD_Update();
  EPD_Clear_R26A6H();

  // Display Error Message
  EPD_ShowString(30, 30, "Error:", 24, BLACK);
  EPD_ShowString(30, 70, (char*)message, 12, BLACK);

  // Update Display
  EPD_Display(ImageBW);
  EPD_PartUpdate();
  
  Serial.print("Error displayed: ");
  Serial.println(message);
}

//=============================================================================
// Network Functions
//=============================================================================

/**
 * Connects to WiFi network using credentials from config.h
 * 
 * @return true if connection successful, false if failed
 */
bool connectToWiFi() {
  Serial.print("Connecting to WiFi network: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  // Connection Timeout (10 seconds)
  const int WIFI_TIMEOUT_MS = 10000;
  const int DELAY_MS = 500;
  int attempts = WIFI_TIMEOUT_MS / DELAY_MS;
  
  // Wait until Connection is Complete
  while (WiFi.status() != WL_CONNECTED && attempts > 0) {
    delay(DELAY_MS);
    Serial.print(".");
    attempts--;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("Connected to WiFi with IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi");
    return false;
  }
}

/**
 * Disconnects from WiFi to save power
 */
void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected for power saving");
}

/**
 * Sends an HTTP GET request to the specified URL
 * 
 * @param url URL to send the request to
 * @return Response content as String
 */
String httpGETRequest(const char* url) {
  WiFiClient client;
  HTTPClient http;

  // Initialize HTTP Client and Specify Server URL
  http.begin(client, url);

  // Set HTTP Request Timeout
  http.setTimeout(HTTP_TIMEOUT_MS);

  Serial.print("Sending HTTP GET request to: ");
  Serial.println(url);

  // Send HTTP GET Request
  httpResponseCode = http.GET();

  // Initialize Response Content
  String payload = "{}";

  // Check Response Code and Process Response Content
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    if (httpResponseCode == 200) {
      // Copy Response to Buffer
      payload = http.getString();
    } else {
      Serial.println("Request failed with non-200 status code");
    }
  } else {
    Serial.print("HTTP Request failed, error code: ");
    Serial.println(httpResponseCode);
    enterDeepSleep();
  }
  
  // Release HTTP Client Resources
  http.end();
  
  return payload;
}

//=============================================================================
// Weather Forecast Data Processing Functions
//=============================================================================

/**
 * Maps OpenWeatherMap icon code to our internal icon number
 * 
 * @param OpenWeatherMapIcon OpenWeatherMap icon code (e.g., "01d")
 * @return Corresponding internal icon number
 */
int getWeatherIconNum(String OpenWeatherMapIcon) {
  // Search for a Match from the Mapping Table
  for (size_t i = 0; i < WEATHER_MAPPINGS_COUNT; i++) {
    if (OpenWeatherMapIcon.indexOf(WEATHER_MAPPINGS[i].iconCode) != -1) {
      return WEATHER_MAPPINGS[i].iconNumber;
    }
  }
  
  // Default is Cloudy if no match found
  Serial.println("Warning: No icon match found for " + OpenWeatherMapIcon + ", using default");
  return ICON_CLOUDS;
}

/**
 * Stores weather information in the forecast array
 * 
 * @param index Index in the forecast array
 * @param unixTime Unix timestamp (UTC)
 * @param iconCode OpenWeatherMap icon code
 * @param temperature Temperature
 * @param pop Probability of precipitation
 */
void storeWeatherInfo(int index, long unixTime, String iconCode, float temperature, float pop) {
  if (index < 0 || index >= FORECAST_COUNT) {
    Serial.println("Error: Invalid forecast index");
    displayErrorMessage("Error: Invalid forecast index");
    enterDeepSleep();
    return;
  }

  // Convert UTC Unix Timestamp to Local Time
  time_t localTime = unixTime + TIMEZONE_OFFSET * 3600;
  struct tm *timeinfo = localtime(&localTime);

  // Store Information in Structure
  char tempTimeStr[6];
  strftime(tempTimeStr, sizeof(tempTimeStr), "%k:%M", timeinfo);
  hourlyForecasts[index].time = String(tempTimeStr);
  hourlyForecasts[index].iconNumber = getWeatherIconNum(iconCode);
  hourlyForecasts[index].temperature = temperature;
  hourlyForecasts[index].pop = pop;
}

/**
 * Prints weather forecast data to Serial for debugging
 */
void printWeatherData() {
  Serial.println("\n--- Weather Forecast Data ---");
  for (int i = 0; i < FORECAST_COUNT; i++) {
    if (hourlyForecasts[i].time.length() > 0) {
      Serial.print("Time: ");
      Serial.print(hourlyForecasts[i].time);
      Serial.print(" | IconNumber: ");
      Serial.print(hourlyForecasts[i].iconNumber);
      Serial.print(" | Temperature: ");
      Serial.print(hourlyForecasts[i].temperature);
      Serial.print("C | POP: ");
      Serial.print(hourlyForecasts[i].pop);
      Serial.println("%");
    }
  }
}

/**
 * Fetches weather forecast data from OpenWeatherMap API or test data
 * 
 * @param useTestData If true, uses test data instead of API
 * @return JSON string containing weather data, or empty string on failure
 */
String fetchWeatherData(bool useTestData = TEST_MODE) {
  // Returns Test Data When in test mode
  if (useTestData) {
    Serial.println("Using test data instead of API");
    return String(TEST_WEATHER_DATA);
  }
  
  // Check WiFi Connection Status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected. Attempting to reconnect...");
    if (!connectToWiFi()) {
      Serial.println("WiFi reconnection failed");
      return "";
    }
  }

  // Build OpenWeatherMap API Request URL
  String url = "http://api.openweathermap.org";
  url += "/data/3.0/onecall";
  url += "?lat=" + String((float) LATITUDE, 5);
  url += "&lon=" + String((float) LONGITUDE, 5);
  url += "&units=" + String(TEMPERATURE_UNIT == 0 ? "metric" : "imperial") + "&lang=en&exclude=minutely,daily,alerts";
  url += "&appid=" + (String) OPENWEATHERMAP_API_KEY;

  Serial.println("Fetching weather forecast data from OpenWeatherMap...");
  
  // Send HTTP Request and Get Response
  int retryCount = 0;
  bool requestSuccess = false;
  String responseData = "";
  
  while (retryCount < MAX_RETRY_COUNT && !requestSuccess) {
    responseData = httpGETRequest(url.c_str());
    
    if (httpResponseCode == 200) {
      requestSuccess = true;
    } else {
      retryCount++;
      Serial.print("Retry attempt ");
      Serial.print(retryCount);
      Serial.print(" of ");
      Serial.println(MAX_RETRY_COUNT);
      delay(1000);
    }
  }
  
  if (!requestSuccess) {
    Serial.println("Failed to fetch weather forecast data after multiple attempts");
    displayErrorMessage("Failed to fetch weather forecast data after multiple attempts");
    enterDeepSleep();
    return "";
  }
  
  return responseData;
}

/**
 * Analyzes weather data JSON and stores it in the forecast array
 * 
 * @param jsonData JSON string containing weather data
 * @return true if analysis was successful, false otherwise
 */
bool analyzeWeatherData(const String& jsonData) {
  // Parse JSON Data
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonData);

  // Check if JSON Parsing was Successful
  if (error != DeserializationError::Ok) {
    Serial.print("JSON parsing failed! Error: ");
    Serial.println(error.c_str());
    String errorMsg = "JSON parsing failed! Error: " + String(error.c_str());
    displayErrorMessage(errorMsg.c_str());
    return false;
  }

  // Store Current Weather Information
  storeWeatherInfo(0, doc["current"]["dt"], 
                  doc["current"]["weather"][0]["icon"],
                  doc["current"]["temp"].as<float>(),
                  doc["current"]["pop"].as<float>());

  // Store Future Weather Information (3, 6, 9, 12 hours later)
  const int hourlyIndices[] = {3, 6, 9, 12};
  
  for (int i = 0; i < 4; i++) {
    int hourlyIndex = hourlyIndices[i];
    
    if (hourlyIndex < doc["hourly"].size()) {
      storeWeatherInfo(i + 1, doc["hourly"][hourlyIndex]["dt"], 
                      doc["hourly"][hourlyIndex]["weather"][0]["icon"],
                      doc["hourly"][hourlyIndex]["temp"].as<float>(), 
                      doc["hourly"][hourlyIndex]["pop"].as<float>());
    } else {
      Serial.print("Warning: Hourly index ");
      Serial.print(hourlyIndex);
      Serial.println(" is out of range");
    }
  }

  // Display Retrieved Data on Serial Monitor
  printWeatherData();
  
  Serial.println("Weather forecast data analyzed successfully");
  return true;
}

/**
 * Fetches and analyzes weather forecast data
 * 
 * Retrieves current weather and hourly forecast data,
 * then stores it in the forecast array
 */
void fetchAndAnalyzeWeatherData() {
  // Fetch weather data
  String jsonData = fetchWeatherData();
  
  // Check if data was fetched successfully
  if (jsonData.isEmpty()) {
    return;
  }
  
  // Store JSON data in global buffer for potential debugging
  jsonBuffer = jsonData;
  
  // Analyze the weather data
  if (!analyzeWeatherData(jsonData)) {
    enterDeepSleep();
  }
}

/**
 * Function to Perform Initialization
 */
void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);
  Serial.println("Weather Display System Starting...");

  // Set E-Paper Display Power Pin
  pinMode(EPD_POWER_PIN, OUTPUT);
  digitalWrite(EPD_POWER_PIN, HIGH);

  // Initialize E-Paper Display GPIO
  EPD_GPIOInit();
  
  // テストモードでない場合のみWiFi接続を行う
  if (!TEST_MODE) {
    // WiFi Connection
    if (!connectToWiFi()) {
      displayErrorMessage("WiFi Connection Error");
      enterDeepSleep();
      return;
    }
  }
  
  // Fetch and Analyze Weather forecast Data
  fetchAndAnalyzeWeatherData();
  
  // テストモードでない場合のみWiFi切断を行う
  if (!TEST_MODE) {
    // Disconnect WiFi (Power Saving)
    disconnectWiFi();
  }

  // Display Weather Forecast
  displayWeatherForecast();
  
  // Enter Deep-Sleep Mode
  enterDeepSleep();
}

/**
 * Main Loop Function
 * Not Executed When Using Deep-Sleep Mode
 */
void loop() {
}
