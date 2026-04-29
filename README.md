# Arduino Color Detector using TCS3200

## 1. Project Overview
This project implements a color detection system using an Arduino and the TCS3200 color sensor. The system measures the color of an object by analyzing reflected light and converting it into RGB values.

The detected color is displayed on an I2C LCD and reproduced using an RGB LED. The system includes calibration and normalization techniques to improve accuracy under varying lighting conditions.

---

## 2. Objectives
- Convert physical light signals into digital RGB values
- Calibrate sensor readings using reference values
- Reduce the effect of ambient light through normalization
- Display results clearly on an LCD
- Reproduce detected colors using an RGB LED

---

## 3. System Description
The system operates as follows:
1. An object is placed in front of the sensor
2. The user presses a button to start the measurement
3. The TCS3200 sensor outputs a frequency proportional to color intensity
4. The Arduino reads the signal using pulse timing
5. Raw values are calibrated and normalized
6. The final RGB values are computed
7. The color is identified and displayed
8. The RGB LED reproduces the detected color

---

## 4. Project Structure
Arduino-Color-Detector/
│
├── arduino_color_detector.ino
├── README.md
│
└── components/
├── tcs3200_sensor_only.ino
├── rgb_led_only.ino
├── lcd_i2c_only.ino
└── button_only.ino


---

## 5. Components Folder (Important)

The `components/` folder contains independent modules for each part of the system.

This folder is provided for two main purposes:

1. Troubleshooting  
   If the final integrated system (`arduino_color_detector.ino`) does not work correctly, each component can be tested separately to identify the issue.

2. Reusability  
   Users who do not need the full system can use only the required part. For example:
   - Use only the sensor code
   - Use only the RGB LED control
   - Use only the LCD display
   - Use only the button input

This makes the project modular and easier to debug and reuse.

---

## 6. Color Calibration and Normalization

Raw sensor values are converted using the following formula:
RGB = 255 × (Raw - Black) / (White - Black)

This ensures consistent and accurate measurements across different lighting conditions.

---

## 7. Hardware Components
- Arduino Mega or Arduino Uno
- TCS3200 Color Sensor
- RGB LED
- LCD I2C (16x2)
- Push button
- Resistors (220Ω recommended)
- Jumper wires

---

## 8. Pin Configuration

| Component   | Arduino Pin |
|------------|------------|
| S0         | 2          |
| S1         | 3          |
| S2         | 4          |
| S3         | 5          |
| OUT        | 6          |
| RED LED    | 9          |
| GREEN LED  | 10         |
| BLUE LED   | 11         |
| BUTTON     | A0         |

---

## 9. Software Requirements
- Arduino IDE
- Required libraries:
  - Wire.h
  - LiquidCrystal_I2C.h
  - math.h

---

## 10. Installation and Usage
1. Connect the hardware components according to the pin configuration
2. Open the Arduino IDE
3. Install the required libraries
4. Upload `arduino_color_detector.ino`
5. Power the system
6. Press the button to perform a color scan

---

## 11. Output
- LCD displays the detected color name and RGB values
- RGB LED reproduces the detected color

---

## 12. Supported Colors
- Red
- Green
- Blue
- Yellow
- Pink
- White
- Black
- Gray

---

## 13. Limitations
- Sensitive to ambient lighting conditions
- Calibration must be adjusted depending on the environment
- Difficulty distinguishing similar colors (e.g., yellow vs orange)
- Measurement depends on distance between object and sensor

---

## 14. Possible Improvements
- Add a controlled light source
- Implement automatic calibration
- Use a higher-precision sensor
- Improve filtering and noise handling
- Add advanced color classification

---

## 15. Author
Computer and Communications Engineering Student  
Embedded Systems Project

---

## 16. License
This project is intended for educational purposes.
