/*****************************************************************************
* | File      	:   EPD_2IN9_V2_test.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper V2 test demo
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2023-08-30
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "EPD_2in9_V2.h"
void drawn_alarm_view_mode(uint16_t Xstart, uint16_t Ystart, SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font);
void drawn_alarm_setup_mode(uint16_t Xstart, uint16_t Ystart, SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font);
int EPD_test(void)
{
    printf("EPD_2IN9_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_2IN9_V2_Init();
    EPD_2IN9_V2_Clear();
    DEV_Delay_ms(1000);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
	Paint_Clear(WHITE);

#if 1  //show image for array  
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);  
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in9);

    EPD_2IN9_V2_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1  // Drawing on the image
    EPD_2IN9_V2_Init_Fast();
	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    /*
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
*/
    Paint_DrawString_EN(10, 0, "Duy Ngoc - 2251036", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 30, "Trung Nam - 2251032", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 60, "Hung Minh - 2251030", &Font16, BLACK, WHITE);


//    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
//    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    //Paint_DrawString_CN(130, 0, "���abc", &Font12CN, BLACK, WHITE);
    //Paint_DrawString_CN(130, 20, "΢ѩ����", &Font24CN, WHITE, BLACK);

    EPD_2IN9_V2_Display_Base(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1   //Partial refresh, example shows time    		
	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);  
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
	
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 10;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		EPD_2IN9_V2_Display_Partial(BlackImage);
        DEV_Delay_ms(500);//Analog clock 1s
    }
#endif

#if 1 // show image for array
//    free(BlackImage);
//    printf("show Gray------------------------\r\n");
//    Imagesize = ((EPD_2IN9_V2_WIDTH % 4 == 0)? (EPD_2IN9_V2_WIDTH / 4 ): (EPD_2IN9_V2_WIDTH / 4 + 1)) * EPD_2IN9_V2_HEIGHT;
//    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
//        printf("Failed to apply for black memory...\r\n");
//        return -1;
//    }
//    EPD_2IN9_V2_Gray4_Init();
//    printf("4 grayscale display\r\n");
//    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
//    Paint_SetScale(4);
//    Paint_Clear(0xff);
//
//    Paint_DrawPoint(10, 80, GRAY4, DOT_PIXEL_1X1, DOT_STYLE_DFT);
//    Paint_DrawPoint(10, 90, GRAY4, DOT_PIXEL_2X2, DOT_STYLE_DFT);
//    Paint_DrawPoint(10, 100, GRAY4, DOT_PIXEL_3X3, DOT_STYLE_DFT);
//    Paint_DrawLine(20, 70, 70, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
//    Paint_DrawLine(70, 70, 20, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
//    Paint_DrawRectangle(20, 70, 70, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//    Paint_DrawRectangle(80, 70, 130, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_FULL);
//    Paint_DrawCircle(45, 95, 20, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//    Paint_DrawCircle(105, 95, 20, GRAY2, DOT_PIXEL_1X1, DRAW_FILL_FULL);
//    Paint_DrawLine(85, 95, 125, 95, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
//    Paint_DrawLine(105, 75, 105, 115, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
//    Paint_DrawString_EN(10, 0, "waveshare", &Font16, GRAY4, GRAY1);
//    Paint_DrawString_EN(10, 20, "hello world", &Font12, GRAY3, GRAY1);
//    Paint_DrawNum(10, 33, 123456789, &Font12, GRAY4, GRAY2);
//    Paint_DrawNum(10, 50, 987654321, &Font16, GRAY1, GRAY4);
//    Paint_DrawString_CN(150, 0,"���abc", &Font12CN, GRAY4, GRAY1);
//    Paint_DrawString_CN(150, 20,"���abc", &Font12CN, GRAY3, GRAY2);
//    Paint_DrawString_CN(150, 40,"���abc", &Font12CN, GRAY2, GRAY3);
//    Paint_DrawString_CN(150, 60,"���abc", &Font12CN, GRAY1, GRAY4);
//    Paint_DrawString_CN(150, 80, "΢ѩ����", &Font24CN, GRAY1, GRAY4);
//    EPD_2IN9_V2_4GrayDisplay(BlackImage);
//    DEV_Delay_ms(3000);
//
//    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 0, WHITE);
//    Paint_SetScale(4);
//    Paint_Clear(WHITE);
//    Paint_DrawBitMap(gImage_2in9_4Gray);
//    EPD_2IN9_V2_4GrayDisplay(BlackImage);
//    DEV_Delay_ms(3000);

