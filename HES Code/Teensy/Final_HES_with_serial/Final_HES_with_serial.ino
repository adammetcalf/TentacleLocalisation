#include <i2c_t3.h> //include I2C library

//I2C bus 0 is used to connect to the sensors for data aquisition. I2C bus 1 is used to pass data between the myRIO and the Arduino.

// LSB of address can be changed by A0 and A1 being set High/low. This allows 4 slave sensors on the slave bus with adresses:
//0x0C A0 = Low, A1 = Low
//0x0D A0 = High, A1 = Low
//0x0E A0 = Low, A1 = High
//0x0F A0 = High, A1 = High
#define Addr1 0x0C // HES 1
#define Addr2 0x0D // HES 2
#define Addr3 0x0E // HES 3 
#define Addr4 0x0F // HES 4 

// bytes and byte array declared as volatile, since it is necessary to inform the compiler that the values may
// change via interrupts, so that the compiler doesn't optimise the code away.
volatile byte dataPacket[24]; // array of bytes to send 
volatile unsigned int data1[7]; //HES 1 array of 7 bytes
volatile unsigned int data2[7]; //HES 2 array of 7 bytes
volatile unsigned int data3[7]; //HES 3 array of 7 bytes
volatile unsigned int data4[7]; //HES 4 array of 7 bytes

int del = 2; //delay between register commands

word x1=0;
word y_1=0;
word z1=0;
word x2=0;
word y2=0;
word z2=0;
word x3=0;
word y3=0;
word z3=0;
word x4=0;
word y4=0;
word z4=0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////          /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// INIT ALL /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////          /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Wire.begin(); // Initialise I2C communication bus 0 as MASTER for reading sensors on bus 0.
  Wire1.begin(1); //Initialise I2C communication bus 1 as Slave for sending data to myRIO.
  Wire1.onRequest(requestEvent); // register event
  Serial.begin(115200);


  ///////////////////////////////////////////////// HES 1 init /////////////////////////////////////////////////
  
  Wire.beginTransmission(Addr1); // Start I2C slave Transmission HES 1
  Wire.write(0x60); // Select Write register command
  Wire.write(0x00); // Set AH = 0x00, BIST disabled
  Wire.write(0x5C); // Set AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5
  Wire.write(0x00); // Select address register, (0x00 << 2)
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr1, 1);   // Request 1 byte of data
 
  if(Wire.available() == 1)   // Read status byte
    {
    unsigned int c1 = Wire.read();
    }
  delay(300);
  ///////////////////////////////////////////////// HES 1 init end /////////////////////////////////////////////////


  ///////////////////////////////////////////////// HES 2 init /////////////////////////////////////////////////
  
  Wire.beginTransmission(Addr2); // Start I2C slave Transmission HES 2
  Wire.write(0x60); // Select Write register command
  Wire.write(0x00); // Set AH = 0x00, BIST disabled
  Wire.write(0x5C); // Set AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5
  Wire.write(0x00); // Select address register, (0x00 << 2)
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr2, 1);   // Request 1 byte of data
 
  if(Wire.available() == 1)   // Read status byte
    {
    unsigned int c2 = Wire.read();
    }
  delay(300);
  /////////////////////////////////////////////////  HES 2 init end /////////////////////////////////////////////////
  
  ///////////////////////////////////////////////// HES 3 init /////////////////////////////////////////////////
  
  Wire.beginTransmission(Addr3); // Start I2C slave Transmission HES 3
  Wire.write(0x60); // Select Write register command
  Wire.write(0x00); // Set AH = 0x00, BIST disabled
  Wire.write(0x5C); // Set AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5
  Wire.write(0x00); // Select address register, (0x00 << 2)
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr3, 1);   // Request 1 byte of data
 
  if(Wire.available() == 1)   // Read status byte
    {
    unsigned int c3 = Wire.read();
    }
  delay(300);
  ///////////////////////////////////////////////// HES 3 init end /////////////////////////////////////////////////

  ///////////////////////////////////////////////// HES 4 init /////////////////////////////////////////////////
  
  Wire.beginTransmission(Addr4); // Start I2C slave Transmission HES 4
  Wire.write(0x60); // Select Write register command
  Wire.write(0x00); // Set AH = 0x00, BIST disabled
  Wire.write(0x5C); // Set AL = 0x5C, Hall plate spinning rate = DEFAULT, GAIN_SEL = 5
  Wire.write(0x00); // Select address register, (0x00 << 2)
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr4, 1);   // Request 1 byte of data
 
  if(Wire.available() == 1)   // Read status byte
    {
    unsigned int c4 = Wire.read();
    }
  delay(300);
  ///////////////////////////////////////////////// HES 4 init end /////////////////////////////////////////////////

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////          /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// END INIT /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////          /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////           /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// MAIN Loop /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////           /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {

  getAddr1(); // read data from sensor 1
  getAddr2(); // read data from sensor 2
  getAddr3(); // read data from sensor 3 
  getAddr4(); // read data from sensor 4 
  fillArray(); // build byte array for transfer
  outputtoSerial(); //output to Serial Monitor (delete later)
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////          /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// END MAIN /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////          /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////           /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// Functions /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////           /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  for (int i=0; i<24; i++)
  {
    Wire1.write(dataPacket[i]);  //data bytes written to I2C bus 1
  }

}
// end function

