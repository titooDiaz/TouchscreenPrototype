#include <Arduino.h>

// --- Sensores táctiles del ESP32 ---
#define TOUCH_T0 4    // D4
#define TOUCH_T1 0    // D0
#define TOUCH_T2 2    // D2
#define TOUCH_T3 15   // D15
#define TOUCH_T4 13   // D13
#define TOUCH_T5 12   // D12
#define TOUCH_T6 14   // D14
#define TOUCH_T7 27   // D27
#define TOUCH_T8 33   // D33
#define TOUCH_T9 32   // D32
#define THRESHOLD 40  

int sensors[] = {TOUCH_T0, TOUCH_T1, TOUCH_T2, TOUCH_T3, TOUCH_T4, TOUCH_T5, TOUCH_T6, TOUCH_T7, TOUCH_T8, TOUCH_T9};
const char* names[] = {"T0","T1","T2","T3","T4","T5","T6","T7","T8","T9"};

int lastSensor = -1;       // Para tracking de dirección
int lastRowSensor = -1;    // Para tu fila 1x3
unsigned long lastTouchTime = 0; // momento del último toque
const unsigned long TIMEOUT = 500; // 500 ms

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Lectura táctil ESP32 lista...");
}

void loop() {
  int currentSensor = -1;

  // --- Cual sensor está tocando ---
  for (int i = 0; i < 10; i++) {
    int val = touchRead(sensors[i]);

    if (val < THRESHOLD && val > 0) {
      currentSensor = i; // guardamos índice del sensor activo
      lastTouchTime = millis(); // actualizar momento del último toque
      break; // solo consideramos el primero que se toque
    }
  }

  // --- Si pasa más de 500ms sin tocar nada, borrar último ---
  if (currentSensor == -1 && millis() - lastTouchTime > TIMEOUT) {
    lastSensor = -1;
    lastRowSensor = -1;
  }

  // --- Detectar dirección en la fila t3-t0-t4 ---
  if (currentSensor != -1) {
    if (currentSensor == 3 || currentSensor == 0 || currentSensor == 4) {
      if (lastRowSensor != -1 && currentSensor != lastRowSensor) {
        // Movimientos DERECHA
        if ((lastRowSensor == 3 && currentSensor == 0) || 
            (lastRowSensor == 0 && currentSensor == 4) || 
            (lastRowSensor == 4 && currentSensor == 3)) {
          Serial.println("derecha");
        }
        // Movimientos IZQUIERDA
        else if ((lastRowSensor == 0 && currentSensor == 3) || 
                 (lastRowSensor == 4 && currentSensor == 0) || 
                 (lastRowSensor == 3 && currentSensor == 4)) {
          Serial.println("izquierda");
        }
      }
      lastRowSensor = currentSensor; // actualizar último de la fila
    }
    lastSensor = currentSensor; // actualizar último global
  }
}