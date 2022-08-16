//#ifndef 420-transmitter_h
//#define 420-transmitter_h

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <SPI.h>

#define SPI_CS 8
#define SPI_SDI 11
#define SPI_SCK 13

#define CHANNEL_1 0
#define CHANNEL_2 1

#define DAC_MODE 1
#define PWM_MODE 0

#define PWM_CH1_PIN 9
#define PWM_CH2_PIN 10

#define PWM_RES 255
#define DAC_RES 4095

class value
{
  public:
    value(int minVal, int maxVal);
    int getMin();
    int getMax();
    int getVal();
    void updateVal(int newVal);
    
    
  private:
    int _maxValue;
    int _minValue;
    int _value;
};

class transmitter
{
  public:
    transmitter(unsigned char CH1Mode, unsigned char CH2Mode);
    void set(unsigned char channel, value inputVal);

  private:
    unsigned char _CH1Mode;
    unsigned char _CH2Mode;
};

//#endif