//////////////Function that gets the data from sensor 1///////////////////////////////////
void getAddr1() {
  Wire.beginTransmission(Addr1); // Start I2C Transmission
  Wire.write(0x3E); // Start single meaurement mode, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission

  Wire.requestFrom(Addr1, 1); // Request 1 byte of data
  if(Wire.available() == 1) // Read status byte
    {
    unsigned int c1 = Wire.read();
    }
  delay(del);
 
  Wire.beginTransmission(Addr1); // Start I2C Transmission
  Wire.write(0x4E); // Send read measurement command, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr1, 7); // Request 7 bytes of data
  // Read 7 bytes of data
  // status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
  if(Wire.available() == 7);
    {
    data1[0] = Wire.read();
    data1[1] = Wire.read();
    data1[2] = Wire.read();
    data1[3] = Wire.read();
    data1[4] = Wire.read();
    data1[5] = Wire.read();
    data1[6] = Wire.read();
    }
 
}
//////////////// end Function ////////////////




//////////////Function that gets the data from sensor 2///////////////////////////////////
void getAddr2() {
  Wire.beginTransmission(Addr2); // Start I2C Transmission
  Wire.write(0x3E); // Start single meaurement mode, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission

  Wire.requestFrom(Addr2, 1); // Request 1 byte of data
  if(Wire.available() == 1) // Read status byte
    {
    unsigned int c2 = Wire.read();
    }
  delay(del);
 
  Wire.beginTransmission(Addr2); // Start I2C Transmission
  Wire.write(0x4E); // Send read measurement command, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr2, 7); // Request 7 bytes of data
  // Read 7 bytes of data
  // status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
  if(Wire.available() == 7);
    {
    data2[0] = Wire.read();
    data2[1] = Wire.read();
    data2[2] = Wire.read();
    data2[3] = Wire.read();
    data2[4] = Wire.read();
    data2[5] = Wire.read();
    data2[6] = Wire.read();
    }
 
}
//////////////// end Function ////////////////



//////////////Function that gets the data from sensor 3///////////////////////////////////
void getAddr3() {
  Wire.beginTransmission(Addr3); // Start I2C Transmission
  Wire.write(0x3E); // Start single meaurement mode, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission

  Wire.requestFrom(Addr3, 1); // Request 1 byte of data
  if(Wire.available() == 1) // Read status byte
    {
    unsigned int c3 = Wire.read();
    }
  delay(del);
 
  Wire.beginTransmission(Addr3); // Start I2C Transmission
  Wire.write(0x4E); // Send read measurement command, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr3, 7); // Request 7 bytes of data
  // Read 7 bytes of data
  // status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
  if(Wire.available() == 7);
    {
    data3[0] = Wire.read();
    data3[1] = Wire.read();
    data3[2] = Wire.read();
    data3[3] = Wire.read();
    data3[4] = Wire.read();
    data3[5] = Wire.read();
    data3[6] = Wire.read();
    }
 
}
//////////////// end Function ////////////////



