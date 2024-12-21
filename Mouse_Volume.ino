#include <M5Dial.h>

#define ENABLE_VOLUME_CONTROL 1
#define ENABLE_MOUSE_CONTROL 1

#ifdef ENABLE_VOLUME_CONTROL
#include "USB.h"
#include "USBHIDConsumerControl.h"
#endif

#ifdef ENABLE_MOUSE_CONTROL
#include "USBHIDMouse.h"
#endif

#if ENABLE_VOLUME_CONTROL
USBHIDConsumerControl ConsumerControl;
#endif

#if ENABLE_MOUSE_CONTROL
USBHIDMouse Mouse;
#endif

long oldPosition = 0;
int prev_x = -1;
int prev_y = -1;
static m5::touch_state_t prev_state;
bool touched = false;

void setup()
{
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);

#if ENABLE_VOLUME_CONTROL
    ConsumerControl.begin();
    USB.begin();
#endif

#if ENABLE_MOUSE_CONTROL
    Mouse.begin();
#endif

    delay(500); 
    oldPosition = M5Dial.Encoder.read(); 
void loop()
{
    M5Dial.update();

    
    long newPosition = M5Dial.Encoder.read();
    if (newPosition != oldPosition)
    {
        int16_t dw = newPosition - oldPosition;
        oldPosition = newPosition;

#if ENABLE_VOLUME_CONTROL
       
        for (int i = 0; i < abs(dw); i++)
        {
            if (dw > 0)
            {
                ConsumerControl.press(HID_USAGE_CONSUMER_VOLUME_INCREMENT);
            }
            else if (dw < 0)
            {
                ConsumerControl.press(HID_USAGE_CONSUMER_VOLUME_DECREMENT);
            }
            delay(5);
            ConsumerControl.release();
        }
#endif
    }

    if (M5Dial.BtnA.wasPressed())
    {
#if ENABLE_VOLUME_CONTROL
        ConsumerControl.press(HID_USAGE_CONSUMER_MUTE);
        delay(50);
        ConsumerControl.release();
#endif
    }

   
    auto t = M5Dial.Touch.getDetail();
    if (prev_state != t.state)
    {
        prev_state = t.state;
        if (t.state == m5::touch_state_t::touch)
        {
            touched = true;
            prev_x = t.x;
            prev_y = t.y;
        }
        if (t.state == m5::touch_state_t::touch_end)
        {
#if ENABLE_MOUSE_CONTROL
            Mouse.click(MOUSE_LEFT);
#endif
        }
        if (t.state == m5::touch_state_t::none)
        {
            touched = false;
        }
    }

if (touched && (prev_x != t.x || prev_y != t.y))
{
    int16_t dy = (( (int16_t)t.y) - prev_y); 
    int16_t dx = (( (int16_t)t.x) - prev_x);

    dx = -dx; 
    dy = -dy; 

#if ENABLE_MOUSE_CONTROL
    Mouse.move(dx * 2, dy * 2, 0, 0); 
#endif

    prev_x = t.x;
    prev_y = t.y;
}

}
