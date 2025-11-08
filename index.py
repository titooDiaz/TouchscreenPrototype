import time
import serial
import pyautogui

STEP = 20
PORT = '/dev/ttyUSB0' # ls /dev/tty*
BAUD = 115200

def connect_serial():
    while True:
        try:
            ser = serial.Serial(PORT, BAUD, timeout=0.1)
            time.sleep(2)
            print(f"CONNECT {PORT}")
            return ser
        except serial.SerialException:
            print("NO CONNECT, RELOADING...")
            time.sleep(2)

def mover(direccion):
    x, y = pyautogui.position()
    if direccion == "derecha":
        pyautogui.moveTo(x + STEP, y)
    elif direccion == "izquierda":
        pyautogui.moveTo(x - STEP, y)
    elif direccion == "arriba":
        pyautogui.moveTo(x, y - STEP)
    elif direccion == "abajo":
        pyautogui.moveTo(x, y + STEP)

def main():
    ser = connect_serial()
    print("waiting to ESP32...")

    buffer = ""
    while True:
        try:
            if ser.in_waiting > 0:
                data = ser.read(ser.in_waiting).decode(errors='ignore')
                buffer += data
                if '\n' in buffer:
                    lines = buffer.split('\n')
                    for line in lines[:-1]:
                        line = line.strip()
                        if line:
                            print(f"command: {line}")
                            mover(line.lower())
                    buffer = lines[-1]
        except serial.SerialException:
            print("Lost conection.")
            ser.close()
            ser = connect_serial()
        except Exception as e:
            print("Error:", e)
            time.sleep(1)

if __name__ == "__main__":
    main()