#endif
    
//	printf("Clear...\r\n");
//	EPD_2IN9_V2_Init();
//    EPD_2IN9_V2_Clear();
	
    printf("Goto Sleep...\r\n");
    EPD_2IN9_V2_Sleep();
//    free(BlackImage);
//    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    //DEV_Module_Exit();
    return 0;
}

// it is the screen display time with the list of name
void default_mode(int *flag_main_screen, uint8_t *hour, uint8_t *minute, uint8_t *second)
{
	UBYTE *BlackImage;

	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

	EPD_2IN9_V2_Init();

		EPD_2IN9_V2_Init();
    	EPD_2IN9_V2_Clear();
    	EPD_2IN9_V2_Init_Fast();
        Paint_ClearWindows(0, 0, EPD_2IN9_V2_HEIGHT, EPD_2IN9_V2_WIDTH, WHITE);

    	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    	printf("Drawing\r\n");
    	//1.Select Image
    	Paint_SelectImage(BlackImage);
    	Paint_Clear(WHITE);

    	// 2.Drawing on the image
    	printf("Drawing:BlackImage\r\n");

    	Paint_DrawString_EN(10, 0, "Duy Ngoc - 2251036", &Font16, BLACK, WHITE);
    	Paint_DrawString_EN(10, 30, "Trung Nam - 2251032", &Font16, BLACK, WHITE);
    	Paint_DrawString_EN(10, 60, "Hung Minh - 2251030", &Font16, BLACK, WHITE);



//      printf("Paint_NewImage\r\n");
//      Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
//      printf("Partial refresh\r\n");
//      Paint_SelectImage(BlackImage);
//
//      PAINT_TIME sPaint_time;
//      sPaint_time.Hour = hour;
//      sPaint_time.Min = minute;
//      sPaint_time.Sec = second;
//
//      Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
//      Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
//      EPD_2IN9_V2_Display_Partial(BlackImage);

	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
	EPD_2IN9_V2_Display_Partial(BlackImage);

    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);

    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = *second;
    UBYTE num = 10;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		EPD_2IN9_V2_Display_Partial(BlackImage);
    }
      EPD_2IN9_V2_Sleep();
      *flag_main_screen = 0;
      free(BlackImage);
}

