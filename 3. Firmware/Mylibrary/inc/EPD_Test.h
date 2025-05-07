/*****************************************************************************
* | File      	:	  EPD_Test.h
* | Author      :   Waveshare team
* | Function    :   e-Paper test Demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-11
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
#ifndef _EPD_TEST_H_
#define _EPD_TEST_H_

#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "Debug.h"
#include <stdlib.h> // malloc() free()

int EPD_test(void);
void default_mode(FLAG_SYSTEM *flag_default_mode, TIME_DATA *time_get_data, uint16_t battery_percentage);
void system_setup_mode (FLAG_SYSTEM *flag_set_up,  SYSTEM_STATE *system_state, uint8_t battery_percentage);
void alarm_view_mode (FLAG_SYSTEM *flag_alarm_view_mode, SYSTEM_STATE *system_state, SYSTEM_PARAM_DATA_ALARM_VIEW_MODE *alarm_view_mode, uint16_t battery_percentage);
void alarm_setup_mode(FLAG_SYSTEM *flag_alarm_set_up_mode, SYSTEM_STATE *system_state, ALARM_SETUP_DATA *alarm_setup_data, uint16_t battery_percentage);
void time_setup_mode(FLAG_SYSTEM *flag_time_set_up_mode, SYSTEM_STATE *system_param, TIME_SETUP_DATA *time_setup_param, uint16_t battery_percentage);
#endif
