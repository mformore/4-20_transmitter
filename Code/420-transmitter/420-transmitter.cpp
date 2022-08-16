
#include "420-transmitter.h"

transmitter::transmitter(unsigned char CH1Mode, unsigned char CH2Mode)
{
  _CH1Mode = CH1Mode;
  _CH2Mode = CH2Mode;

  if(_CH1Mode == DAC_MODE || _CH2Mode == DAC_MODE)
  {
    pinMode(SPI_SDI,OUTPUT); 
    pinMode(SPI_SCK,OUTPUT); 
    pinMode(SPI_CS,OUTPUT); 
    SPI.begin();
  }
}

void transmitter::set(unsigned char channel, value inputVal)
{
  int input, channelA, channelB;
  
  if(channel == CHANNEL_1)
  {
    if(_CH1Mode == DAC_MODE)
    {
      input = map(inputVal.getVal(), inputVal.getMin(), inputVal.getMax(), 0, DAC_RES);
      channelA = input | 0b0111000000000000;

      digitalWrite(SPI_CS, LOW);
      SPI.transfer(highByte(channelA));
      SPI.transfer(lowByte(channelA));
      digitalWrite(SPI_CS, HIGH); 
    }
    else
    {
      input = map(inputVal.getVal(), inputVal.getMin(), inputVal.getMax(), 0, PWM_RES);
      analogWrite(PWM_CH1_PIN, input);
    }
  }
  else
  {
    if(_CH2Mode == DAC_MODE)
    {
      input = map(inputVal.getVal(), inputVal.getMin(), inputVal.getMax(), 0, DAC_RES);
      channelB = input | 0b1111000000000000;

      digitalWrite(SPI_CS, LOW);
      SPI.transfer(highByte(channelB));
      SPI.transfer(lowByte(channelB));
      digitalWrite(SPI_CS, HIGH); 
    }
    else
    {
      input = map(inputVal.getVal(), inputVal.getMin(), inputVal.getMax(), 0, PWM_RES);
      analogWrite(PWM_CH2_PIN, input);
    }
  }
  
}

/*int transmitter::mapValue(int inVal, int minVal, int maxVal)
{
  return map(inVal, minVal, maxVal, 0, 4095);
}*/

value::value(int minVal, int maxVal)
{
  _maxValue = maxVal;
  _minValue = minVal;
  _value = 0;
}

int value::getMin()
{
  return _minValue;
}

int value::getMax()
{
  return _maxValue;
}

int value::getVal()
{
  return _value;
}

void value::updateVal(int newVal)
{
  if(newVal <= _maxValue && newVal >= _minValue)
  {
    _value = newVal;
  }
}
