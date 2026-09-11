#ifndef EPD_SPI_H_
#define EPD_SPI_H_

#include <Arduino.h>
// NOTE: <SPI.h> is intentionally NOT included here.
// It is included only in epd_spi.cpp to avoid filename collision
// with src/spi.h on case-insensitive file systems (Windows).

// SPI Pins (HSPI on ESP32-S3: SCK=12, MOSI=11)
#define EPD_PIN_SCK  12
#define EPD_PIN_MOSI 11
#define EPD_PIN_MISO (-1)  // Not used by EPD
#define EPD_PIN_RES  47
#define EPD_PIN_DC   46
#define EPD_PIN_CS   45
#define EPD_PIN_BUSY 48

// Hardware SPI bus and clock speed (SSD1683 supports up to 20 MHz)
#define EPD_SPI_BUS      HSPI
#define EPD_SPI_CLOCK_HZ 20000000UL

// Control pin macros (DC / CS / RES remain as GPIO)
#define EPD_RES_Clr() digitalWrite(EPD_PIN_RES, LOW)
#define EPD_RES_Set() digitalWrite(EPD_PIN_RES, HIGH)

#define EPD_DC_Clr()  digitalWrite(EPD_PIN_DC, LOW)
#define EPD_DC_Set()  digitalWrite(EPD_PIN_DC, HIGH)

#define EPD_CS_Clr()  digitalWrite(EPD_PIN_CS, LOW)
#define EPD_CS_Set()  digitalWrite(EPD_PIN_CS, HIGH)

#define EPD_ReadBUSY  digitalRead(EPD_PIN_BUSY)

void EPD_GPIOInit(void);
void EPD_WR_Bus(uint8_t dat);
void EPD_WR_REG(uint8_t reg);
void EPD_WR_DATA8(uint8_t dat);

/**
 * @brief Begin a multi-byte SPI data transfer (DC=HIGH, CS=LOW)
 *        Must be paired with EPD_WR_DATA_End()
 */
void EPD_WR_DATA_Begin(void);

/**
 * @brief Send one byte within an ongoing multi-byte transfer
 *        Call between EPD_WR_DATA_Begin() and EPD_WR_DATA_End()
 */
void EPD_WR_DATA_Byte(uint8_t dat);

/**
 * @brief End a multi-byte SPI data transfer (CS=HIGH, release bus)
 */
void EPD_WR_DATA_End(void);

#endif // EPD_SPI_H_
