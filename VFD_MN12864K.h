/*
Mario Becker, 2024, License:MIT.

Code is for STM32, others not tested! Critical things used:
- IntervalTimer
*/

#pragma once

#include <Adafruit_GFX.h>
#include <SPI.h>


// /// STM32 bluepill pins...
// // SPI1: SCLK=PA5         //ok disp pin 8 & 13
// //       MOSI=PA7         //ok          9
// const byte pinBLK1 = PB1; //ok disp pin 6 = 1st of the many pins
// const byte pinLAT1 = PB0; //ok          7
// const byte pinGBLK = PA2; //ok ??? what for ???
// // const byte pinGLAT = ---; //ok      14 - connect to LAT1 PB0
// // const byte pinGCLK = ---; //ok      13 - connect to SCLK PA5
// // const byte pinGSIN = ---; //ok      12 - connect to SOUT pin 10

// const byte pinPWM = 20; // generates clock for filament driver


template <size_t BitDepth> class MN12864Kgeneric : public Adafruit_GFX
{
public:
    MN12864Kgeneric(
        byte pinBLK = 14,
        byte pinLAT = 16,
        byte pinGBLK = 15,
        byte pinPWM = 17,
        byte pinMOSI = 11,
        byte pinSCLK = 13
        );
    ~MN12864Kgeneric(void);
    void begin();

    uint32_t getDisplayTime(); // 24us
    uint32_t getDisplayFps1(); // 24us

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void fillScreen(uint8_t color);
    void swapBuffers();

    /** must be called from timer or otherwise very regularly ! Go for min 4000 times per sec */
    static void refresh();

    static const int16_t targetFps = 150 * 44; // this is per gate // 

protected:
    static void nextGate();
    static const int16_t bufferSize = 136 / 8 * 64; // black'n'white, + margin

    // bufferlayout
    ///   front | back
    ///   0 | 1 | 0 | 1 //bitplanes

    SPISettings spiSettings;

    uint8_t buffer[bufferSize*2*BitDepth];
    uint16_t bufferOffset = 0;
    uint8_t tempBuffer[48+6]; // one gate data block 384 + 48 bits

    union u32u4
    {
        uint64_t u32;
        uint8_t u4[4];
    };

    union u64u8
    {
        uint64_t u64;
        uint8_t u8[8];
    };

    uint8_t  gate;
    u64u8 gateBuf;

    uint32_t displayTime;
    uint32_t displayLast;
    uint32_t displayFps1;

    uint32_t loadLast;
    uint32_t loadFps1;

    const byte pinBLK;
    const byte pinLAT;
    const byte pinGBLK;
    const byte pinPWM;    
    const byte pinMOSI;
    const byte pinSCLK;
    static MN12864Kgeneric *_the;
};

/**
 * MN12864K in "black'n'white or mono or 1 bit depth.
 * In this case the two serial inputs must be wired in parallel.
 * * The refresh function is faster -> less CPU load.
 * * Needs less memory.
*/
class MN12864K : public MN12864Kgeneric<1>
{
public:
    MN12864K(
        byte pinBLK = 14,
        byte pinLAT = 16,
        byte pinGBLK = 15,
        byte pinPWM = 17,
        byte pinMOSI = 11,
        byte pinSCLK = 13);
};

#include "VFD_MN12864K.inl"
