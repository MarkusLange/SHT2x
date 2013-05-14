/*
SHT2x - Library for the SHT2x series of Sensirion sensors.

Copyright 2012 Eduard Iten

Supported devices:
SHT20*
SHT21
SHT25

*The SHT20 has not been tested so far, but should work according
the Sensirion datasheet. If anyone can confirm this, please report.

This library is free software, it is released under the GNU GPL v3.
Please find the terms and conditions in the attached gpl.txt or
in the world wide web: <http://www.gnu.org/licenses/>

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
or check in the web: <http://www.gnu.org/licenses/>
*/

#include "SHT2x.h"

uint32_t timeout=0;

void SHT2xClass::softReset()
{
  Wire.beginTransmission(SHT2xADDR);
  Wire.write(SOFT_RESET);
  delay(15);
}

uint8_t SHT2xClass::readUserRegister()
{
  Wire.beginTransmission(SHT2xADDR);
  Wire.write(USER_REG_R);
  Wire.endTransmission();
  Wire.requestFrom(SHT2xADDR,(uint8_t)1);
  while(Wire.available()<1) {
  	;
  }
  return Wire.read();
}

void SHT2xClass::writeUserRegister(uint8_t userRegister)
{
  Wire.beginTransmission(SHT2xADDR);
  Wire.write(USER_REG_W);
  Wire.write(userRegister);
  Wire.endTransmission();
}

void SHT2xClass::setResolution(uint8_t resolution)
{
	uint8_t userRegister;
  userRegister=readUserRegister();
	writeUserRegister( (userRegister & ~SHT2x_RES_MASK) | resolution);
}

uint8_t SHT2xClass::getResolution()
{
	uint8_t userRegister;
  userRegister=readUserRegister();
	return (userRegister & SHT2x_RES_MASK);
}

void SHT2xClass::readSerial(uint8_t *serial)
{
	Wire.beginTransmission(SHT2xADDR);
	Wire.write(0xFA);
	Wire.write(0x0F);
	Wire.endTransmission();
	
	Wire.requestFrom(SHT2xADDR,(uint8_t)8);
	serial[2]=Wire.read();
	Wire.read();
	serial[3]=Wire.read();
	Wire.read();
	serial[4]=Wire.read();
	Wire.read();
	serial[5]=Wire.read();
	Wire.read();
	
	Wire.beginTransmission(SHT2xADDR);
	Wire.write(0xFC);
	Wire.write(0xC9);
	Wire.endTransmission();
	
	Wire.requestFrom(SHT2xADDR,(uint8_t)6);
	serial[6]=Wire.read();
	serial[7]=Wire.read();
	Wire.read();
	serial[0]=Wire.read();
	serial[1]=Wire.read();
	Wire.read();
}

uint16_t SHT2xClass::readMeasurement(SHT2xMeasureType type)
{
  uint16_t value = 0;
  uint8_t data[3];

  Wire.beginTransmission(SHT2xADDR);
  switch (type) {
  case HUMIDITY:
  	Wire.write(MEASUREMENT_RH_HM);
    break;
  case TEMP:
    Wire.write(MEASUREMENT_T_HM);
    break;
  }
  Wire.endTransmission();

  Wire.requestFrom(SHT2xADDR,(uint8_t)3);
  while (Wire.available()<3) {
  	;
  }
  data[0]=Wire.read();
  data[1]=Wire.read();
  data[2]=Wire.read();
  value=(uint16_t)data[0] << 8 | data[1];
  value &= ~0x0003;
  if (checkcrc(data) != 0)
  	return 0;
  else
  	return value;
}

uint8_t SHT2xClass::checkcrc(uint8_t *data)
{
	uint8_t crc, i;
	crc = 0;
	
	for (int i = 0; i < 2; i++) {
		crc ^= data[i];
		for (int bit = 8; bit > 0; bit--) {
			if (crc & 0x80) {
				crc = (crc << 1)^POLYNOMIAL;
			} else {
				crc = (crc << 1);
			}
		}
	}
	if (crc != data[2])
		return 1;
	else
		return 0;
}

float SHT2xClass::readT()
{
  return -46.85+175.72/65536.00*(float)readMeasurement(TEMP);
}

float SHT2xClass::readRH()
{
  return -6.0+125.0/65536.00*(float)readMeasurement(HUMIDITY);
}

void SHT2xClass::setHeater(uint8_t on)
{
  uint8_t userRegister;
  userRegister=readUserRegister();
  
  if (on) {
  	userRegister=(userRegister&~SHT2x_HEATER_MASK) | SHT2x_HEATER_ON;
  } else {
    userRegister=(userRegister&~SHT2x_HEATER_MASK) | SHT2x_HEATER_OFF;
  }
}
SHT2xClass SHT2x;