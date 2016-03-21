// (c) 2016 by Dale Scott.
// (c) 2016 by Steve Pye.
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

bool led = false;                 // state of Arduino LED ("L", "pin 13")

// TLV493 I2C register read values
int byte0 = 0;                    // Bx value MSB's
int byte1 = 0;                    // By value MSB's
int byte2 = 0;                    // Bz value MSB's
int byte3 = 0;                    // Selected channel, frame counter, temperature MSB's
int byte4 = 0;                    // By, Bx LSB's
int byte5 = 0;                    // Bz LSB's, PD flag
int byte6 = 0;                    // Temperature LSB's
int byte7 = 0;                    // Factory settings
int byte8 = 0;                    // Factory settings
int byte9 = 0;                    // Factory settings

// sensor values
int bx = 0;
int by = 0;
int bz = 0;
int temp = 0;

void setup() {
  // initialize digital pin 13 as an output (LED "L")
  // TODO it seems this is "pin 13" of the original digital
  //  Arduino interface or some such thing. The OSEPP schematic
  //  connector pin has a "13" beside it. The signal is pin 17
  //  on the mpu symbol ("PB5(SCK)") and is called "PB5 (SCK/PCINT5)"
  //  in the ATmega328 datasheet).
  //
  // So, how /exactly/ does pinMode work?
  pinMode(13, OUTPUT);

  Wire.begin();                   // join i2c bus (address optional for master)
  Serial.begin(9600);             // start serial 9600bps for output

  // read register map to get factory settings
  Wire.requestFrom(94, 10);       // request 10 bytes (all registers) from I2C Device 94 (TLV493 default ADDR=1)
  if (10 <= Wire.available()) {   // if ten bytes were received
    byte0  = Wire.read();         // Bx value MSB's
    byte1  = Wire.read();         // By value MSB's
    byte2  = Wire.read();         // Bz value MSB's
    byte3  = Wire.read();         // Selected channel, frame counter, temperature MSB's
    byte4  = Wire.read();         // By, Bx LSB's
    byte5  = Wire.read();         // Bz LSB's, PD flag
    byte6  = Wire.read();         // Temperature LSB's
    byte7  = Wire.read();         // Factory settings (store for mode command)
    byte8  = Wire.read();         // Factory settings (store for mode command)
    byte9  = Wire.read();         // Factory settings (store for mode command)
  }

  // output registers as initially set before configuring mode
  Serial.print(byte0, BIN); Serial.print("\t"); Serial.print(byte1, BIN); Serial.print("\t"); Serial.print(byte2, BIN); Serial.print("\t"); Serial.print(byte3, BIN); Serial.print("\t"); Serial.print(byte4, BIN); Serial.print("\t"); Serial.print(byte5, BIN); Serial.print("\t"); Serial.print(byte6, BIN); Serial.print("\t"); Serial.print(byte7, BIN); Serial.print("\t"); Serial.print(byte8, BIN); Serial.print("\t"); Serial.println(byte9, BIN);

  // configure TLV493D into master-controlled mode
  Wire.beginTransmission(94);   // write to I2C TLV493 (default ADDR=1)
  Wire.write(byte(0x00));       // config byte 1
  Wire.write(byte(0x03));       // config byte 1
  Wire.write(byte(0x00));       // config byte 2
  Wire.write(byte(0x00));       // config byte 3
  Wire.endTransmission();       // stop transmitting

  // output header for values to come
  Serial.println("TLV493D Bx, By, Bz (MSB's)");
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
  Wire.requestFrom(94, 10);       // request 10 bytes (all registers) from I2C Device 94 (TLV493 default ADDR=1)
  if (10 <= Wire.available()) {   // if ten bytes were received
    byte0  = Wire.read();         // Bx value MSB's
    byte1  = Wire.read();         // By value MSB's
    byte2  = Wire.read();         // Bz value MSB's
    byte3  = Wire.read();         // Selected channel, frame counter, temperature MSB's
    byte4  = Wire.read();         // By, Bx LSB's
    byte5  = Wire.read();         // Bz LSB's, PD flag
    byte6  = Wire.read();         // Temperature LSB's
    byte7  = Wire.read();         // Factory settings (store for mode command)
    byte8  = Wire.read();         // Factory settings (store for mode command)
    byte9  = Wire.read();         // Factory settings (store for mode command)
  }

  // reassemble sensor values
  //bx = byte0;
  bx = ((uint16_t)byte0 << 4) | (byte4 & 0x0F);
  //by = byte1;
  by = ((uint16_t)byte1 << 4) | (byte4 >> 4);
  //bz = byte2;
  bz = ((uint16_t)byte2 << 4) | (byte5 & 0x0F);
  //temp = byte3;
  temp = (((uint16_t)byte3 << 4) & 0xF0) | byte6;

  // output Bx, By, Bz, temperature
  // TODO output time?
  Serial.print(byte0, BIN); Serial.print("\t"); Serial.print(byte1, BIN); Serial.print("\t"); Serial.print(byte2, BIN); Serial.print("\t"); Serial.print(byte3, BIN); Serial.print("\t"); Serial.print(byte4, BIN); Serial.print("\t"); Serial.print(byte5, BIN); Serial.print("\t"); Serial.print(byte6, BIN); Serial.print("\t"); Serial.print(byte7, BIN); Serial.print("\t"); Serial.print(byte8, BIN); Serial.print("\t"); Serial.println(byte9, BIN);
  //Serial.print(bx, BIN); Serial.print("\t"); Serial.print(by, BIN); Serial.print("\t"); Serial.print(bz, BIN); Serial.print("\t"); Serial.println(temp, BIN);

  delay(250);                     // sample and LED blink rate
}

