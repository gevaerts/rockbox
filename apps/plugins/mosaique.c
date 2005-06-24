/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Itai Shaked 
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 **************************************************************************/
#include "plugin.h"
#include "playergfx.h"

#ifdef HAVE_LCD_BITMAP
#define LARGE ((LCD_WIDTH - 2) / 2)
#define HAUT  ((LCD_HEIGHT - 2) / 2)
#else
#define LARGE 9
#define HAUT  6
#endif

/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#define MOSAIQUE_QUIT BUTTON_OFF
#define MOSAIQUE_SPEED BUTTON_F1
#define MOSAIQUE_RESTART BUTTON_PLAY

#elif CONFIG_KEYPAD == PLAYER_PAD
#define MOSAIQUE_QUIT BUTTON_STOP
#define MOSAIQUE_SPEED BUTTON_MENU
#define MOSAIQUE_RESTART BUTTON_PLAY

#elif CONFIG_KEYPAD == ONDIO_PAD
#define MOSAIQUE_QUIT BUTTON_OFF
#define MOSAIQUE_SPEED BUTTON_MENU
#define MOSAIQUE_RESTART BUTTON_RIGHT

#elif CONFIG_KEYPAD == IRIVER_H100_PAD
#define MOSAIQUE_QUIT BUTTON_OFF
#define MOSAIQUE_SPEED BUTTON_MODE
#define MOSAIQUE_RESTART BUTTON_ON
#endif

enum plugin_status plugin_start(struct plugin_api* api, void* parameter)
{
    int button;
    int timer = 10;
    int x=0;
    int y=0;
    int sx = 3;
    int sy = 3;
    struct plugin_api* rb = api;
    TEST_PLUGIN_API(api);
    (void)parameter;

#ifdef HAVE_LCD_BITMAP
    rb->lcd_clear_display();
    rb->lcd_set_drawmode(DRMODE_COMPLEMENT);
#else
    if (!pgfx_init(rb, 4, 2))
    {
        rb->splash(HZ*2, true, "Old LCD :(");
        return PLUGIN_OK;
    }
    pgfx_display(3, 0);
    pgfx_clear_display();
#endif
    while (1) {

        x+=sx;
        if (x>LARGE) 
        {
            x = 2*LARGE-x;
            sx=-sx;
        }
	
        if (x<0) 
        {
            x = -x;
            sx = -sx;
        }
	
        y+=sy;
        if (y>HAUT) 
        {
            y = 2*HAUT-y;
            sy=-sy;
        }
	
        if (y<0) 
        {
            y = -y;
            sy = -sy;
        }

#ifdef HAVE_LCD_BITMAP
        rb->lcd_fillrect(LARGE-x, HAUT-y, 2*x+1, 1);
        rb->lcd_fillrect(LARGE-x, HAUT+y, 2*x+1, 1);
        rb->lcd_fillrect(LARGE-x, HAUT-y+1, 1, 2*y-1);
        rb->lcd_fillrect(LARGE+x, HAUT-y+1, 1, 2*y-1);
        rb->lcd_update();
#else
        pgfx_invertrect(LARGE-x, HAUT-y, 2*x+1, 1);
        pgfx_invertrect(LARGE-x, HAUT+y, 2*x+1, 1);
        pgfx_invertrect(LARGE-x, HAUT-y+1, 1, 2*y-1);
        pgfx_invertrect(LARGE+x, HAUT-y+1, 1, 2*y-1);
        pgfx_update();
#endif

        rb->sleep(HZ/timer);
        
        button = rb->button_get(false);
        switch (button)
        {
            case MOSAIQUE_QUIT:
#ifdef HAVE_LCD_BITMAP
                rb->lcd_set_drawmode(DRMODE_SOLID);
#else
                pgfx_release();
#endif
                return PLUGIN_OK;

            case MOSAIQUE_SPEED:
                timer = timer+5;
                if (timer>20)
                    timer=5;
                break;
                
            case MOSAIQUE_RESTART:

                sx = rb->rand() % (HAUT/2) + 1;
                sy = rb->rand() % (HAUT/2) + 1;
                x=0;
                y=0;
#ifdef HAVE_LCD_BITMAP
                rb->lcd_clear_display();
#else
                pgfx_clear_display();
#endif
                break;


            default:
                if (rb->default_event_handler(button) == SYS_USB_CONNECTED)
                {
#ifdef HAVE_LCD_BITMAP
                    rb->lcd_set_drawmode(DRMODE_SOLID);
#else
                    pgfx_release();
#endif
                    return PLUGIN_USB_CONNECTED;
                }
                break;
        }
    }
}

