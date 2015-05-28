#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONEWIRE 7
#define aref_voltage 3.3

#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
 
OneWire ds(10);
LiquidCrystal lcd(7, 11, 5, 4, 3, 2);

int led1 = 13;
int led2 = 12;
float previous;

void setup(void) {
  // put your setup code here, to run once:
  Serial.print("Booting...");
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);  
  lcd.begin(16, 2);
  lcd.print("init...");
  Serial.begin(115200);
}
  
  void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(0);
  float voltage = reading * 5.0;
  voltage /= 1024;
  lcd.setCursor(0, 1);
  //lcd.print(millis()/1000);
  Serial.print(voltage);
  Serial.print(" Volts     ");
  
  float temperatureC = (voltage - 0.5) * 100;
  Serial.print(temperatureC);
  Serial.print(" *C     ");
  
  float temperatureF = (temperatureC * (9.0/5.0)) + 32;
  Serial.print(temperatureF);
  Serial.print(" *F     ");
  
  float change = temperatureF - previous;
  
  float temp1 = (getTemp() * (9/5)) + 32;
  Serial.print(temp1 + 10);
  Serial.print(" *F (Waterproof)");
  
  lcd.print(temp1+10);
  
  Serial.println("");
  
  if(change > 0)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
  if(change < 0)
  {
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
  }
  if(change == 0)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
  
  previous = temperatureF;
  
  delay(1000);
  //delay(500);
  //digitalWrite(led1, HIGH);
  //delay(500);
  //digitalWrite(led2, HIGH);
  //delay(500);
  //digitalWrite(led1, LOW);
  //delay(500);
  //digitalWrite(led2, LOW);
}  

float getTemp()
{
  byte data[12];
  byte addr[8];
  
  if (!ds.search(addr))
  {
    ds.reset_search();
  }
  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.println("CRC is not valad!");
    return -1000;
  }
  
  if (addr[0] != 0x10 && addr[0] != 0x28)
  {
    Serial.print("Device is not recognized!");
    return -1000;
  }
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  
  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  
  for (int i = 0; i < 9; i++)
  {
    data[i] = ds.read();
    //Serial.println(data[i]);
  }
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];
  
  float TRead = ((MSB << 8) | LSB);
  float Temperature = TRead / 16;
  return Temperature;
}
