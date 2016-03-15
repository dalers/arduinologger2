// (c) 2016 by Dale Scott.
// Provided according to the 2-clause BSD License.
// https://opensource.org/licenses/BSD-2-Clause

// Gets field measurement from I2C TLV493D using Wire library

#include <Wire.h>
#include <SoftwareSerial.h>

// TLV_ADDRESS1 0x5E (decimal 94) (ADDR=1)
// TLV_ADDRESS2 0x1F (decimal 31) (ADDR=0)

bool led = false;                 // state of Arduino LED ("L", "pin 13")

// TLV493 I2C read registers
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

void setup() {
  // initialize digital pin 13 as an output (LED "L")
  // TODO: it seems this is "pin 13" of the original digital
  //  Arduino interface or some such thing. The OSEPP schematic
  //  connector pin has a "13" beside it. The signal is pin 17
  //  on the mpu symbol ("PB5(SCK)") and is called "PB5 (SCK/PCINT5)"
  //  in the ATmega328 datasheet).
  //
  // So, how exactly does pinMode work?
  pinMode(13, OUTPUT);

  Wire.begin();                   // join i2c bus (address optional for master)
  Serial.begin(9600);             // start serial 9600bps for output

  // read register map for factory settings
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

  // output initial TLV493D register values
  Serial.println("TLV493D Registers");
  Serial.println(byte0);
  Serial.println(byte1);
  Serial.println(byte2);
  Serial.println(byte3);
  Serial.println(byte4);
  Serial.println(byte5);
  Serial.println(byte6);
  Serial.println(byte7);
  Serial.println(byte8);
  Serial.println(byte9);
  Serial.println("");

  // configure TLV493D into master-controlled mode
  Wire.beginTransmission(94);   // write to I2C TLV493 (default ADDR=1)
  Wire.write(byte(0x00));       // config byte 1
  Wire.write(byte(0x03));       // config byte 1
  Wire.write(byte(0x00));       // config byte 2
  Wire.write(byte(0x00));       // config byte 3
  Wire.endTransmission();       // stop transmitting
}

void loop() {
  // toggle LED On/Off each pass through loop
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

  Serial.println("TLV493D Bx, By, Bz (MSB's)");
  Serial.println(byte0);
  Serial.println(byte1);
  Serial.println(byte2);
  Serial.println("");

  delay(250);                     // sample and LED blink rate
}

