#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BMP3XX.h>
#include <bmp3.h>
#include <bmp3_defs.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

//BMP WIRING:
#define BMP_SCK 19
#define BMP_SDA 18

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;//create sensor

void setup() {
//set up bmp388
Serial.begin(115200);//serial for debugging

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP388 sensor, check wiring");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);//set up other parameters (arbitrary)
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  if (! bmp.performReading()) {//check sensor can take readings
    Serial.println("Failed to perform reading :(");
    return;
  }
  
}


float altitude_start = bmp.readAltitude(SEALEVELPRESSURE_HPA);//offest for all subsequent altitude reaadings, this is the zero point



void loop() {
  //ascending - do nothing:
  float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - altitude_start;
  delay(100);
  while (altitude-(bmp.readAltitude(SEALEVELPRESSURE_HPA) - altitude_start) < 0) delay(10);//check still ascending
  
  
  //descending:
  delay(5000);//wait time to ensure definitely past apogee
  //deploy heat shield


  //wait until 200m altitude reached:
  while ((bmp.readAltitude(SEALEVELPRESSURE_HPA)-altitude_start)>200) {
    delay(10);
  }
  //release heat shield
  //deploy parachrute

}
