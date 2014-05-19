#include <math.h>

#define SENSORS_GAUSS_TO_MICROTESLA (100)

static float _hmc5883_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _hmc5883_Gauss_LSB_Z  = 980.0F;   // Varies with gain
static int address = 0x1E; //0011110b, I2C 7bit address of HMC5883

void setup(){
    Serial.begin(9600);
    Wire.begin();
  
    Wire.beginTransmission(address);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();
}

void loop(){
    float x,y,z;

    Wire.beginTransmission(address);
    Wire.write(0x03);
    Wire.endTransmission();
  
    Wire.requestFrom(address, 6);
    if(6<=Wire.available()){
        uint8_t xhi = Wire.read();
        uint8_t xlo = Wire.read();
        uint8_t zhi = Wire.read();
        uint8_t zlo = Wire.read();
        uint8_t yhi = Wire.read();
        uint8_t ylo = Wire.read();
        
        x = (int16_t)(xlo | ((int16_t)xhi << 8));
        y = (int16_t)(ylo | ((int16_t)yhi << 8));
        z = (int16_t)(zlo | ((int16_t)zhi << 8));
    }
    
    float heading = atan2(y, x);
    float declinationAngle = 0.22;
    
    heading += declinationAngle;
      
    if(heading < 0)
        heading += 2 * M_PI;
        
    if(heading > 2 * M_PI)
        heading -= 2 * M_PI;
       
    float headingDegrees = heading * 180/ M_PI; 
  
    Serial.print("x: ");
    Serial.print(x);
    Serial.print("  y: ");
    Serial.print(y);
    Serial.print("  z: ");
    Serial.print(z);
    Serial.print(" Heading: ");
    Serial.println(headingDegrees);
  
    delay(250);
}