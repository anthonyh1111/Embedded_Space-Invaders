// ----------------------------------------------------------------------------------------------------
// =========== MAX30101 Photodetector Sensor ============ 
// ----------------------------------------------------------------------------------------------------

/*
 * Photo Sensor (PPG) Include and Settings
 */
#include <Wire.h>     // include the Wire protocol (for I2C communication)
#include "MAX30105.h" // include the Photodetector library
MAX30105 photoSensor; // instantiate the Photodetector object
const int MAX_READING = 32767; // 2^15-1; might change w/config – TEST IT!!



const int COVER_THRESHOLD = 1600;  // Threshold for photosensor to detect firing 
const int SMOOTHING_WINDOW = 10;   // Filter for more accurate values  

int ppgBuffer[SMOOTHING_WINDOW];
int ppgIndex = 0;
int smoothedPPG = 0;



/*
 * Initialize the sensor to use red, green, and IR LEDs for the best signal
 */
void setupPhotoSensor() {
  // Use default I2C port at 400kHz speed
  if(!photoSensor.begin(Wire, I2C_SPEED_FAST)) {
    sendMessage("MAX30101 not found. Please check wiring/power.");
    while(1); // “Halt program” by looping forever
  }

  // Sensor settings to get 50 Hz signal
  // https://cdn.sparkfun.com/assets/8/1/c/9/0/MAX30101_Datasheet.pdf
  byte ledBrightness = 0x1F; // Options: 0==Off to 255==50mA (0x1F==50/8mA)
  byte sampleAverage = 4;    // Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3;          // Options: 1==R, 2==R+IR, 3==R+G+IR
  int sampleRate = 400;      // Options: 50,100,200,400,800,1000,1600,3200
  int pulseWidth = 215;      // Options: 69, 118, 215, 411
  int adcRange = 16384;      // Options: 2048, 4096, 8192, 16384

  // Configure sensor with these settings
  photoSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

/*
 * We are reading the IR signal here, but you should experiment with it.
 * The signal is negated because of the inverse relationship with blood
 * volume - a small reading means most absorption (peak of pulse).
 */

//Low-pass filter for photosensor 
void readPhotoSensor() {
  // Read raw IR value with no inversion
  int rawIR = photoSensor.getIR();
  
  // Simple moving average filter
  ppgBuffer[ppgIndex] = rawIR;
  ppgIndex = (ppgIndex + 1) % SMOOTHING_WINDOW;
  
  // Calculate smoothed value
  smoothedPPG = 0;
  for(int i=0; i<SMOOTHING_WINDOW; i++) {
    smoothedPPG += ppgBuffer[i];
  }

  smoothedPPG /= SMOOTHING_WINDOW;
  
 /* thresholds for firing 
  Serial.print("PPG: ");
  Serial.print(smoothedPPG);
  Serial.print(" | Threshold: ");
  Serial.print(COVER_THRESHOLD);
  Serial.print(" | Firing: ");
  Serial.println(isCovered() ? "YES" : "NO");
  */
}



// checks to see if photosensor is covered 
bool isCovered() {
  return smoothedPPG > COVER_THRESHOLD;
}
