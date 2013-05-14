#include <Wire.h>
//#include <i2c_t3.h> 	//fast Teensy 3 I2C Library http://forum.pjrc.com/threads/21680-New-I2C-library-for-Teensy3
#include <SHT2x.h>

uint8_t number[8];

void setup() {
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
  SHT2x.softReset();
  Serial.begin(9600);
  while (!Serial);
  //printAddress(number);
  //Serial.println(SHT2x.readUserRegister(), BIN);
//  Serial.print("Serial: ");
//  SHT2x.readSerial(number);
//  printAddress(number);
  SHT2x.setResolution(SHT2x_RES_8_12BIT);
}

void loop() {
  //Serial.println(SHT2x.readUserRegister(), BIN);
  Serial.print("Temperature: ");
  Serial.print(SHT2x.readT());
     // On the following line, \260 is the octal code for the degree sign. If
     // you would just write °, this would not work, as the Arduino IDE would
     // save it as an UTF8 character, resulting in two characters in the serial
     // transmission.
  Serial.print("\260C, Humidity: ");
  Serial.print(SHT2x.readRH());
  Serial.println(" %RH");
  delay(1000);
  Serial.print("Serial: ");
  SHT2x.readSerial(number);
  printAddress(number);
  
  //Serial.println(SHT2x.getResolution());
  
  presentresolution();
}

void printAddress(uint8_t *number)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (number[i] < 16)
      Serial.print("0");
    Serial.print(number[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void presentresolution(){
  switch(SHT2x.getResolution()) {
    case SHT2x_RES_12_14BIT:
      Serial.println("SHT2x_RES_12_14BIT");
      break;
    case SHT2x_RES_8_12BIT:
      Serial.println("SHT2x_RES_8_12BIT");
      break;
    case SHT2x_RES_10_13BIT:
      Serial.println("SHT2x_RES_10_13BIT");
      break;
    case SHT2x_RES_11_11BIT:
      Serial.println("SHT2x_RES_11_11BIT");
      break;
  }
}
