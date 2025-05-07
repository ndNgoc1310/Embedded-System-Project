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
void drawn_alarm_view_mode(uint16_t Xstart, uint16_t Ystart, ALARM_DATA_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font);
void drawn_alarm_setup_mode(uint16_t Xstart, uint16_t Ystart, ALARM_DATA_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font);
void draw_battery(uint16_t battery_percentage);
void draw_day(uint16_t day, uint16_t date, uint16_t month, uint16_t year);

void default_mode(FLAG_SYSTEM *flag_default_mode, TIME_DATA *time_get_data, uint16_t battery_percentage)
{
	UBYTE *BlackImage;

	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

    EPD_2IN9_V2_Init_Fast();
    if(flag_default_mode ->flag_DEFAULT_MODE == 1)
    {
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

    	// Paint_DrawString_EN(10, 0, "Duy Ngoc - 2251036", &Font16, BLACK, WHITE);
    	// Paint_DrawString_EN(10, 30, "Trung Nam - 2251032", &Font16, BLACK, WHITE);
    	// Paint_DrawString_EN(10, 60, "Hung Minh - 2251030", &Font16, BLACK, WHITE);

    	Paint_DrawString_EN(35, 40, "ChronoSync: Embedded Digital CLOCK ", &Font12, BLACK, WHITE);


	    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
        EPD_2IN9_V2_Display(BlackImage);
        flag_default_mode->flag_ALARM_SETUP_MODE = 1;
        flag_default_mode->flag_ALARM_VIEW_MODE = 1;
        flag_default_mode->flag_SYSTEM_SETUP_MODE = 1;
        flag_default_mode->flag_TIME_SETUP_MODE = 1;
    }
        Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
        EPD_2IN9_V2_Display_Partial(BlackImage);
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);

    PAINT_TIME sPaint_time;
    sPaint_time.Hour = time_get_data->hour;
    sPaint_time.Min = time_get_data->minute;
    sPaint_time.Sec = 0;
    UBYTE num = 3;
    for (;;) {
        Paint_ClearWindows(100, 60, 100 + Font20.Width * 7, 60 + Font20.Height, WHITE);
        Paint_ClearWindows(70, 100, 296, 100 + Font20.Height, WHITE);
        Paint_DrawTime(100, 60, &sPaint_time, &Font20, WHITE, BLACK);
        draw_day(time_get_data->dayofweek, time_get_data->dateofmonth, time_get_data->month, time_get_data->year);
        draw_battery(battery_percentage);
        num = num - 1;
        if(num == 0) {
            break;
        }
		EPD_2IN9_V2_Display_Partial(BlackImage);
    }
      EPD_2IN9_V2_Sleep();
      flag_default_mode->flag_DEFAULT_MODE = 0;
      free(BlackImage);
}

// mode menu
void system_setup_mode (FLAG_SYSTEM *flag_set_up,  SYSTEM_STATE *system_state, uint8_t battery_percentage)
{
	UBYTE *BlackImage;

	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

	EPD_2IN9_V2_Init();
	if(flag_set_up->flag_SYSTEM_SETUP_MODE == 1)
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

 
    
    	EPD_2IN9_V2_Display_Base(BlackImage);
        flag_set_up->flag_ALARM_SETUP_MODE  = 1;
        flag_set_up->flag_ALARM_VIEW_MODE   = 1;
        flag_set_up->flag_DEFAULT_MODE      = 1;
        flag_set_up->flag_TIME_SETUP_MODE   = 1;
	}

Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
printf("Partial refresh\r\n");
Paint_SelectImage(BlackImage);
UBYTE num = 3;

for (;;) {

    /*
    Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
    Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
*/
    draw_battery(battery_percentage);
	switch (system_state->system_opt_cursor){
        case CLEAR_ALL_ALARM:
        Paint_ClearWindows(10, 0, 10 + Font16.Width * 20, 0 + Font16.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. CLEAR ALL ALARM", &Font16, WHITE, BLACK);
        //mode_1
        Paint_ClearWindows(10, 40, 10 + Font16.Width * 20, 40 + Font16.Height, WHITE);
        Paint_DrawString_EN(10, 40, "1. CONTRIBUTOR INFO", &Font16, BLACK, WHITE);
        break;

        case CONTRIBUTOR_INFO:
        Paint_ClearWindows(10, 0, 10 + Font16.Width * 20, 0 + Font16.Height, WHITE);
        Paint_DrawString_EN(10, 0, "0. CLEAR ALL ALARM", &Font16, BLACK, WHITE);
        //mode_1
        Paint_ClearWindows(10, 40, 10 + Font16.Width * 20, 40 + Font16.Height, WHITE);
        Paint_DrawString_EN(10, 40, "1. CONTRIBUTOR INFO", &Font16, WHITE, BLACK);
        break;
       default: break;
    }

    num = num - 1;
    if(num == 0) {
        break;
    }
	EPD_2IN9_V2_Display_Partial(BlackImage);
}
  flag_set_up->flag_SYSTEM_SETUP_MODE = 0;
  EPD_2IN9_V2_Sleep();
  free(BlackImage);
}

