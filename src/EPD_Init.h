#ifndef _EPD_INIT_H_
#define _EPD_INIT_H_

#include "spi.h"

// Since the 5.97 Inch E-Paper screen is controlled by two SSD1683 ICs,
// and the resolution of SSD1683 is 400x300, the resolution of E-Paper is 792x272.
// Therefore, the master-slave chips use a resolution of 396x272 to cascade
// them together for display. This results in a space of 8 columns of pixels
// at the junction of the two ICs, so address offset is required when displaying.
//
// Therefore, the program defines EPD_W and EPD_H as 800x272,
// but the actual display area is still 792x272.
//
// At the same time, if you use the EPD_Display function to directly
// display a full-screen image, the modulation resolution needs to be 800x272.
// If you use the EPD_ShowPicture function to display a full-screen image,
// the modulation resolution is 792x272.
#define EPD_W 800
#define EPD_H 272

#define WHITE 0xFF
#define BLACK 0x00

#define ALLSCREEN_GRAGHBYTES  27200/2

#define Source_BYTES 400/8
#define Gate_BITS 272
#define ALLSCREEN_BYTES Source_BYTES*Gate_BITS

void EPD_READBUSY(void);
void EPD_HW_RESET(void);
void EPD_Update(void);
void EPD_PartUpdate(void);
void EPD_FastUpdate(void);
void EPD_DeepSleep(void);
void EPD_Init(void);
void EPD_FastMode1Init(void);
void EPD_SetRAMMP(void);
void EPD_SetRAMMA(void);
void EPD_SetRAMSP(void);
void EPD_SetRAMSA(void);
void EPD_Clear_R26A6H(void);
void EPD_Display_Clear(void);
void EPD_Display(const uint8_t *ImageBW);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);
#endif
