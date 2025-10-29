# mouse_control_serial.py
import pyautogui
import serial

# pasos del cusror
STEP = 20  

#Puerto de la esp32
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

def derecha():
    x, y = pyautogui.position()
    pyautogui.moveTo(x + STEP, y)

def izquierda():
    x, y = pyautogui.position()
    pyautogui.moveTo(x - STEP, y)

def arriba():
    x, y = pyautogui.position()
    pyautogui.moveTo(x, y - STEP)

def abajo():
    x, y = pyautogui.position()
    pyautogui.moveTo(x, y + STEP)

print("Esperando comandos del ESP32...")

while True:
    line = ser.readline().decode().strip()
    if not line:
        continue

    print(f"Comando recibido: {line}")

    if line.lower() == "arriba":
        arriba()
    elif line.lower() == "abajo":
        abajo()
    elif line.lower() == "izquierda":
        izquierda()
    elif line.lower() == "derecha":
        derecha()
