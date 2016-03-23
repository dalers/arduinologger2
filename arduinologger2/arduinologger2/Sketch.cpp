// (c) 2016 by Dale Scott
// (c) 2016 by Steve Pye
// Provided according to the 2-clause BSD License.
// https://opensource.org/licenses/BSD-2-Clause

// Gets measurement from I2C TLV493D using Wire library

/*Beginning of Auto generated code by Atmel studio */
#include <Arduino.h>
/*End of auto generated code by Atmel studio */

#include <Wire.h>
#include <SoftwareSerial.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

// TLV_ADDRESS1 0x5E (decimal 94) (ADDR=1)
// TLV_ADDRESS2 0x1F (decimal 31) (ADDR=0)

bool led = false;					// state of Arduino LED ("L", "pin 13")
int sample = 0;						// sample counter

// TLV493 I2C register read values
byte byte0 = 0;						// Bx value MSB's
byte byte1 = 0;						// By value MSB's
byte byte2 = 0;						// Bz value MSB's
byte byte3 = 0;						// Selected channel, frame counter, temperature MSB's
byte byte4 = 0;						// By, Bx LSB's
byte byte5 = 0;						// Bz LSB's, PD flag
byte byte6 = 0;						// Temperature LSB's
byte byte7 = 0;						// Factory settings
byte byte8 = 0;						// Factory settings
byte byte9 = 0;						// Factory settings

// sensor values
int bx = 0;
int by = 0;
int bz = 0;
int temp_i = 0;
int temp_f = 0;

// PRINT_BINARY - Arduino
// Prints a positive integer in binary format with a fixed width
// (c) 2007 by Peter H. Anderson
// http://www.phanderson.com/arduino/arduino_display.html
void print_binary(int v, int num_places)
{
	int mask=0, n;

	for (n=1; n<=num_places; n++)
	{
		mask = (mask << 1) | 0x0001;
	}
	v = v & mask;  // truncate v to specified number of places

	while(num_places)
	{

		if (v & (0x0001 << num_places-1))
		{
			Serial.print("1");
		}
		else
		{
			Serial.print("0");
		}

		--num_places;
		//if(((num_places%4) == 0) && (num_places != 0))
		//{
		//	Serial.print("_");
		//}
	}
}

// PRINT_DECIMAL
// Prints a positive integer with leading zero's in 5-digit field
void print_decimal(int v)
{
	if (v < 10000) Serial.print("0");
	if (v <  1000) Serial.print("0");
	if (v <   100) Serial.print("0");
	if (v <    10) Serial.print("0");
	Serial.print(v);
}

void setup() {
	// initialize digital pin 13 as an output (LED "L")
	//	TODO it seems this is "pin 13" of the original digital
	//	Arduino interface or some such thing. The OSEPP schematic
	//	connector pin has a "13" beside it. The signal is pin 17
	//	on the mpu symbol ("PB5(SCK)") and is called "PB5 (SCK/PCINT5)"
	//	in the ATmega328 datasheet).
	//
	//	So, how /exactly/ does pinMode work?
	//
	pinMode(13, OUTPUT);

	Wire.begin();					// join i2c bus (address optional for master)
	Serial.begin(9600);				// start serial 9600bps for output

	// read register map to get factory settings
	Wire.requestFrom(94, 10);		// request 10 bytes (all registers) from I2C Device 94 (TLV493 default ADDR=1)
	if (10 <= Wire.available()) {	// if ten bytes were received
		byte0  = Wire.read();		// Bx value MSB's
		byte1  = Wire.read();		// By value MSB's
		byte2  = Wire.read();		// Bz value MSB's
		byte3  = Wire.read();		// Selected channel, frame counter, temperature MSB's
		byte4  = Wire.read();		// By, Bx LSB's
		byte5  = Wire.read();		// Bz LSB's, PD flag
		byte6  = Wire.read();		// Temperature LSB's
		byte7  = Wire.read();		// Factory settings (store for mode command)
		byte8  = Wire.read();		// Factory settings (store for mode command)
		byte9  = Wire.read();		// Factory settings (store for mode command)
	}

	// output header for values
	Serial.print("Sample"); Serial.print("\t"); Serial.print("Byte0   "); Serial.print("\t"); Serial.print("Byte1   "); Serial.print("\t");
		Serial.print("Byte2   "); Serial.print("\t"); Serial.print("Byte3   "); Serial.print("\t"); Serial.print("Byte4   "); Serial.print("\t");
		Serial.print("Byte5   "); Serial.print("\t"); Serial.print("Byte6   "); Serial.print("\t"); Serial.print("Byte7   "); Serial.print("\t");
		Serial.print("Byte8   "); Serial.print("\t"); Serial.print("Byte9   "); Serial.print("\t"); Serial.print("Bx     "); Serial.print("\t");
		Serial.print("By     "); Serial.print("\t"); Serial.print("Bz     "); Serial.print("\t"); Serial.print("Temp   "); Serial.println("");

	// output registers as initially set before configuring
	Serial.print("00000\t"); print_binary(byte0,8); Serial.print("\t"); print_binary(byte1,8); Serial.print("\t");
		print_binary(byte2,8); Serial.print("\t"); print_binary(byte3,8); Serial.print("\t"); print_binary(byte4,8); Serial.print("\t");
		print_binary(byte5,8); Serial.print("\t"); print_binary(byte6,8); Serial.print("\t"); print_binary(byte7,8); Serial.print("\t");
		print_binary(byte8,8); Serial.print("\t"); print_binary(byte9,8); Serial.println("");

	// configure TLV493D into master-controlled mode
	Wire.beginTransmission(94);		// write to I2C TLV493 (default ADDR=1)
	Wire.write(byte(0x00));			// config byte 1
	Wire.write(byte(0x03));			// config byte 1
	Wire.write(byte(0x00));			// config byte 2
	Wire.write(byte(0x00));			// config byte 3
	Wire.endTransmission();			// stop transmitting
}