// alarm view mode
void alarm_view_mode (FLAG_SYSTEM *flag_alarm_view_mode, SYSTEM_STATE *system_state, SYSTEM_PARAM_DATA_ALARM_VIEW_MODE *alarm_view_mode, uint16_t battery_percentage)
{
	UBYTE *BlackImage;

	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

	EPD_2IN9_V2_Init();
	if(flag_alarm_view_mode->flag_ALARM_VIEW_MODE == 1)
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
        // switch (alarm_view_mode->sParam_data_alarm_setup_mode_1->dy_dt)
        // {
        // case DAY_OF_WEEK_MODE:
        //     /* code */
        //     Paint_DrawNum(200, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 0, "DY", &Font16, BLACK, WHITE);
        //     break;
        // case DATE_OF_MONTH_MODE:
        //     Paint_DrawNum(200, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 0, "DT", &Font16, BLACK, WHITE);
        //     break;
        // case NOT_USED_MODE:
        //     break;
        // default:
        //     break;
        // }
        //parameter 2
    	Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);
        // switch (alarm_view_mode->sParam_data_alarm_setup_mode_2->dy_dt)
        // {
        // case DAY_OF_WEEK_MODE:
        //     /* code */
        //     Paint_DrawNum(200, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 20, "DY", &Font16, BLACK, WHITE);
        //     break;
        // case DATE_OF_MONTH_MODE:
        //     Paint_DrawNum(200, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 20, "DT", &Font16, BLACK, WHITE);
        //     break;
        // case NOT_USED_MODE:
        //     break;
        // default:
        //     break;
        // }
        //parameter 3
    	Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);
        // switch (alarm_view_mode->sParam_data_alarm_setup_mode_3->dy_dt)
        // {
        // case DAY_OF_WEEK_MODE:
        //     /* code */
        //     Paint_DrawNum(200, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 40, "DY", &Font16, BLACK, WHITE);
        //     break;
        // case DATE_OF_MONTH_MODE:
        //     Paint_DrawNum(200, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 40, "DT", &Font16, BLACK, WHITE);
        //     break;
        // case NOT_USED_MODE:
        //     break;
        // default:
        //     break;
        // }
        //parameter 4
    	Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);
        // switch (alarm_view_mode->sParam_data_alarm_setup_mode_4->dy_dt)
        // {
        // case DAY_OF_WEEK_MODE:
        //     /* code */
        //     Paint_DrawNum(200, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 60, "DY", &Font16, BLACK, WHITE);
        //     break;
        // case DATE_OF_MONTH_MODE:
        //     Paint_DrawNum(200, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 60, "DT", &Font16, BLACK, WHITE);
        //     break;
        // case NOT_USED_MODE:
        //     break;
        // default:
        //     break;
        // }
        //parameter 5
    	Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
        drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);
        // switch (alarm_view_mode->sParam_data_alarm_setup_mode_5->dy_dt)
        // {
        // case DAY_OF_WEEK_MODE:
        //     /* code */
        //     Paint_DrawNum(200, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 80, "DY", &Font16, BLACK, WHITE);
        //     break;
        // case DATE_OF_MONTH_MODE:
        //     Paint_DrawNum(200, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom/10, &Font16, BLACK, WHITE);
        //     Paint_DrawNum(210, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom%10, &Font16, BLACK, WHITE);
        //     Paint_DrawString_EN(230, 80, "DY", &Font16, BLACK, WHITE);
        //     break;
        // case NOT_USED_MODE:
        //     break;
        // default:
        
    	EPD_2IN9_V2_Display_Base(BlackImage);
        flag_alarm_view_mode->flag_ALARM_SETUP_MODE  = 1;
        flag_alarm_view_mode->flag_SYSTEM_SETUP_MODE = 1;
        flag_alarm_view_mode->flag_DEFAULT_MODE      = 1;
        flag_alarm_view_mode->flag_TIME_SETUP_MODE   = 1;
	}
    UBYTE num = 3;
    uint32_t count_change_screen;
    if(system_state->alarm_view_cursor < 5)
    {
        count_change_screen = 0;
    }

    else
    {
        count_change_screen = 1;
    }

    if(count_change_screen  == 0)
    {
        for (;;) {
            draw_battery(battery_percentage);
            /*
            Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
            Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
        */
            switch (system_state->alarm_view_cursor)
            {
                case 0:
                //parameter 1
                Paint_ClearWindows(10, 0, 255, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font16);
                // switch (alarm_view_mode->sParam_data_alarm_setup_mode_1->dy_dt)
                //     {
                //     case DAY_OF_WEEK_MODE:
                //         /* code */
                //         Paint_DrawNum(200, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom/10, &Font16, BLACK, WHITE);
                //         Paint_DrawNum(210, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom%10, &Font16, BLACK, WHITE);
                //         Paint_DrawString_EN(230, 0, "DY", &Font16, BLACK, WHITE);
                //         break;
                //     case DATE_OF_MONTH_MODE:
                //         Paint_DrawNum(200, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom/10, &Font16, BLACK, WHITE);
                //         Paint_DrawNum(210, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1->dow_dom%10, &Font16, BLACK, WHITE);
                //         Paint_DrawString_EN(230, 0, "DT", &Font16, BLACK, WHITE);
                //         break;
                //     case NOT_USED_MODE:
                //         break;
                //     default:
                //         break;
                //     }
       //parameter 2
       Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
       drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);
    //    switch (alarm_view_mode->sParam_data_alarm_setup_mode_2->dy_dt)
    //    {
    //    case DAY_OF_WEEK_MODE:
    //        /* code */
    //        Paint_DrawNum(200, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 20, "DY", &Font16, BLACK, WHITE);
    //        break;
    //    case DATE_OF_MONTH_MODE:
    //        Paint_DrawNum(200, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 20, "DT", &Font16, BLACK, WHITE);
    //        break;
    //    case NOT_USED_MODE:
    //        break;
    //    default:
    //        break;
    //    }
       //parameter 3
       Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
       drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);
    //    switch (alarm_view_mode->sParam_data_alarm_setup_mode_3->dy_dt)
    //    {
    //    case DAY_OF_WEEK_MODE:
    //        /* code */
    //        Paint_DrawNum(200, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 40, "DY", &Font16, BLACK, WHITE);
    //        break;
    //    case DATE_OF_MONTH_MODE:
    //        Paint_DrawNum(200, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 40, "DT", &Font16, BLACK, WHITE);
    //        break;
    //    case NOT_USED_MODE:
    //        break;
    //    default:
    //        break;
    //    }
    //    //parameter 4
    //    Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
    //    drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);
    //    switch (alarm_view_mode->sParam_data_alarm_setup_mode_4->dy_dt)
    //    {
    //    case DAY_OF_WEEK_MODE:
    //        /* code */
    //        Paint_DrawNum(200, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 60, "DY", &Font16, BLACK, WHITE);
    //        break;
    //    case DATE_OF_MONTH_MODE:
    //        Paint_DrawNum(200, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 60, "DT", &Font16, BLACK, WHITE);
    //        break;
    //    case NOT_USED_MODE:
    //        break;
    //    default:
    //        break;
    //    }
       //parameter 5
       Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
       drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);
    //    switch (alarm_view_mode->sParam_data_alarm_setup_mode_5->dy_dt)
    //    {
    //    case DAY_OF_WEEK_MODE:
    //        /* code */
    //        Paint_DrawNum(200, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 80, "DY", &Font16, BLACK, WHITE);
    //        break;
    //    case DATE_OF_MONTH_MODE:
    //        Paint_DrawNum(200, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom/10, &Font16, BLACK, WHITE);
    //        Paint_DrawNum(210, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5->dow_dom%10, &Font16, BLACK, WHITE);
    //        Paint_DrawString_EN(230, 80, "DY", &Font16, BLACK, WHITE);
    //        break;
    //    case NOT_USED_MODE:
    //        break;
    //    default:
    //        break;
    //    }

                break;
        
                case 1:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font16);
               
                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);
                //parameter 3
                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);
                //parameter 4
                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);
                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);
                break;
        
                case 2:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font16);
                
                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);

                //parameter 3
                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);

                //parameter 4
                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);

                break;
                case 3:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font16);

                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);
                //parameter 3

                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);

                //parameter 4

                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);

                break;
        
                case 4:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "1.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_1, Font16);

                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "2.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_2, Font16);

                //parameter 3
                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "3.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_3, Font16);

                //parameter 4
                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "4.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_4, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "5.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_5, Font16);
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
            draw_battery(battery_percentage);
            /*
            Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
            Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
        */
            switch (system_state->alarm_view_cursor)
            {
                case 5:
                //parameter 1
                Paint_ClearWindows(10, 0, 255, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6, Font16);

                //parameter 2
                Paint_ClearWindows(10, 20, 255, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7, Font16);

                //parameter 3
                Paint_ClearWindows(10, 40, 255, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8, Font16);

                //parameter 4
                Paint_ClearWindows(10, 60, 255, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 255, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10, Font16);
                
                switch (alarm_view_mode->sParam_data_alarm_setup_mode_6->dy_dt)
                    {
                    case DAY_OF_WEEK_MODE:
                        /* code */
                        Paint_DrawNum(200, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6->dow_dom/10, &Font16, BLACK, WHITE);
                        Paint_DrawNum(210, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6->dow_dom%10, &Font16, BLACK, WHITE);
                        Paint_DrawString_EN(230, 0, "DY", &Font16, BLACK, WHITE);
                        break;
                    case DATE_OF_MONTH_MODE:
                        Paint_DrawNum(200, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6->dow_dom/10, &Font16, BLACK, WHITE);
                        Paint_DrawNum(210, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6->dow_dom%10, &Font16, BLACK, WHITE);
                        Paint_DrawString_EN(230, 0, "DT", &Font16, BLACK, WHITE);
                        break;
                    case NOT_USED_MODE:
                        break;
                    default:
                        break;
                    }

                switch (alarm_view_mode->sParam_data_alarm_setup_mode_7->dy_dt)
                {
                    case DAY_OF_WEEK_MODE:
                        /* code */
                        Paint_DrawNum(200, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7->dow_dom/10, &Font16, BLACK, WHITE);
                        Paint_DrawNum(210, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7->dow_dom%10, &Font16, BLACK, WHITE);
                        Paint_DrawString_EN(230, 20, "DY", &Font16, BLACK, WHITE);
                        break;
                    case DATE_OF_MONTH_MODE:
                        Paint_DrawNum(200, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7->dow_dom/10, &Font16, BLACK, WHITE);
                        Paint_DrawNum(210, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7->dow_dom%10, &Font16, BLACK, WHITE);
                        Paint_DrawString_EN(230, 20, "DT", &Font16, BLACK, WHITE);
                        break;
                    case NOT_USED_MODE:
                        break;
                    default:
                        break;
                    }

        switch (alarm_view_mode->sParam_data_alarm_setup_mode_8->dy_dt)
       {
        case DAY_OF_WEEK_MODE:
           /* code */
           Paint_DrawNum(200, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8->dow_dom/10, &Font16, BLACK, WHITE);
           Paint_DrawNum(210, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8->dow_dom%10, &Font16, BLACK, WHITE);
           Paint_DrawString_EN(230, 40, "DY", &Font16, BLACK, WHITE);
           break;
        case DATE_OF_MONTH_MODE:
           Paint_DrawNum(200, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8->dow_dom/10, &Font16, BLACK, WHITE);
           Paint_DrawNum(210, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8->dow_dom%10, &Font16, BLACK, WHITE);
           Paint_DrawString_EN(230, 40, "DT", &Font16, BLACK, WHITE);
           break;
        case NOT_USED_MODE:
           break;
        default:
           break;
        }
    switch (alarm_view_mode->sParam_data_alarm_setup_mode_9->dy_dt)
       {
       case DAY_OF_WEEK_MODE:
           /* code */
           Paint_DrawNum(200, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9->dow_dom/10, &Font16, BLACK, WHITE);
           Paint_DrawNum(210, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9->dow_dom%10, &Font16, BLACK, WHITE);
           Paint_DrawString_EN(230, 60, "DY", &Font16, BLACK, WHITE);
           break;
       case DATE_OF_MONTH_MODE:
           Paint_DrawNum(200, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9->dow_dom/10, &Font16, BLACK, WHITE);
           Paint_DrawNum(210, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9->dow_dom%10, &Font16, BLACK, WHITE);
           Paint_DrawString_EN(230, 60, "DT", &Font16, BLACK, WHITE);
           break;
       case NOT_USED_MODE:
           break;
       default:
           break;
       }
    switch (alarm_view_mode->sParam_data_alarm_setup_mode_10->dy_dt)
       {
       case DAY_OF_WEEK_MODE:
           /* code */
           Paint_DrawNum(200, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10->dow_dom/10, &Font16, BLACK, WHITE);
           Paint_DrawNum(210, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10->dow_dom%10, &Font16, BLACK, WHITE);
           Paint_DrawString_EN(230, 80, "DY", &Font16, BLACK, WHITE);
           break;
       case DATE_OF_MONTH_MODE:
           Paint_DrawNum(200, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10->dow_dom/10, &Font16, BLACK, WHITE);
           Paint_DrawNum(210, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10->dow_dom%10, &Font16, BLACK, WHITE);
           Paint_DrawString_EN(230, 80, "DT", &Font16, BLACK, WHITE);
           break;
       case NOT_USED_MODE:
           break;
       default:
           break;
       }
                // change to another
                break;
        
                case 6:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6, Font16);

                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7, Font16);

                //parameter 3
                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8, Font16);

                //parameter 4
                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                 drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10, Font16);
               
                break;
        
                case 7:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6, Font16);

                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7, Font16);

                //parameter 3
                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8, Font16);

                //parameter 4
                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10, Font16);

                break;
        
                case 8:
                //parameter 1
                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6, Font16);

                //parameter 2
                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7, Font16);

                //parameter 3
                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8, Font16);

                //parameter 4
                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9, Font16);

                //parameter 5
                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10, Font16);

                break;
        
                case 9:
                //parameter 1

                Paint_ClearWindows(10, 0, 200, Font16.Height, WHITE);
                Paint_DrawString_EN(10, 0, "6.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 0, alarm_view_mode->sParam_data_alarm_setup_mode_6, Font16);

                //parameter 2

                Paint_ClearWindows(10, 20, 200, 20 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 20, "7.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 20, alarm_view_mode->sParam_data_alarm_setup_mode_7, Font16);

                //parameter 3

                Paint_ClearWindows(10, 40, 200, 40 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 40, "8.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 40, alarm_view_mode->sParam_data_alarm_setup_mode_8, Font16);

                //parameter 4

                Paint_ClearWindows(10, 60, 200, 60 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 60, "9.", &Font16, BLACK, WHITE);
                drawn_alarm_view_mode(140, 60, alarm_view_mode->sParam_data_alarm_setup_mode_9, Font16);

                //parameter 5

                Paint_ClearWindows(10, 80, 200, 80 + Font16.Height, WHITE);
                Paint_DrawString_EN(10, 80, "10.", &Font16, WHITE, BLACK);
                drawn_alarm_view_mode(140, 80, alarm_view_mode->sParam_data_alarm_setup_mode_10, Font16);

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

  flag_alarm_view_mode->flag_ALARM_VIEW_MODE = 0;
  EPD_2IN9_V2_Sleep();
  free(BlackImage);
}

void drawn_alarm_view_mode(uint16_t Xstart, uint16_t Ystart, ALARM_DATA_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font)
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
void alarm_setup_mode(FLAG_SYSTEM *flag_alarm_set_up_mode, SYSTEM_STATE *system_state, ALARM_SETUP_DATA *alarm_setup_data, uint16_t battery_percentage)
{
	UBYTE *BlackImage;
    UBYTE num = 3;
	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }
    ALARM_DATA_SETUP_MODE *alarm_setup_data_store = malloc(sizeof(ALARM_DATA_SETUP_MODE));
    alarm_setup_data_store->hour    = alarm_setup_data->hour;
    alarm_setup_data_store->minute  = alarm_setup_data->minute;
    alarm_setup_data_store->on_off  = alarm_setup_data->on_off;

    
	EPD_2IN9_V2_Init();
	if(flag_alarm_set_up_mode->flag_ALARM_SETUP_MODE == 1)
	{
        flag_alarm_set_up_mode->flag_ALARM_VIEW_MODE    = 1;
        flag_alarm_set_up_mode->flag_DEFAULT_MODE       = 1;
        flag_alarm_set_up_mode->flag_SYSTEM_SETUP_MODE  = 1;
        flag_alarm_set_up_mode->flag_TIME_SETUP_MODE    = 1;

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
           
            drawn_alarm_setup_mode(90, 100, alarm_setup_data_store, Font24);
    //     switch (alarm_setup_data->dy_dt)
    //     {
    //         case DAY_OF_WEEK_MODE:
    //         /* code */
    //         Paint_DrawNum(90, 30, alarm_setup_data->dow_dom/10, &Font16, BLACK, WHITE);
    //         Paint_DrawNum(100, 30, alarm_setup_data->dow_dom%10, &Font16, BLACK, WHITE);
    //         Paint_DrawString_EN(120, 30, "DY", &Font16, BLACK, WHITE);
    //         break;
    //     case DATE_OF_MONTH_MODE:
    //         Paint_DrawNum(90, 30, alarm_setup_data->dow_dom/10, &Font16, BLACK, WHITE);
    //         Paint_DrawNum(100, 30, alarm_setup_data->dow_dom%10, &Font16, BLACK, WHITE);
    //         Paint_DrawString_EN(120, 30, "DT", &Font16, BLACK, WHITE);
    //         break;
    //     case NOT_USED_MODE:
    //         break;
    //     default:
    //         break;
    // }
            EPD_2IN9_V2_Display_Base(BlackImage);

        }


	    

	
        // Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        // Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        // num = num - 1;
        // if(num == 0) {
        // }
		// EPD_2IN9_V2_Display_Partial(BlackImage);
        switch (system_state->alarm_setup_cursor)
        {
            case ALARM_MINUTE:
               /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, alarm_setup_data_store, Font24);
                for(;;)
                {
                    draw_battery(battery_percentage);
                    Paint_ClearWindows(100 + Font24.Width * 2 + Font24.Width / 2, 50, 100 + Font24.Width * 4 +  Font24.Width / 4, 50 + Font24.Height, WHITE);
                    Paint_DrawNum(100 + Font24.Width * 2 + Font24.Width / 2, 50, alarm_setup_data->minute / 10, &Font24, WHITE, BLACK);
                    Paint_DrawNum(100 + Font24.Width * 3 + Font24.Width / 2, 50, alarm_setup_data->minute % 10, &Font24, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            //set hour
            case ALARM_HOUR:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                drawn_alarm_setup_mode(90, 100, alarm_setup_data_store, Font24);
                for(;;)
                {
                    draw_battery(battery_percentage);
                    Paint_ClearWindows(100, 50, 100 + Font24.Width * 2, 50 + Font24.Height, WHITE);
                    Paint_DrawNum(100, 50, alarm_setup_data->hour / 10, &Font24, WHITE, BLACK);
                    Paint_DrawNum(100 + Font24.Width * 1, 50, alarm_setup_data->hour % 10, &Font24, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
		            EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            //set on
            case ALARM_ON_OFF:
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                for(;;)
                {
                    draw_battery(battery_percentage);
                    Paint_ClearWindows(90, 100, 90 + 100, 100 + Font16.Height, WHITE);
                    drawn_alarm_setup_mode(90, 100, alarm_setup_data_store, Font24);
                    
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;
            // //DY - DT
            // case ALARM_DY_DT:
            // Paint_ClearWindows(120, 30, 160, 30 + Font16.Height, WHITE);
            // for(;;)
            // {
                
            //     draw_battery(battery_percentage);
            //     Paint_ClearWindows(120, 30, 160, 30 + Font16.Height, WHITE);
            //     switch (alarm_setup_data->dy_dt)
            //     {
            //         case DAY_OF_WEEK_MODE:
            //         /* code */
            //         Paint_DrawString_EN(120, 30, "DY", &Font16, WHITE, BLACK);
            //         break;
            //     case DATE_OF_MONTH_MODE:
            //         Paint_DrawString_EN(120, 30, "DT", &Font16, BLACK, WHITE);
            //         break;
            //     case NOT_USED_MODE:
            //         Paint_DrawString_EN(120, 30, "NA", &Font16, BLACK, WHITE);
            //         break;
            //     default:
            //         break;
            // }
            //     num = num - 1;
            //     if(num == 0) {
            //         num = 3;
            //         break;
            //     }
            //     EPD_2IN9_V2_Display_Partial(BlackImage);
            // }
            // break;
            // //DOW_DOM
            // case ALARM_DOW_DOM:
            // Paint_ClearWindows(90, 30, 105, 30 + Font16.Height, WHITE);
            // for(;;)
            // {
            //     draw_battery(battery_percentage);
            //     Paint_ClearWindows(90, 30, 105, 30 + Font16.Height, WHITE);
            //     Paint_DrawNum(90, 30, alarm_setup_data->dow_dom/10, &Font16, WHITE, BLACK);
            //     Paint_DrawNum(100, 30, alarm_setup_data->dow_dom%10, &Font16, WHITE, BLACK);
            //     num = num - 1;
            //     if(num == 0) {
            //         num = 3;
            //         break;
            //     }
            //     EPD_2IN9_V2_Display_Partial(BlackImage);
            // }
            // break;
        default:
            break;
        }
      EPD_2IN9_V2_Sleep();
      flag_alarm_set_up_mode->flag_ALARM_SETUP_MODE = 0;
      free(BlackImage);
      free(alarm_setup_data_store);
    //   free(sParam_data_alarm_setup_mode);
}

void drawn_alarm_setup_mode(uint16_t Xstart, uint16_t Ystart, ALARM_DATA_SETUP_MODE *sParam_data_alarm_setup_mode, sFONT Font)
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
void time_setup_mode(FLAG_SYSTEM *flag_time_set_up_mode, SYSTEM_STATE *system_param, TIME_SETUP_DATA *time_setup_param, uint16_t battery_percentage)
{
    UBYTE *BlackImage;
    UBYTE num = 3;
	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }


    
	EPD_2IN9_V2_Init();
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = time_setup_param->hour;
    sPaint_time.Min = time_setup_param->minute;
    sPaint_time.Sec = 0;
	if(flag_time_set_up_mode->flag_TIME_SETUP_MODE == 1)
	{
        flag_time_set_up_mode->flag_ALARM_SETUP_MODE    = 1;
        flag_time_set_up_mode->flag_ALARM_VIEW_MODE     = 1;
        flag_time_set_up_mode->flag_SYSTEM_SETUP_MODE   = 1;
        flag_time_set_up_mode->flag_DEFAULT_MODE        = 1;

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
        Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
        Paint_DrawTime(100, 50, &sPaint_time, &Font24, WHITE, BLACK);
        draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
        draw_battery(battery_percentage);
        // drawn_alarm_setup_mode(90, 100, time_setup_param, Font24);
    	EPD_2IN9_V2_Display_Base(BlackImage);
	}    

	
        // Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        // Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        // num = num - 1;
        // if(num == 0) {
        // }
		// EPD_2IN9_V2_Display_Partial(BlackImage);
        char day_string[5];
    char month_string[5];

    switch (time_setup_param->dow)
    {
    case 1:
        day_string[0] = 'S';
        day_string[1] = 'U';
        day_string[2] = 'N';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;
    case 2:
        day_string[0] = 'M';
        day_string[1] = 'O';
        day_string[2] = 'N';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;

    case 3:
        day_string[0] = 'T';
        day_string[1] = 'U';
        day_string[2] = 'E';
        day_string[3] = 'S';
        day_string[4] = NULL;
        break;

    case 4:
        day_string[0] = 'W';
        day_string[1] = 'E';
        day_string[2] = 'D';
        day_string[3] = 'N';
        day_string[4] = NULL;
        break;

    case 5:
        day_string[0] = 'T';
        day_string[1] = 'H';
        day_string[2] = 'U';
        day_string[3] = 'R';
        break;

    case 6:
        day_string[0] = 'F';
        day_string[1] = 'R';
        day_string[2] = 'I';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;

    case 7:
        day_string[0] = 'S';
        day_string[1] = 'A';
        day_string[2] = 'T';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;

    default:
        break;
    }

switch (time_setup_param->month)
    {
    case 1:
        month_string[0] = 'J';
        month_string[1] = 'A';
        month_string[2] = 'N';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;
    case 2:
        month_string[0] = 'F';
        month_string[1] = 'E';
        month_string[2] = 'B';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 3:
        month_string[0] = 'M';
        month_string[1] = 'A';
        month_string[2] = 'R';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 4:
        month_string[0] = 'A';
        month_string[1] = 'P';
        month_string[2] = 'I';
        month_string[3] = 'R';
        month_string[4] = NULL;
        break;

    case 5:
        month_string[0] = 'M';
        month_string[1] = 'A';
        month_string[2] = 'Y';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 6:
        month_string[0] = 'J';
        month_string[1] = 'U';
        month_string[2] = 'N';
        month_string[3] = 'E';
        month_string[4] = NULL;
        break;

    case 7:
        month_string[0] = 'J';
        month_string[1] = 'U';
        month_string[2] = 'L';
        month_string[3] = 'Y';
        month_string[4] = NULL;
        break;

    case 8:
        month_string[0] = 'A';
        month_string[1] = 'U';
        month_string[2] = 'G';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 9:
        month_string[0] = 'S';
        month_string[1] = 'E';
        month_string[2] = 'P';
        month_string[3] = 'T';
        month_string[4] = NULL;
        break;
    case 10:
        month_string[0] = 'O';
        month_string[1] = 'C';
        month_string[2] = 'T';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 11:
        month_string[0] = 'N';
        month_string[1] = 'O';
        month_string[2] = 'V';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 12:
        month_string[0] = 'D';
        month_string[1] = 'E';
        month_string[2] = 'C';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;
    default:
        break;
    }
    
        //translate day month
        switch (system_param->time_setup_cursor)
        {
            case TIME_MINUTE:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                Paint_DrawTime(100, 50, &sPaint_time,  &Font24, WHITE, BLACK);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
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
            //set hour
            case TIME_HOUR:
                /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                Paint_DrawTime(100, 50, &sPaint_time,  &Font24, WHITE, BLACK);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
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
            case TIME_DOW:
            /* code */
            Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
            Paint_DrawTime(100, 50, &sPaint_time,  &Font24, WHITE, BLACK);
            Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
            draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
            for(;;)
            {
                Paint_ClearWindows(85, 100, 120, 100 + Font16.Height, WHITE);
                Paint_DrawString_EN(80, 100, &day_string[0], &Font16, WHITE, BLACK);
                num = num - 1;
                if(num == 0) {
                    num = 3;
                    break;
                }
                EPD_2IN9_V2_Display_Partial(BlackImage);
            }
            break;
            //set second
            case TIME_MONTH:
            /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                Paint_DrawTime(100, 50, &sPaint_time,  &Font24, WHITE, BLACK);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
                for(;;)
                {
                    Paint_ClearWindows(135, 100, 170, 100 + Font16.Height, WHITE);
                    Paint_DrawString_EN(130, 100, &month_string[0], &Font16, WHITE, BLACK);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;

            case TIME_DOM:
            /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                Paint_DrawTime(100, 50, &sPaint_time,  &Font24, WHITE, BLACK);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
                for(;;)
                {
                    Paint_ClearWindows(180, 100, 200, 100 + Font16.Height, WHITE);
                    Paint_DrawNum(180, 100, time_setup_param->dom / 10, &Font16, BLACK, WHITE);
                    Paint_DrawNum(190, 100, time_setup_param->dom % 10, &Font16, BLACK, WHITE);
                    num = num - 1;
                    if(num == 0) {
                        num = 3;
                        break;
                    }
                    EPD_2IN9_V2_Display_Partial(BlackImage);
                }
            break;

            case TIME_YEAR:
            /* code */
                Paint_ClearWindows(100, 50, 100 + Font24.Width * 7, 50 + Font24.Height, WHITE);
                Paint_DrawTime(100, 50, &sPaint_time,  &Font24, WHITE, BLACK);
                Paint_ClearWindows(80, 100, 220, 100 + Font16.Height, WHITE);
                draw_day(time_setup_param->dow, time_setup_param->dom, time_setup_param->month, time_setup_param->year);
                for(;;)
                {
                    Paint_ClearWindows(210, 100, 230, 100 + Font16.Height, WHITE);
                    Paint_DrawNum(210, 100, time_setup_param->year / 10, &Font16, BLACK, WHITE);
                    Paint_DrawNum(220, 100, time_setup_param->year % 10, &Font16, BLACK, WHITE);
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
      flag_time_set_up_mode->flag_TIME_SETUP_MODE  = 0;
      free(BlackImage);
}

void draw_day(uint16_t day, uint16_t date, uint16_t month, uint16_t year)
{
    char day_string[5];
    char month_string[5];

    switch (day)
    {
    case 1:
        day_string[0] = 'S';
        day_string[1] = 'U';
        day_string[2] = 'N';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;
    case 2:
        day_string[0] = 'M';
        day_string[1] = 'O';
        day_string[2] = 'N';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;

    case 3:
        day_string[0] = 'T';
        day_string[1] = 'U';
        day_string[2] = 'E';
        day_string[3] = 'S';
        day_string[4] = NULL;
        break;

    case 4:
        day_string[0] = 'W';
        day_string[1] = 'E';
        day_string[2] = 'D';
        day_string[3] = 'N';
        day_string[4] = NULL;
        break;

    case 5:
        day_string[0] = 'T';
        day_string[1] = 'H';
        day_string[2] = 'U';
        day_string[3] = 'R';
        break;

    case 6:
        day_string[0] = 'F';
        day_string[1] = 'R';
        day_string[2] = 'I';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;

    case 7:
        day_string[0] = 'S';
        day_string[1] = 'A';
        day_string[2] = 'T';
        day_string[3] = NULL;
        day_string[4] = NULL;
        break;

    default:
        break;
    }

switch (month)
    {
    case 1:
        month_string[0] = 'J';
        month_string[1] = 'A';
        month_string[2] = 'N';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;
    case 2:
        month_string[0] = 'F';
        month_string[1] = 'E';
        month_string[2] = 'B';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 3:
        month_string[0] = 'M';
        month_string[1] = 'A';
        month_string[2] = 'R';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 4:
        month_string[0] = 'A';
        month_string[1] = 'P';
        month_string[2] = 'I';
        month_string[3] = 'R';
        month_string[4] = NULL;
        break;

    case 5:
        month_string[0] = 'M';
        month_string[1] = 'A';
        month_string[2] = 'Y';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 6:
        month_string[0] = 'J';
        month_string[1] = 'U';
        month_string[2] = 'N';
        month_string[3] = 'E';
        month_string[4] = NULL;
        break;

    case 7:
        month_string[0] = 'J';
        month_string[1] = 'U';
        month_string[2] = 'L';
        month_string[3] = 'Y';
        month_string[4] = NULL;
        break;

    case 8:
        month_string[0] = 'A';
        month_string[1] = 'U';
        month_string[2] = 'G';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 9:
        month_string[0] = 'S';
        month_string[1] = 'E';
        month_string[2] = 'P';
        month_string[3] = 'T';
        month_string[4] = NULL;
        break;
    case 10:
        month_string[0] = 'O';
        month_string[1] = 'C';
        month_string[2] = 'T';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 11:
        month_string[0] = 'N';
        month_string[1] = 'O';
        month_string[2] = 'V';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;

    case 12:
        month_string[0] = 'D';
        month_string[1] = 'E';
        month_string[2] = 'C';
        month_string[3] = NULL;
        month_string[4] = NULL;
        break;
    default:
        break;
    }




    Paint_DrawString_EN(80, 100, &day_string[0], &Font16, BLACK, WHITE);
    Paint_DrawString_EN(130, 100, &month_string[0], &Font16, BLACK, WHITE);
    Paint_DrawNum(180, 100, date / 10, &Font16, WHITE, BLACK);
    Paint_DrawNum(190, 100, date % 10, &Font16, WHITE, BLACK);
    Paint_DrawNum(210, 100, year / 10, &Font16, WHITE, BLACK);
    Paint_DrawNum(220, 100, year % 10, &Font16, WHITE, BLACK);

}

void draw_battery(uint16_t battery_percentage)
{
    Paint_ClearWindows(260, 0, 296, 15 + Font16.Height, WHITE);
    if((battery_percentage < 100) && (battery_percentage > 75))
    {
        Paint_DrawRectangle(260, 2, 265, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(265, 2, 270, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(270, 2, 275, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(275, 2, 280, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }
    else if ((battery_percentage < 75) && (battery_percentage > 50)){
        Paint_DrawRectangle(260, 2, 265, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(265, 2, 270, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(270, 2, 275, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(275, 2, 280, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }

    else if ((battery_percentage < 50) && (battery_percentage > 25)){
        Paint_DrawRectangle(260, 2, 265, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(265, 2, 270, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(270, 2, 275, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawRectangle(275, 2, 280, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }

    else if ((battery_percentage < 25) && (battery_percentage > 5)){
        Paint_DrawRectangle(260, 2, 265, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(265, 2, 270, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(270, 2, 275, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(275, 2, 280, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }

    else
    {
        Paint_DrawRectangle(260, 2, 265, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(265, 2, 270, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(270, 2, 275, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawRectangle(275, 2, 280, 10, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    }
}
void E_ink_display_name(void)
{
    UBYTE *BlackImage;
    UBYTE num = 3;
	UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }


    
	EPD_2IN9_V2_Init();


    	EPD_2IN9_V2_Clear();
    	EPD_2IN9_V2_Init_Fast();
    	Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
    	printf("Drawing\r\n");
    	//1.Select Image
    	Paint_SelectImage(BlackImage);
    	Paint_Clear(WHITE);

    	Paint_DrawString_EN(10, 0, "Duy Ngoc - 2251036", &Font16, BLACK, WHITE);
    	Paint_DrawString_EN(10, 30, "Trung Nam - 2251032", &Font16, BLACK, WHITE);
    	Paint_DrawString_EN(10, 60, "Hung Minh - 2251030", &Font16, BLACK, WHITE);
    	EPD_2IN9_V2_Display_Base(BlackImage);
        free(BlackImage);
	}    
