# Ultrasonic Distance Sensor with I2C LCD (8051 + Arduino ISP)

This project measures distance using an ultrasonic sensor (HC-SR04) and displays it on an I2C-based 16x2 LCD, using an 8051 microcontroller.

---

## 🛠️ Components Used

- AT89C51 (8051 MCU)
- HC-SR04 Ultrasonic Sensor
- I2C 16x2 LCD
- Arduino UNO (as ISP programmer)
- avrdude (to upload .hex to 8051)
- 11.0592 MHz Crystal Oscillator

---

## 💻 How I Used Arduino as ISP with avrdude

### 🔌 Setup

1. **Upload ArduinoISP Sketch** to Arduino UNO via Arduino IDE:

2. **Connect AT89C51 to Arduino UNO:**

| Arduino Pin | 8051 Pin |
|-------------|-----------|
| D10         | RESET     |
| D11         | MOSI      |
| D12         | MISO      |
| D13         | SCK       |
| GND         | GND       |
| 5V          | VCC       |

3. **Compile your Keil `.c` code** into a `.hex` file using Keil or SDCC.

### 📤 Upload via `avrdude`

Use `avrdude` with this command:

```bash
avrdude -c arduino -p at89s51 -P COM3 -b 19200 -U flash:w:main.hex