void loop() {
	// toggle LED On if Off, or vice versa
	if (led) {  // if On, turn Off
		led = false;
		digitalWrite(13, LOW);
	} else {    // otherwise turn On
		led = true;
		digitalWrite(13, HIGH);
	}

	// get measurement from TLV493D
	Wire.requestFrom(94, 10);		// request 10 bytes (all registers) from I2C Device 94 (TLV493 default ADDR=1)
	if (10 <= Wire.available()) {	// if ten bytes were received
		byte0  = Wire.read();		// Bx value MSB's
		byte1  = Wire.read();		// By value MSB's
		byte2  = Wire.read();		// Bz value MSB's
		byte3  = Wire.read();		// Selected channel, frame counter, temperature MSB's
		byte4  = Wire.read();		// By, Bx LSB's
		byte5  = Wire.read();		// Bz LSB's, PD flag
		byte6  = Wire.read();		// Temperature LSB's
		byte7  = Wire.read();		// Factory settings (store for mode command)
		byte8  = Wire.read();		// Factory settings (store for mode command)
		byte9  = Wire.read();		// Factory settings (store for mode command)
	}
	sample = sample + 1;			// increment sample counter
	
	// reassemble sensor values
	//bx = byte0;
	bx = ((uint16_t)byte0 << 4) | (byte4 & 0x0F);
	//by = byte1;
	by = ((uint16_t)byte1 << 4) | (byte4 >> 4);
	//bz = byte2;
	bz = ((uint16_t)byte2 << 4) | (byte5 & 0x0F);

	//temp = byte3;
	//temp = (((uint16_t)byte3 << 4) & 0xF0) | byte6;
	temp_i = ((int)byte3 << 4) | (int)byte6;
	// extend sign if 12-bit negative
	//if ((temp_i & 0x0800) != 0) {
		//(temp_i = 0xF000 | temp_i);
	//}
	//temp_f = ((float)temp_i * 1.1) - (340. * 1.1) + 25.0;
	temp_f = 99.999;

	// output Bx, By, Bz, temperature
	// TODO output time?
	print_decimal(sample); Serial.print("\t"); print_binary(byte0,8); Serial.print("\t"); print_binary(byte1,8); Serial.print("\t");
		print_binary(byte2,8); Serial.print("\t"); print_binary(byte3,8); Serial.print("\t"); print_binary(byte4,8); Serial.print("\t");
		print_binary(byte5,8); Serial.print("\t"); print_binary(byte6,8); Serial.print("\t"); print_binary(byte7,8); Serial.print("\t");
		print_binary(byte8,8); Serial.print("\t"); print_binary(byte9,8); Serial.print("\t"); Serial.print(bx, DEC); Serial.print("\t");
		Serial.print(by, DEC); Serial.print("\t"); Serial.print(bz, DEC); Serial.print("\t"); Serial.print(temp_i, DEC); Serial.print("\t");
		Serial.print(temp_f, 4); Serial.println("");

	delay(250);						// sample and LED blink rate
}
