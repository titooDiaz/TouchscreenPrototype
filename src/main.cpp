#include <Arduino.h>
// --- Button and LED pins ---
#define BUTTON_PIN 0 // Boot button (GPIO0)
#define LED_ON 12 // Green LED
#define LED_OFF 18 // Red LED

bool touchEnabled = true; // Touchpad starts enabled
bool lastButtonState = HIGH; // Button not pressed

// --- ESP32 touch sensors ---
#define TOUCH_T0 4    
#define TOUCH_T3 15   
#define TOUCH_T4 13   
#define TOUCH_T6 14   
#define TOUCH_T7 27   
#define TOUCH_T8 33   

#define THRESHOLD 40
#define TIMEOUT 250  // Reset delay

int sensors[] = {TOUCH_T0, TOUCH_T3, TOUCH_T4, TOUCH_T6, TOUCH_T7, TOUCH_T8};
const char* names[] = {"T0", "T3", "T4", "T6", "T7", "T8"};

int lastSensor = -1;
int lastRowSensor = -1;
int lastGroup = -1; // 0 = group 1, 1 = group 2
unsigned long lastTouchTime = 0;

bool verticalMode = false;
String verticalDirection = ""; // "arriba" / "abajo"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 touch sensors ready...");

  // Pin setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_OFF, OUTPUT);

  // Touchpad ON at start
  digitalWrite(LED_ON, HIGH);
  digitalWrite(LED_OFF, LOW);
}

void loop() {
  int currentSensor = -1;
  int currentGroup = -1;

  // --- Button control ---
  bool reading = digitalRead(BUTTON_PIN);

  // Detect press
  if (reading == LOW && lastButtonState == HIGH) {
    touchEnabled = !touchEnabled;

    if (touchEnabled) {
      Serial.println("touch_on");
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, LOW);
    } else {
      Serial.println("touch_off");
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, HIGH);
    }
  }

  lastButtonState = reading;

  // Touch disabled → stop here
  if (!touchEnabled) return;

  // --- Read touch sensors ---
  for (int i = 0; i < 6; i++) {
    int value = touchRead(sensors[i]);
    if (value < THRESHOLD && value > 0) {
      currentSensor = i;
      lastTouchTime = millis();
      break;
    }
  }

  // --- Reset states if no touch ---
  if (currentSensor == -1 && millis() - lastTouchTime > TIMEOUT) {
    lastSensor = -1;
    lastRowSensor = -1;
    lastGroup = -1;
    verticalMode = false;
    verticalDirection = "";
  }

  // --- Touch detected ---
  if (currentSensor != -1) {
    // Pick group
    if (currentSensor <= 2) currentGroup = 0; 
    else currentGroup = 1;

    // --- Vertical movement ---
    if (!verticalMode && lastGroup != -1 && currentGroup != lastGroup) {
      if (lastGroup == 0 && currentGroup == 1) {
        Serial.println("arriba");
        verticalDirection = "arriba";
      } else if (lastGroup == 1 && currentGroup == 0) {
        Serial.println("abajo");
        verticalDirection = "abajo";
      }
      Serial.flush();
      verticalMode = true;
    }

    // Vertical mode active → only repeat movement on new sensor
    else if (verticalMode && currentSensor != lastSensor) {
      Serial.println(verticalDirection);
      Serial.flush();
    }

    // --- Horizontal movement ---
    else if (!verticalMode) {

      // Group 1
      if (currentGroup == 0) {
        if (lastRowSensor != -1 && currentSensor != lastRowSensor) {
          if ((lastRowSensor == 1 && currentSensor == 0) ||
              (lastRowSensor == 0 && currentSensor == 2) ||
              (lastRowSensor == 2 && currentSensor == 1)) {
            Serial.println("derecha");
          } else if ((lastRowSensor == 0 && currentSensor == 1) ||
                     (lastRowSensor == 2 && currentSensor == 0) ||
                     (lastRowSensor == 1 && currentSensor == 2)) {
            Serial.println("izquierda");
          }
          Serial.flush();
        }
        lastRowSensor = currentSensor;
      }

      // Group 2
      if (currentGroup == 1) {
        if (lastRowSensor != -1 && currentSensor != lastRowSensor) {
          if ((lastRowSensor == 3 && currentSensor == 4) ||
              (lastRowSensor == 4 && currentSensor == 5) ||
              (lastRowSensor == 5 && currentSensor == 3)) {
            Serial.println("derecha");
          } else if ((lastRowSensor == 4 && currentSensor == 3) ||
                     (lastRowSensor == 5 && currentSensor == 4) ||
                     (lastRowSensor == 3 && currentSensor == 5)) {
            Serial.println("izquierda");
          }
          Serial.flush();
        }
        lastRowSensor = currentSensor;
      }
    }

    // Update state
    lastGroup = currentGroup;
    lastSensor = currentSensor;
  }
}
