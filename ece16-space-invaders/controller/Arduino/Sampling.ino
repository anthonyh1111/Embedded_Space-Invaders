
// Function prototype for readAccelSensor


void readAccelSensor();

unsigned long timeStart = 0;
const int SAMPLE_RATE = 100; // Hz
const unsigned long SAMPLE_DELAY = 1e6 / SAMPLE_RATE;

bool sampleSensors() {
  unsigned long timeEnd = micros();
  if (timeEnd - timeStart >= SAMPLE_DELAY) {
    timeStart = timeEnd;
    readAccelSensor(); // Defined in accelerometer.ino
    readPhotoSensor(); // Defined in Photodetector.ino
    return true;
  }
  return false;
}