// mode menu
void menu_set_up (int *flag_set_up,  SYSTEM_MODE *mode, uint8_t *count)
{
	UBYTE *BlackImage;

	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

	EPD_2IN9_V2_Init();
	if(*flag_set_up == 1)
	{
		EPD_2IN9_V2_Init();
    	EPD_2IN9_V2_Clear();
    	//DEV_Delay_ms(1000);
    	EPD_2IN9_V2_Init_Fast();
    	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    	printf("Drawing\r\n");
    	//1.Select Image
    	Paint_SelectImage(BlackImage);
    	Paint_Clear(WHITE);

    	// 2.Drawing on the image
    	printf("Drawing:BlackImage\r\n");

    	Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, BLACK, WHITE);
    	Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, BLACK, WHITE);
    	Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, BLACK, WHITE);
    	Paint_DrawString_EN(10, 45, "3. Alarm Review", &Font12, BLACK, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, BLACK, WHITE);
    	Paint_DrawString_EN(10, 75, "5. System Options", &Font12, BLACK, WHITE);

    	EPD_2IN9_V2_Display_Base(BlackImage);
	}

Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
printf("Partial refresh\r\n");
Paint_SelectImage(BlackImage);
UBYTE num = 3;
if (*count > 6)
{
    *count = 0;
}
for (;;) {

    /*
    Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
    Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
*/
	switch (*mode){
        case DEFAULT_MODE:
        Paint_ClearWindows(10, 0, 10 + Font12.Width * 20, 0 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, WHITE, BLACK);
        //mode_1
        Paint_ClearWindows(10, 15, 10 + Font12.Width * 20, 15 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, BLACK, WHITE);
        //mode 2
        Paint_ClearWindows(10, 30, 10 + Font12.Width * 20, 30 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, BLACK, WHITE);
        //mode 3
        Paint_ClearWindows(10, 45, 10 + Font12.Width * 20, 45 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 45, "3. Alarm Review", &Font12, BLACK, WHITE);
        //mode 4
        Paint_ClearWindows(10, 60, 10 + Font12.Width * 20, 60 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, BLACK, WHITE);
    	//mode 5
        Paint_ClearWindows(10, 75, 10 + Font12.Width * 20, 75 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 75, "5. System Options", &Font12, BLACK, WHITE);
        break;

        case TIME_SETUP_MODE:
        Paint_ClearWindows(10, 0, 10 + Font12.Width * 20, 0 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, BLACK, WHITE);
        //mode_1
        Paint_ClearWindows(10, 15, 10 + Font12.Width * 20, 15 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, WHITE, BLACK);
        //mode 2
        Paint_ClearWindows(10, 30, 10 + Font12.Width * 20, 30 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, BLACK, WHITE);
        //mode 3
        Paint_ClearWindows(10, 45, 10 + Font12.Width * 20, 45 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 45, "3. Alarm Review", &Font12, BLACK, WHITE);
        //mode 4
        Paint_ClearWindows(10, 60, 10 + Font12.Width * 20, 60 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, BLACK, WHITE);
    	//mode 5
        Paint_ClearWindows(10, 75, 10 + Font12.Width * 20, 75 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 75, "5. System Options", &Font12, BLACK, WHITE);
        break;

        case ALARM_SETUP_MODE:
        Paint_ClearWindows(10, 0, 10 + Font12.Width * 20, 0 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, BLACK, WHITE);
        //mode_1
        Paint_ClearWindows(10, 15, 10 + Font12.Width * 20, 15 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, BLACK, WHITE);
        //mode 2
        Paint_ClearWindows(10, 30, 10 + Font12.Width * 20, 30 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, WHITE, BLACK);
        //mode 3
        Paint_ClearWindows(10, 45, 10 + Font12.Width * 20, 45 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 45, "3. Alarm Review", &Font12, BLACK, WHITE);
        //mode 4
        Paint_ClearWindows(10, 60, 10 + Font12.Width * 20, 60 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, BLACK, WHITE);
    	//mode 5
        Paint_ClearWindows(10, 75, 10 + Font12.Width * 20, 75 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 75, "5. System Options", &Font12, BLACK, WHITE);
        break;

        case ALARM_VIEW_MODE:
        Paint_ClearWindows(10, 0, 10 + Font12.Width * 20, 0 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, BLACK, WHITE);
        //mode_1
        Paint_ClearWindows(10, 15, 10 + Font12.Width * 20, 15 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, BLACK, WHITE);
        //mode 2
        Paint_ClearWindows(10, 30, 10 + Font12.Width * 20, 30 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, BLACK, WHITE);
        //mode 3
        Paint_ClearWindows(10, 45, 10 + Font12.Width * 20, 45 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 45, "3. Alarm Revie", &Font12, WHITE, BLACK);
        //mode 4
        Paint_ClearWindows(10, 60, 10 + Font12.Width * 20, 60 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, BLACK, WHITE);
    	//mode 5
        Paint_ClearWindows(10, 75, 10 + Font12.Width * 20, 75 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 75, "5. System Options", &Font12, BLACK, WHITE);
        break;

        case ALARM_ACTIVE_MODE:

        Paint_ClearWindows(10, 0, 10 + Font12.Width * 20, 0 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, BLACK, WHITE);
        //mode_1
        Paint_ClearWindows(10, 15, 10 + Font12.Width * 20, 15 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, BLACK, WHITE);
        //mode 2
        Paint_ClearWindows(10, 30, 10 + Font12.Width * 20, 30 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, BLACK, WHITE);
        //mode 3
        Paint_ClearWindows(10, 45, 10 + Font12.Width * 20, 45 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 45, "3. Alarm Revie", &Font12, BLACK, WHITE);
        //mode 4
        Paint_ClearWindows(10, 60, 10 + Font12.Width * 20, 60 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, WHITE, BLACK);
    	//mode 5
        Paint_ClearWindows(10, 75, 10 + Font12.Width * 20, 75 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 75, "5. System Options", &Font12, BLACK, WHITE);
        break;

        case SYSTEM_OPTIONS_MODE:
        Paint_ClearWindows(10, 0, 10 + Font12.Width * 20, 0 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. Default Mode", &Font12, BLACK, WHITE);
        //mode_1
        Paint_ClearWindows(10, 15, 10 + Font12.Width * 20, 15 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 15, "1. Time Setup", &Font12, BLACK, WHITE);
        //mode 2
        Paint_ClearWindows(10, 30, 10 + Font12.Width * 20, 30 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 30, "2. Alarm Setup", &Font12, BLACK, WHITE);
        //mode 3
        Paint_ClearWindows(10, 45, 10 + Font12.Width * 20, 45 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 45, "3. Alarm Revie", &Font12, BLACK, WHITE);
        //mode 4
        Paint_ClearWindows(10, 60, 10 + Font12.Width * 20, 60 + Font12.Height, WHITE);
    	Paint_DrawString_EN(10, 60, "4. Alarm Active", &Font12, BLACK, WHITE);
    	//mode 5
        Paint_ClearWindows(10, 75, 10 + Font12.Width * 20, 75 + Font12.Height, WHITE);
        Paint_DrawString_EN(10, 75, "5. System Options", &Font12, WHITE, BLACK);
        break;
    }

    num = num - 1;
    if(num == 0) {
        break;
    }
	EPD_2IN9_V2_Display_Partial(BlackImage);
}
  *flag_set_up = 0;
  EPD_2IN9_V2_Sleep();
  free(BlackImage);
}

