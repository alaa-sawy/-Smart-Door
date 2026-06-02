# Secure Access Through Biometric IoT

**Smart Door Security System** using Fingerprint Authentication and IoT Monitoring.

Advanced biometric access control system built with ESP32 that provides secure, fast, and remotely monitored door access.

<img width="720" height="390" alt="image" src="https://github.com/user-attachments/assets/3a946aa5-b789-4d3a-9c4a-0e6195e431df" />
<img width="720" height="1454" alt="image" src="https://github.com/user-attachments/assets/e947e299-3334-4e5c-b7f1-19abb000797b" />




## 🎯 Features

- **Fingerprint Authentication** with >99% accuracy
- **Real-time Mobile Monitoring** via Blynk App
- **Automatic Door Control** using Servo Motor
- **Proximity Detection** using Ultrasonic Sensor
- **LCD Status Display**
- **Visual Feedback** (Green/Red LEDs)
- **Remote Open** from mobile app
- **Access Logging** and notifications

## 🛠️ Technologies Used

- **Microcontroller**: ESP32
- **Fingerprint Sensor**: RC307 (Adafruit Fingerprint Library)
- **IoT Platform**: Blynk
- **Programming Language**: C++
- **Actuator**: SG90 Servo Motor
- **Sensors**: HC-SR04 Ultrasonic, LCD I2C
- **Storage**: ESP32 Preferences (NVS)

## 📁 Project Structure
├── PA.ino              # Main program (Door operation)

├── En2.ino             # Fingerprint Enrollment program

├── Circuit Diagram

├── Presentation.pdf

└── Demo Video

## 🚀 How to Use

1. **Enrollment**:
   - Upload `En2.ino` to ESP32
   - Enter names through Serial Monitor to enroll fingerprints

2. **Main System**:
   - Upload `PA.ino`
   - Connect to WiFi and open Blynk App

3. **Place finger** on sensor → Door opens automatically if authorized

## 📊 Results

- **Accuracy**: 99%
- **Latency**: < 0.5 seconds
- **Stability**: 100%
- **Remote Control**: Fully functional

## 📽️ Demo

https://drive.google.com/file/d/1I1Pr_9p0_PRPcmQNrJ4oZ95ToyG56u-f/view?usp=sharing
