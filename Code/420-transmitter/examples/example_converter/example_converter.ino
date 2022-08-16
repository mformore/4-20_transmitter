#include <LiquidCrystal_I2C.h>
#include <420-transmitter.h>
#include <DHT.h>
#include <Wire.h> 

#define DHT11_PIN 5
#define HCSR4_TRIG_PIN 7
#define HCSR4_ECHO_PIN 6
#define BTN_PIN 4
#define POT_PIN A0
#define MENU_ITEMS 3
#define TIM_SEC 50

bool btnStateFlag = 0;
long timer = 0;
bool pressed = false;
unsigned char menu = 0;
unsigned long int DHT_prev_time = 0;
int DHT_read = 0;

DHT dht;
LiquidCrystal_I2C lcd(0x38, 16, 2);
transmitter transmitter(DAC_MODE, DAC_MODE);
value temperature(-20, 60);
value distance(2, 200);
value potentiometer(0, 1023);

void setup() 
{
  DHT11_init();
  HCSR4_init();
  LCD_init();
  BTN_init();
}

void loop() 
{
  potentiometer.updateVal(Pot_read());
  temperature.updateVal(DHT11_read());
  distance.updateVal(HCSR4_read());
  
  BTN_StateMonitor();

  transmitter.set(CHANNEL_1, temperature);

  switch (menu)
  {
    case 0:
    LCD_writeTemp(temperature.getVal());
    break;

    case 1:
    LCD_writeDist(distance.getVal());
    transmitter.set(CHANNEL_2, distance);
    break;

    case 2:
    LCD_writePot(potentiometer.getVal());
    transmitter.set(CHANNEL_2, potentiometer);
    break;

    default:
    break;
  }
}

void BTN_StateMonitor()
{

  if(!(digitalRead(BTN_PIN)) && !(pressed) ) 
  {
    pressed = 1;
  }
  else if(digitalRead(BTN_PIN) && pressed) 
  {
    if(menu >= MENU_ITEMS-1)
    {
      menu = 0;
    }
    else
    {
      menu++; 
    }
    pressed = 0;
  }
}

void BTN_init()
{
  pinMode(BTN_PIN,INPUT_PULLUP);
}

void DHT11_init()
{
  dht.setup(DHT11_PIN);
}

void HCSR4_init()
{
  pinMode(HCSR4_TRIG_PIN, OUTPUT); 
  pinMode(HCSR4_ECHO_PIN, INPUT); 
}

void LCD_init()
{
  lcd.begin();   
  lcd.backlight(); 
  lcd.setCursor(0,0);
}

void LCD_writeTemp(int val)
{
  int amp = map(val, 0, 60, 40, 200);
  int volt = map(val, 0, 60, 0, 100);
  
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(val);
  lcd.print("st. C   ");

  lcd.setCursor(0,1); 
  lcd.print(amp/10);
  lcd.print(".");
  lcd.print(amp - amp/10*10);
  lcd.print("mA ");
 
  lcd.print(volt/10);
  lcd.print(".");
  lcd.print(volt - volt/10*10);
  lcd.print("V    "); 
}

void LCD_writeDist(int val)
{
  int amp = map(val, 2, 200, 40, 200);
  int volt = map(val, 2, 200, 0, 100);
  
  lcd.setCursor(0,0); 
  lcd.print("Dist: ");
  lcd.print(val);
  lcd.print("cm      ");

  lcd.setCursor(0,1); 
  lcd.print(amp/10);
  lcd.print(".");
  lcd.print(amp - amp/10*10);
  lcd.print("mA ");
 
  lcd.print(volt/10);
  lcd.print(".");
  lcd.print(volt - volt/10*10);
  lcd.print("V    "); 
}

void LCD_writePot(int val)
{
  int amp = map(val, 0, 1023, 40, 200);
  int volt = map(val, 0, 1023, 0, 100);
  int pot = map(val, 0, 1023, 0, 50);
  
  lcd.setCursor(0,0); 
  lcd.print("Pot: ");
  lcd.print(pot/10);
  lcd.print(".");
  lcd.print(pot - pot/10*10);
  lcd.print("V       "); 

  lcd.setCursor(0,1); 
  lcd.print(amp/10);
  lcd.print(".");
  lcd.print(amp - amp/10*10);
  lcd.print("mA ");
 
  lcd.print(volt/10);
  lcd.print(".");
  lcd.print(volt - volt/10*10);
  lcd.print("V    "); 
}

int Pot_read()
{
  return analogRead(POT_PIN);
}

int HCSR4_read() 
{
  long period, dist;

  digitalWrite(HCSR4_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(HCSR4_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCSR4_TRIG_PIN, LOW);

  period = pulseIn(HCSR4_ECHO_PIN, HIGH);
  dist = period / 58;
  if(dist > 200)  dist = 200;
  
  return dist;
}

int DHT11_read()
{   
  if(millis() - DHT_prev_time >= 1500 && millis() - DHT_prev_time < 3000)  
  {
    DHT_prev_time = millis();
    DHT_read = dht.getTemperature();
  }
  return DHT_read;
}