// alarm view mode
void alarm_view_mode (int *flag_alarm_view_mode_set_up,  uint8_t *count_view, uint8_t *count_select, SYSTEM_PARAM_DATA_ALARM_VIEW_MODE *alarm_view_mode)
{
	UBYTE *BlackImage;

	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

	EPD_2IN9_V2_Init();
	if(*flag_alarm_view_mode_set_up == 1)
	{
		EPD_2IN9_V2_Init();
    	EPD_2IN9_V2_Clear();
    	//DEV_Delay_ms(1000);
    	EPD_2IN9_V2_Init_Fast();
    	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    	printf("Drawing\r\n");
    	//1.Select Image
    	Paint_SelectImage(BlackImage);
    	Paint_Clear(WHITE);

    	// 2.Drawing view mode
    	printf("Drawing:BlackImage\r\n");
        //parameter 1
    	Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font16);
        //parameter 2
    	Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);
        //parameter 3
    	Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);
        //parameter 4
    	Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);
        //parameter 5
    	Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);
    	EPD_2IN9_V2_Display_Base(BlackImage);
	}
    UBYTE num = 3;
    if(*count_view > 1)
    {
        *count_view = 0;
    }

    if (*count_select > 4)
    {
        *count_select = 0;
    }

    if(*count_view == 0)
    {
        for (;;) {
        
            /*
            Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
            Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
        */
            switch (*count_select)
            {
                case 0:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, WHITE, BLACK);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 2, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                break;
        
                case 1:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, WHITE, BLACK);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 2, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                break;
        
                case 2:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, WHITE, BLACK);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 2, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                break;
        
                case 3:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, WHITE, BLACK);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 2, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                break;
        
                case 4:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 2, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, WHITE, BLACK);
                break;
        
                default: break;
            }

    num = num - 1;
    if(num == 0) {
        num = 3;
        break;
    }
	EPD_2IN9_V2_Display_Partial(BlackImage);
}
    }

    else
    {
        for (;;) 
        {
        
            /*
            Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
            Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
        */
            switch (*count_select)
            {
                case 0:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, WHITE, BLACK);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 3, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                break;
        
                case 1:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, WHITE, BLACK);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 3, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                break;
        
                case 2:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, WHITE, BLACK);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 3, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                break;
        
                case 3:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, WHITE, BLACK);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 3, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                break;
        
                case 4:
                //parameter 1
                Paint_ClearWindows(10, 0, 10 + Font16.Width * 2, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                //parameter 2
                Paint_ClearWindows(10, 20, 10 + Font16.Width * 2, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                //parameter 3
                Paint_ClearWindows(10, 40, 10 + Font16.Width * 2, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                //parameter 4
                Paint_ClearWindows(10, 60, 10 + Font16.Width * 2, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                //parameter 5
                Paint_ClearWindows(10, 80, 10 + Font16.Width * 3, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, WHITE, BLACK);
                break;
        
                default: break;
            }

            num = num - 1;
            if(num == 0) {
                num = 3;
                break;
            }
	        EPD_2IN9_V2_Display_Partial(BlackImage);
    }
}

  *flag_alarm_view_mode_set_up = 0;
  EPD_2IN9_V2_Sleep();
  free(BlackImage);
}

