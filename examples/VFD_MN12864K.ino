#include <Arduino.h>
#include <VFD_MN12864K.h>
#include <mbLog.h>
#include <elapsedMillis.h>

// Create an IntervalTimer object
#ifdef ARDUINO_ARCH_STM32
HardwareTimer timer(TIM3);
#else // teensy ??
IntervalTimer timer;
#endif

MN12864K display;

elapsedMillis checker = 0;
elapsedMillis logger = 0;
uint32_t frameCounter = 0;
uint32_t drawtime = 0;
int16_t textPos = 128;

void setup()
{
    display.begin();
    randomSeed(analogRead(0));

    // You need to trigger the refresh function regularly !
#ifdef ARDUINO_ARCH_STM32
    timer.setOverflow(1000000 / display.targetFps, MICROSEC_FORMAT);
    timer.attachInterrupt([](HardwareTimer *timer) { display.refresh(); });
#else
    timer.begin(display.refresh, 4000000 / display.targetFps); // starting slowly
#endif
}

void loop()
{
    if(checker > 100)
    {
        checker = 0;
        uint32_t time = micros();

// #define DRAWBEBUG 1
#ifdef DRAWBEBUG

        const uint16_t fullCol = 1;
        // writes 128 pix
        // v1 ~ 1975us
        // v2 ~ 1223us
        display.fillScreen(0);
        display.drawLine(0,32,63,63,fullCol);
        display.drawLine(64,0,64+63,63,fullCol);
        
        // display.drawRect(1,1,4,4,1);
        // display.drawRect(7,7,4,4,1);
        // LOG <<"-----------" <<LOG.endl;
#else
        /// draw random EQ
        // v2 ~ 31ms

        display.fillScreen(0);
        display.drawLine(0,10,127,10, 1);
        display.setCursor(textPos,0);
        display.setTextColor(1);
        display.setTextWrap(false);
        display.print("subermajo");
        display.print(MYEMOJI);

        display.drawLine(0,54,127,54, 1);
        display.setCursor(-textPos+40,56);
        display.print("hypercool");

        textPos -= 3;
        if(textPos < -120) textPos = 128;

        for(int16_t x = 0; x < 120; x+=9)
        {
            int16_t hi = random(16);
            // display.fillRect(x,16,7,31,0);
            display.fillRect(x,32 - hi, 7, hi, 1);
            display.fillRect(x,32, 7, hi, 1);
        }
#endif

        display.swapBuffers();

        drawtime = micros() - time;
    }
    if(logger > 1000)
    {
        // display.fillScreen(0);
        logger = 0;
        LOG <<LOG.dec <<"gate time: " <<display.getDisplayTime() <<"us, possible fps:" <<1000000/display.getDisplayTime()/44 <<" current fps:" <<1000000/display.getDisplayFps1()/44 <<" draw:" <<drawtime <<"us" <<LOG.endl;
    }
}

