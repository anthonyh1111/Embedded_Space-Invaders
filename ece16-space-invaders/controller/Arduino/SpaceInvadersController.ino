
// Function prototypes 
void setupAccelSensor();
void readAccelSensor();
void smoothAccelData();
int getOrientation();

void setupDisplay();
void writeDisplay(const char *message, int row, bool clearRow);
void displayScoreAndLives(int score, int lives);
void displayTopScores(int topScores[3]);
void displaySensitivity(int sensitivity);

void setupCommunication();
String receiveMessage();
void sendMessage(String message);

void setupPhotoSensor();

unsigned long lastFireTime = 0;
const unsigned long FIRE_COOLDOWN = 0; // ms between shots but kept at zero so rapid firing could happen

int ax = 0, ay = 0, az = 0;
int ppg = 0;
int sampleTime = 0;
bool sending = false;

int sensitivity = 1; //initial sensitivity 
const int BUTTON_PIN = 12;

int currentScore = 0;
int currentLives = 3;
int topScores[3] = {0, 0, 0}; //top score array 
unsigned long lastSendTime = 0;

//updates the top score screen 
void updateTopScores(int newScore) {
  for (int i = 0; i < 3; i++) {
    if (newScore > topScores[i]) {
      // Shifts the lower scores down 
      for (int j = 2; j > i; j--) {
        topScores[j] = topScores[j-1];
      }
      topScores[i] = newScore;
      break;
    }
  }
}

//Initialize all the components
void setup() {
  setupAccelSensor(); 
  setupCommunication();
  setupDisplay();
  setupPhotoSensor();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  sending = false;

  displaySensitivity(sensitivity);
  displayTopScores(topScores);
  displayScoreAndLives(0, 3); // Initial display values
}

void loop() {

  // Check if the button is pressed to cycle sensitivity settings
  if (digitalRead(BUTTON_PIN) == LOW) {
    sensitivity = (sensitivity % 3) + 1;
    delay(500); // Debounce delay
    displaySensitivity(sensitivity);
  }

  // Parse command coming from Python but there wasn't enough space on the display so it won't show. 
  String command = receiveMessage();
  if (command == "stop") {
    sending = false;
    writeDisplay("Ctrl: Off", 5, true);
  } else if (command == "start") {
    sending = true;
    writeDisplay("Ctrl: On", 5, true);
  }


  // incoming command handling for the display. Handles score, lives, and top scores with live updates . 
  if (command.startsWith("SCORE:")) {
    currentScore = command.substring(6).toInt();
    displayScoreAndLives(currentScore, currentLives);
  } else if (command.startsWith("LIVES:")) {
    currentLives = command.substring(6).toInt();
    displayScoreAndLives(currentScore, currentLives);
  } else if (command.startsWith("GAMEOVER:")) {
    int score = command.substring(9).toInt();
    updateTopScores(score);
    displayTopScores(topScores);
  }



  
    readPhotoSensor();
  // Photodetector firing 
    if (isCovered()) {
    if (millis() - lastFireTime > FIRE_COOLDOWN) {
      sendMessage("2");
      lastFireTime = millis();
    }
  }

  // Read the accelerometer data for movement
  readAccelSensor(); 
  smoothAccelData(); // Apply moving average filter

  // Send the orientation of the board
  if (sending) {
    int orientation = getOrientation();

    /*  Prints orientation and sensitivity for debugging 
  
    Serial.print("Sensitivity: ");
    Serial.println(sensitivity);
    Serial.print("Sending Orientation: ");
    Serial.println(orientation);
    
    */
      
  // Sends the orientation to the Python controller
    
  Serial.println(orientation);
    
  // logic for sensitivity (how fast you move left and right) 
if (orientation == 3 || orientation == 4) {
    unsigned long now = millis();
    switch(sensitivity) {
        case 1: if (now - lastSendTime >= 500) { sendMessage(String(orientation)); lastSendTime = now; } break;
        case 2: if (now - lastSendTime >= 250) { sendMessage(String(orientation)); lastSendTime = now; } break;
        case 3: sendMessage(String(orientation)); break;
    }
}
