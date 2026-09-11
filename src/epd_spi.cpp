#include <SPI.h>
#include "epd_spi.h"

// Hardware SPI instance
static SPIClass epd_spi(EPD_SPI_BUS);
static SPISettings epd_spi_settings(EPD_SPI_CLOCK_HZ, SPI_MSBFIRST, SPI_MODE0);

/**
 * @brief       Initialize GPIO pins and hardware SPI
 * @retval      None
 */
void EPD_GPIOInit(void)
{
    // Control pins remain as GPIO
    pinMode(EPD_PIN_RES,  OUTPUT);
    pinMode(EPD_PIN_DC,   OUTPUT);
    pinMode(EPD_PIN_CS,   OUTPUT);
    pinMode(EPD_PIN_BUSY, INPUT);

    // CS idle high
    EPD_CS_Set();

    // Initialize hardware SPI (HSPI: SCK=12, MOSI=11, MISO=-1)
    epd_spi.begin(EPD_PIN_SCK, EPD_PIN_MISO, EPD_PIN_MOSI, EPD_PIN_CS);
}

/**
 * @brief       Transfer one byte via hardware SPI
 * @param       dat: Byte data to be sent
 * @retval      None
 */
void EPD_WR_Bus(uint8_t dat)
{
    epd_spi.beginTransaction(epd_spi_settings);
    EPD_CS_Clr();
    epd_spi.transfer(dat);
    EPD_CS_Set();
    epd_spi.endTransaction();
}

/**
 * @brief       Write register command to EPD
 * @param       reg: Command to write
 * @retval      None
 */
void EPD_WR_REG(uint8_t reg)
{
    EPD_DC_Clr();
    EPD_WR_Bus(reg);
    EPD_DC_Set();
}

/**
 * @brief       Write a byte of data to EPD
 * @param       dat: Data to write
 * @retval      None
 */
void EPD_WR_DATA8(uint8_t dat)
{
    // DC is already HIGH after EPD_WR_REG(); keep it HIGH for data
    EPD_WR_Bus(dat);
}

/**
 * @brief Begin a bulk data transfer session (DC=HIGH, CS=LOW, bus acquired)
 *        Call EPD_WR_DATA_Byte() repeatedly, then EPD_WR_DATA_End()
 */
void EPD_WR_DATA_Begin(void)
{
    epd_spi.beginTransaction(epd_spi_settings);
    EPD_DC_Set();
    EPD_CS_Clr();
}

/**
 * @brief Send one byte within an ongoing bulk transfer (no CS/Transaction overhead)
 */
void EPD_WR_DATA_Byte(uint8_t dat)
{
    epd_spi.transfer(dat);
}

/**
 * @brief End a bulk data transfer session (CS=HIGH, bus released)
 */
void EPD_WR_DATA_End(void)
{
    EPD_CS_Set();
    epd_spi.endTransaction();
}
