
const int X_PIN = A2;
const int Y_PIN = A3;
const int Z_PIN = A4;

const int X_ZERO = 2150;
const int Y_ZERO = 2150; 
const int Z_ZERO = 2250;

const int WINDOW_SIZE = 5; // Size of the moving average window
int ax_buffer[WINDOW_SIZE] = {0};
int ay_buffer[WINDOW_SIZE] = {0};
int az_buffer[WINDOW_SIZE] = {0};
int buffer_index = 0;

void setupAccelSensor() {
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
  Serial.begin(115200); 

}

void readAccelSensor() {
  ax = analogRead(X_PIN); 
  ay = analogRead(Y_PIN);
  az = analogRead(Z_PIN);

  // Prints ray accelerometer value sfor debugging 
  /*
  Serial.print("Raw - ax: ");
  Serial.print(ax);
  Serial.print(" ay: ");
  Serial.print(ay);
  Serial.print(" az: ");
  Serial.println(az);
  */
  
  // Add the new readings to the buffer
  ax_buffer[buffer_index] = ax;
  ay_buffer[buffer_index] = ay;
  az_buffer[buffer_index] = az;
  buffer_index = (buffer_index + 1) % WINDOW_SIZE;
}

//smooths the data for more accurate movement 
void smoothAccelData() {
  int ax_sum = 0, ay_sum = 0, az_sum = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    ax_sum += ax_buffer[i];
    ay_sum += ay_buffer[i];
    az_sum += az_buffer[i];
  }
  ax = ax_sum / WINDOW_SIZE;
  ay = ay_sum / WINDOW_SIZE;
  az = az_sum / WINDOW_SIZE;

  // Prints Smoothed values for debugging 
  /*
  Serial.print("Smoothed - ax: ");
  Serial.print(ax);
  Serial.print(" ay: ");
  Serial.print(ay);
  Serial.print(" az: ");
  Serial.println(az);
  */
}

int getOrientation() {
  int orientation = 0;

  // Subtracts out the zeros 
  int x = ax - X_ZERO;
  int y = ay - Y_ZERO;
  int z = az - Z_ZERO;

  // Prints zero-adjusted values for debugging 
  /*
  Serial.print("Zero-Adjusted - x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.println(z);
  */

  // If ax has the biggest magnitude, it's either left or right
  if (abs(x) >= abs(y) && abs(x) >= abs(z)) {
    if (x > -200 ) // left 
      orientation = 3;
    else if (x < 300) // right 
      orientation = 4;
  }
  // If ay has the biggest magnitude, it's either up or down
  else if (abs(y) >= abs(x) && abs(y) >= abs(z)) {
    if (y > 0) // up 
      orientation = 1;
    else if (y < 0) // shoot 
      orientation = 2;
  }
  // If az has the biggest magnitude, it's flat (or upside-down)
  else if (abs(z) > abs(x) && abs(z) >= abs(y)) {
    orientation = 0; // flat
  }

  // Prints orientation for debugging 
  Serial.print("Orientation: ");
  Serial.println(orientation);

  return orientation;
}
