/**
 * Test weather data in JSON format
 * This file contains a sample JSON response from OpenWeatherMap API
 * for testing purposes.
 */

#ifndef TESTDATA_H
#define TESTDATA_H

// Sample weather data JSON string
const char* TEST_WEATHER_DATA = R"(
{
    "lat": 35.681,
    "lon": 139.7670,
    "timezone": "Asia/Tokyo",
    "timezone_offset": 32400,
    "current": {
        "dt": 1749681000,
        "sunrise": 1749756269,
        "sunset": 1749808638,
        "temp": 15.00,
        "feels_like": 12.34,
        "pressure": 1234,
        "humidity": 12,
        "dew_point": 12.3,
        "uvi": 1.23,
        "clouds": 12,
        "visibility": 12345,
        "wind_speed": 1.2,
        "wind_deg": 12,
        "weather": [
            {
                "id": 123,
                "main": "dummy",
                "description": "dummy",
                "icon": "10d"
            }
        ]
    },
    "hourly": [
        {
            "dt": 1749690000,
            "temp": 16.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "09d"
                }
            ],
            "pop": 0.8
        },
        {
            "dt": 1749690000,
            "temp": 16.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "09d"
                }
            ],
            "pop": 0.8
        },
        {
            "dt": 1749690000,
            "temp": 16.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "09d"
                }
            ],
            "pop": 0.8
        },
        {
            "dt": 1749690000,
            "temp": 16.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "09d"
                }
            ],
            "pop": 0.8
        },
        {
            "dt": 1749700800,
            "temp": 18.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "02d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749700800,
            "temp": 18.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "02d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749700800,
            "temp": 18.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "02d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749711600,
            "temp": 24.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "01d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749711600,
            "temp": 24.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "01d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749711600,
            "temp": 24.00,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "01d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749722400,
            "temp": 23.0,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "01n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749722400,
            "temp": 23.0,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "01n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1749722400,
            "temp": 23.0,
            "feels_like": 12.34,
            "pressure": 1234,
            "humidity": 12,
            "dew_point": 12.3,
            "uvi": 1.23,
            "clouds": 12,
            "visibility": 12345,
            "wind_speed": 1.2,
            "wind_deg": 12,
            "weather": [
                {
                    "id": 123,
                    "main": "dummy",
                    "description": "dummy",
                    "icon": "01n"
                }
            ],
            "pop": 0
        }
    ]
}
)";

#endif // TESTDATA_H