//////////////Function that gets the data from sensor 4///////////////////////////////////
void getAddr4() {
  Wire.beginTransmission(Addr4); // Start I2C Transmission
  Wire.write(0x3E); // Start single meaurement mode, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission

  Wire.requestFrom(Addr4, 1); // Request 1 byte of data
  if(Wire.available() == 1) // Read status byte
    {
    unsigned int c4 = Wire.read();
    }
  delay(del);
 
  Wire.beginTransmission(Addr4); // Start I2C Transmission
  Wire.write(0x4E); // Send read measurement command, ZYX enabled
  Wire.endTransmission(); // Stop I2C Transmission
 
  Wire.requestFrom(Addr4, 7); // Request 7 bytes of data
  // Read 7 bytes of data
  // status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
  if(Wire.available() == 7);
    {
    data4[0] = Wire.read();
    data4[1] = Wire.read();
    data4[2] = Wire.read();
    data4[3] = Wire.read();
    data4[4] = Wire.read();
    data4[5] = Wire.read();
    data4[6] = Wire.read();
    }

}
//////////////// end Function ////////////////

// function to build transfer array
void fillArray() {
  dataPacket[0] = data1[1];
  dataPacket[1] = data1[2];
  dataPacket[2] = data1[3];
  dataPacket[3] = data1[4];
  dataPacket[4] = data1[5];
  dataPacket[5] = data1[6];
  dataPacket[6] = data2[1];
  dataPacket[7] = data2[2];
  dataPacket[8] = data2[3];
  dataPacket[9] = data2[4];
  dataPacket[10] = data2[5];
  dataPacket[11] = data2[6];
  dataPacket[12] = data3[1]; 
  dataPacket[13] = data3[2]; 
  dataPacket[14] = data3[3]; 
  dataPacket[15] = data3[4]; 
  dataPacket[16] = data3[5]; 
  dataPacket[17] = data3[6]; 
  dataPacket[18] = data4[1]; 
  dataPacket[19] = data4[2]; 
  dataPacket[20] = data4[3]; 
  dataPacket[21] = data4[4]; 
  dataPacket[22] = data4[5]; 
  dataPacket[23] = data4[6]; 
}
/////////////////// end function///////////////////////////////

/////// Function that outputs to serial monitor //////////
void outputtoSerial() {

  x1 = word(data1[1],data1[2]);
  y_1 = word(data1[3],data1[4]);
  z1 = word(data1[5],data1[6]);
  x2 = word(data2[1],data2[2]);
  y2 = word(data2[3],data2[4]);
  z2 = word(data2[5],data2[6]);
  x3 = word(data3[1],data3[2]);
  y3 = word(data3[3],data3[4]);
  z3 = word(data3[5],data3[6]);
  x4 = word(data4[1],data4[2]);
  y4 = word(data4[3],data4[4]);
  z4 = word(data4[5],data4[6]);

  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.print("HES1 X-Axis : ");
  Serial.print(x1);
  Serial.print(" Y-Axis : ");
  Serial.print(y_1);
  Serial.print(" Z-Axis : ");
  Serial.println(z1);

  Serial.print("HES2 X-Axis : ");
  Serial.print(x2);
  Serial.print(" Y-Axis : ");
  Serial.print(y2);
  Serial.print(" Z-Axis : ");
  Serial.println(z2);

  Serial.print("HES3 X-Axis : ");
  Serial.print(x3);
  Serial.print(" Y-Axis : ");
  Serial.print(y3);
  Serial.print(" Z-Axis : ");
  Serial.println(z3);

  Serial.print("HES4 X-Axis : ");
  Serial.print(x4);
  Serial.print(" Y-Axis : ");
  Serial.print(y4);
  Serial.print(" Z-Axis : ");
  Serial.println(z4);
  
}
//////////////// end Function ////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////               /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// End Functions /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////               /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
