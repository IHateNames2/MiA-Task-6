#include <Wire.h>

#define MPU6050_ADDR 0x68 // I2C address of the sensor
#define PWR_MGMT_1 0x6B // Power management register
#define GYRO_CONFIG 0x1B // Gyroscope configuration register
#define GYRO_XOUT_H 0x43 // Gyroscope X axis high byte register
#define GYRO_XOUT_L 0x44 // Gyroscope X axis low byte register
#define GYRO_YOUT_H 0x45 // Gyroscope Y axis high byte register
#define GYRO_YOUT_L 0x46 // Gyroscope Y axis low byte register
#define GYRO_ZOUT_H 0x47 // Gyroscope Z axis high byte register
#define GYRO_ZOUT_L 0x48 // Gyroscope Z axis low byte register

float yaw = 0; // Yaw angle in degrees
float z_dps = 0; // Z axis angular velocity in degrees per second
int16_t z = 0; // Z axis raw data in LSB
int16_t z_high = 0; // Z axis high byte data
int16_t z_low = 0; // Z axis low byte data

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Wake up the sensor from sleep mode by writing 0x00 to PWR_MGMT_1 register
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();
  
  // Set the gyroscope full-scale range to ±2000°/s by writing 0x18 to GYRO_CONFIG register
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x18);
  Wire.endTransmission();
}

void loop() {
  
   // Read the gyroscope Z axis high byte data from GYRO_ZOUT_H register
   Wire.beginTransmission(MPU6050_ADDR);
   Wire.write(GYRO_ZOUT_H);
   Wire.endTransmission();
   Wire.requestFrom(MPU6050_ADDR,1);
   if(Wire.available() > 0){
     z_high = Wire.read();
   }
   
   // Read the gyroscope Z axis low byte data from GYRO_ZOUT_L register
   Wire.beginTransmission(MPU6050_ADDR);
   Wire.write(GYRO_ZOUT_L);
   Wire.endTransmission();
   Wire.requestFrom(MPU6050_ADDR,1);
   if(Wire.available() > 0){
     z_low = Wire.read();
   }
   
   // Combine the high and low bytes into a 16-bit signed integer value
   z = (int16_t)((z_high << 8) | z_low);
   
   // Convert the raw data to degrees per second using a scale factor of 16.4 LSB/°/s
   z_dps = z / 16.4;
   
   // Integrate the angular velocity over time to get the yaw angle, assuming a sampling rate of 10 milliseconds
   yaw = yaw + z_dps * 0.01;
   
   // Print the yaw angle to the serial monitor
   Serial.print("Yaw: ");
   Serial.println(yaw);

}
