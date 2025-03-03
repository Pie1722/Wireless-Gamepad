#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "00001"; // Address for communication

struct JoystickData {
  int xAxis;
  int yAxis;
  int zAxis;
  int rYaxis;
  int rXaxis;
  int rZaxis;
  int switch1;
  int switch2;
  int switch3;
  int switch4;
  int switch5;
  int switch6;
  int switch7;
  int switch8;
};

JoystickData joystick;

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float angleX, angleY;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY;
float elapsedTime, currentTime, previousTime;

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  radio.begin();
  //initialize_MPU6050();
  radio.openWritingPipe(address);
  radio.setAutoAck(true);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {

  joystick.xAxis = analogRead(A0);
  joystick.yAxis = analogRead(A1);
  joystick.zAxis = analogRead(A6);
  joystick.rXaxis = analogRead(A3);
  joystick.rYaxis = analogRead(A2);
  joystick.rZaxis = analogRead(A7);
  joystick.switch1 = digitalRead(3); // Read Switch 1
  joystick.switch2 = digitalRead(2); // Read Switch 2
  joystick.switch3 = digitalRead(8); // Read Switch 1
  joystick.switch4 = digitalRead(5); // Read Switch 2
  joystick.switch5 = digitalRead(6); // Read Switch 1
  joystick.switch6 = digitalRead(7); // Read Switch 2
  joystick.switch7 = digitalRead(0); // Read Switch 1
  joystick.switch8 = digitalRead(1); // Read Switch 2

   // read_IMU();    // Use MPU6050 for controling left, right, forward and backward movements

  
  radio.write(&joystick, sizeof(joystick)); // Send data
}

void initialize_MPU6050() {
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  // Configure Accelerometer
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000dps full scale)
  Wire.endTransmission(true);
}

void read_IMU() {
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-8g, we need to divide the raw values by 4096, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value

  // Calculating angle values using
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 1.15; // AccErrorX ~(-1.15) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 0.52; // AccErrorX ~(0.5)

  // === Read gyro data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000;   // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 4, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 32.8; // For a 1000dps range we have to divide first the raw value by 32.8, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 32.8;
  GyroX = GyroX + 1.85; //// GyroErrorX ~(-1.85)
  GyroY = GyroY - 0.15; // GyroErrorY ~(0.15)
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = GyroX * elapsedTime;
  gyroAngleY = GyroY * elapsedTime;

  // Complementary filter - combine acceleromter and gyro angle values
  angleX = 0.98 * (angleX + gyroAngleX) + 0.02 * accAngleX;
  angleY = 0.98 * (angleY + gyroAngleY) + 0.02 * accAngleY;
  // Map the angle values from -90deg to +90 deg into values from 0 to 255, like the values we are getting from the Joystick
  joystick.xAxis = map(angleX, -90, +90, 0, 1023);
  joystick.rYaxis = map(angleY, -90, +90, 1023, 0);
}