void drawn_alarm_view_mode(uint16_t Xstart, uint16_t Ystart, SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font)
{
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = sParam_data_alarm_setup_mode->hour;
    sPaint_time.Min = sParam_data_alarm_setup_mode->minute;
    sPaint_time.Sec = sParam_data_alarm_setup_mode->second;
    Paint_DrawTime(50, Ystart, &sPaint_time, &Font, WHITE, BLACK);
    if(sParam_data_alarm_setup_mode->on_off == TRUE)
    {
    Paint_DrawRectangle(Xstart, Ystart + 2, Xstart + 10, Ystart + 12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(Xstart + 20, Ystart, "ON", &Font16, BLACK, WHITE);
    }

    if(sParam_data_alarm_setup_mode->on_off == FALSE)
    {
    Paint_DrawRectangle(Xstart, Ystart + 2, Xstart + 10, Ystart + 12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(Xstart + 20, Ystart, "OFF", &Font16, BLACK, WHITE);
    }
}

//alarm setup mode
void alarm_setup_mode(int *flag_alarm_set_up_mode, uint8_t *count_view, uint8_t *count_select, uint8_t *count_edit, SYSTEM_PARAM_DATA_ALARM_VIEW_MODE *alarm_view_mode)
{
	UBYTE *BlackImage;
    UBYTE num = 3;
	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

    SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *sParam_data_alarm_setup_mode = (SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *)malloc(sizeof(SYSTEM_PARAM_DATA_ALARM_SETUP_MODE));


    if(*count_view > 1)
    {
        *count_view = 0;
    }

    if(*count_select > 4)
    {
        *count_select = 0;
    }

    if(*count_edit > 5)
    {
        *count_edit = 0;
    }
    //select param data
    if(*count_view == 0)
    {
        switch (*count_select)
        {
            case 0:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_1;
            break;

            case 1:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_2;
            break;

            case 2:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_3;
            break;

            case 3:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_4;
            break;

            case 4:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_5;
            break;

            default:
            break;
        }
    }
    
    else
        {
            switch (*count_select)
        {
            case 0:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_6;
            break;

            case 1:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_7;
            break;

            case 2:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_8;
            break;

            case 3:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_9;
            break;

            case 4:
            sParam_data_alarm_setup_mode = alarm_view_mode->sParam_data_alarm_setup_mode_10;
            break;

            default:
            break;
        }
    }

	EPD_2IN9_V2_Init();
	if(*flag_alarm_set_up_mode == 1)
	{
		EPD_2IN9_V2_Init();
    	EPD_2IN9_V2_Clear();
    	DEV_Delay_ms(1000);
    	EPD_2IN9_V2_Init_Fast();
    	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    	printf("Drawing\r\n");
    	//1.Select Image
    	Paint_SelectImage(BlackImage);
    	Paint_Clear(WHITE);

    	// 2.Drawing on the image
        if(*count_view == 0)
        {
            switch (*count_select)
            {
                case 0:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font24);
                break;

                case 1:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font24);
                break;

                case 2:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font24);
                break;

                case 3:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font24);
                break;

                case 4:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font24);
                break;

                default:
                break;
            }
        }
        
        else
            {
                switch (*count_select)
            {
                case 0:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_6, Font24);
                break;

                case 1:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_7, Font24);
                break;

                case 2:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_8, Font24);
                break;

                case 3:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_9, Font24);
                break;

                case 4:
                drawn_alarm_setup_mode(90, 100, alarm_view_mode->sParam_data_alarm_setup_mode_10, Font24);
                break;

                default:
                break;
            }
        }


    	EPD_2IN9_V2_Display_Base(BlackImage);
	}    

	
        // Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        // Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        // num = num - 1;
        // if(num == 0) {
        // }
		// EPD_2IN9_V2_Display_Partial(BlackImage);
        switch (*count_edit)
        {
            case 0:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, sParam_data_alarm_setup_mode, Font24);
                for(;;)
                {
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
		            EPD_2IN9_V2_Display_Partial(BlackImage);
                }
                break;
            //set hour
            case 1:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, sParam_data_alarm_setup_mode, Font24);
                for(;;)
                {
                    Paint_ClearWindows(100, 50, 100 + Font24.Width * 2, 50 + Font24.Height, WHITE);
                    Paint_DrawNum(100, 50, sParam_data_alarm_setup_mode->hour / 10, &Font24, WHITE, BLACK);
                    Paint_DrawNum(100 + Font24.Width * 1, 50, sParam_data_alarm_setup_mode->hour % 10, &Font24, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
		            EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            //set minute
            case 2:
            /* code */
            Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
            drawn_alarm_setup_mode(90, 100, sParam_data_alarm_setup_mode, Font24);
            for(;;)
            {
                Paint_ClearWindows(100 + Font24.Width * 2 + Font24.Width / 2, 50, 100 + Font24.Width * 4 +  Font24.Width / 4, 50 + Font24.Height, WHITE);
                Paint_DrawNum(100 + Font24.Width * 2 + Font24.Width / 2, 50, sParam_data_alarm_setup_mode->minute / 10, &Font24, WHITE, BLACK);
                Paint_DrawNum(100 + Font24.Width * 3 + Font24.Width / 2, 50, sParam_data_alarm_setup_mode->minute % 10, &Font24, WHITE, BLACK);
                num = num - 1;
                if(num == 0) {
                    num = 3;
                    break;
                }
                EPD_2IN9_V2_Display_Partial(BlackImage);
            }
            break;
            //set second
            case 3:
            /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, sParam_data_alarm_setup_mode, Font24);
                for(;;)
                {
                    Paint_ClearWindows(100 + Font24.Width * 5, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                    Paint_DrawNum(100 + Font24.Width * 5, 50, sParam_data_alarm_setup_mode->second / 10, &Font24, WHITE, BLACK);
                    Paint_DrawNum(100 + Font24.Width * 6, 50, sParam_data_alarm_setup_mode->second % 10, &Font24, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            //set on
            case 4:
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                for(;;)
                {
                    Paint_ClearWindows(90, 100, 90 + 100, 100 + Font16.Height, WHITE);
                    sParam_data_alarm_setup_mode->on_off = TRUE;
                    drawn_alarm_setup_mode(90, 100, sParam_data_alarm_setup_mode, Font24);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            //set off
            case 5:
            Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
            for(;;)
            {
                Paint_ClearWindows(90, 100, 90 + 100, 100 + Font16.Height, WHITE);
                sParam_data_alarm_setup_mode->on_off = FALSE;
                drawn_alarm_setup_mode(90, 100, sParam_data_alarm_setup_mode, Font24);
                num = num - 1;
                if(num == 0) {
                    num = 3;
                    break;
                }
                EPD_2IN9_V2_Display_Partial(BlackImage);
            }
            break;

        default:
            break;
        }
      EPD_2IN9_V2_Sleep();
      *flag_alarm_set_up_mode = 0;
      free(BlackImage);
      free(sParam_data_alarm_setup_mode);
}

void drawn_alarm_setup_mode(uint16_t Xstart, uint16_t Ystart, SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font)
{
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = sParam_data_alarm_setup_mode->hour;
    sPaint_time.Min = sParam_data_alarm_setup_mode->minute;
    sPaint_time.Sec = sParam_data_alarm_setup_mode->second;
    Paint_DrawTime(100, 50, &sPaint_time, &Font, WHITE, BLACK);
    Paint_DrawString_EN(Xstart + 20, Ystart, "ON", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(Xstart + 50, Ystart, "/", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(Xstart + 90, Ystart, "OFF", &Font16, BLACK, WHITE);

    if(sParam_data_alarm_setup_mode->on_off == TRUE)
    {
        Paint_DrawRectangle(Xstart, Ystart + 2, Xstart + 10, Ystart + 12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(Xstart + 70, Ystart + 2, Xstart + 80, Ystart + 12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    }

    if(sParam_data_alarm_setup_mode->on_off == FALSE)
    {
        Paint_DrawRectangle(Xstart, Ystart + 2, Xstart + 10, Ystart + 12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(Xstart + 70, Ystart + 2, Xstart + 80, Ystart + 12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }
}

//Time setup mode
void time_setup_mode(int *flag_time_set_up_mode, uint8_t *count_edit, SYSTEM_PARAM_DATA_ALARM_SETUP_MODE *time_setup_param)
{
    UBYTE *BlackImage;
    UBYTE num = 3;
	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

    if(*count_edit > 7)
    {
        *count_edit = 0;
    }
    
	EPD_2IN9_V2_Init();
	if(*flag_time_set_up_mode == 1)
	{
		EPD_2IN9_V2_Init();
    	EPD_2IN9_V2_Clear();
    	DEV_Delay_ms(1000);
    	EPD_2IN9_V2_Init_Fast();
    	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    	printf("Drawing\r\n");
    	//1.Select Image
    	Paint_SelectImage(BlackImage);
    	Paint_Clear(WHITE);

    	// 2.Drawing on the image


        drawn_alarm_setup_mode(90, 100, time_setup_param, Font24);
        Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
    	EPD_2IN9_V2_Display_Base(BlackImage);
	}    

	
        // Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        // Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        // num = num - 1;
        // if(num == 0) {
        // }
		// EPD_2IN9_V2_Display_Partial(BlackImage);
        switch (*count_edit)
        {
            case 0:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, time_setup_param, Font24);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                for(;;)
                {
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
		            EPD_2IN9_V2_Display_Partial(BlackImage);
                }
                break;
            //set hour
            case 1:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, time_setup_param, Font24);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                for(;;)
                {
                    Paint_ClearWindows(100, 50, 100 + Font24.Width * 2, 50 + Font24.Height, WHITE);
                    Paint_DrawNum(100, 50, time_setup_param->hour / 10, &Font24, WHITE, BLACK);
                    Paint_DrawNum(100 + Font24.Width * 1, 50, time_setup_param->hour % 10, &Font24, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
		            EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            //set minute
            case 2:
            /* code */
            Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
            drawn_alarm_setup_mode(90, 100, time_setup_param, Font24);
            Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
            for(;;)
            {
                Paint_ClearWindows(100 + Font24.Width * 2 + Font24.Width / 2, 50, 100 + Font24.Width * 4 +  Font24.Width / 4, 50 + Font24.Height, WHITE);
                Paint_DrawNum(100 + Font24.Width * 2 + Font24.Width / 2, 50, time_setup_param->minute / 10, &Font24, WHITE, BLACK);
                Paint_DrawNum(100 + Font24.Width * 3 + Font24.Width / 2, 50, time_setup_param->minute % 10, &Font24, WHITE, BLACK);
                num = num - 1;
                if(num == 0) {
                    num = 3;
                    break;
                }
                EPD_2IN9_V2_Display_Partial(BlackImage);
            }
            break;
            //set second
            case 3:
            /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, time_setup_param, Font24);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                for(;;)
                {
                    Paint_ClearWindows(100 + Font24.Width * 5, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                    Paint_DrawNum(100 + Font24.Width * 5, 50, time_setup_param->second / 10, &Font24, WHITE, BLACK);
                    Paint_DrawNum(100 + Font24.Width * 6, 50, time_setup_param->second % 10, &Font24, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
        default:
            break;
        }
      EPD_2IN9_V2_Sleep();
      *flag_time_set_up_mode = 0;
      free(BlackImage);
}
