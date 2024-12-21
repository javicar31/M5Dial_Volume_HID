#include <M5Dial.h>

#define ENABLE_VOLUME_CONTROL 1

#ifdef ENABLE_VOLUME_CONTROL
#include "USB.h"
#include "USBHIDConsumerControl.h"
#endif

#if ENABLE_VOLUME_CONTROL
USBHIDConsumerControl ConsumerControl;
#endif

long oldPosition = 0;

void setup()
{
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);

#if ENABLE_VOLUME_CONTROL
    ConsumerControl.begin();
    USB.begin();
#endif

    delay(500); 

    oldPosition = M5Dial.Encoder.read(); 
}

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
}
