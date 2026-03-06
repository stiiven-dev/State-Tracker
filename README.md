<h1 align="center">🧓 State Tracker</h1>

<p align="center">
Wearable monitoring system for elderly safety and health tracking
</p>

<p align="center">

![GitHub stars](https://img.shields.io/github/stars/stiiven-dev/State-Tracker?style=for-the-badge)
![GitHub forks](https://img.shields.io/github/forks/stiiven-dev/State-Tracker?style=for-the-badge)
![GitHub issues](https://img.shields.io/github/issues/stiiven-dev/State-Tracker?style=for-the-badge)
![License](https://img.shields.io/github/license/stiiven-dev/State-Tracker?style=for-the-badge)

</p>

---

# 📖 Definition

**State Tracker** is a wearable monitoring device designed to help elderly people who live alone.  
The system continuously monitors biometric and motion data to detect dangerous situations such as falls or abnormal physiological states.

If a risk is detected, the system automatically alerts caregivers or guardians through a communication system.

---

# 🎯 Project Objective

The goal of this project is to:

- Monitor elderly health conditions
- Detect dangerous situations such as falls
- Provide real-time alerts
- Allow caregivers to track the user's state remotely

This system aims to increase the safety and independence of elderly individuals living alone.

---

# ⚠️ Disclaimer

This device is designed for elderly people living alone.

It is **NOT intended for patients with**:

- Dementia
- Alzheimer’s disease
- Urinary disorders

These conditions require continuous medical supervision.

---

# 🧠 System Overview

The system collects **biometric and motion data** using sensors embedded in a wearable device.

The data is processed by a microcontroller and analyzed to detect abnormal situations.

If a dangerous state is detected, the system triggers an alert that is sent to caregivers via communication modules.

---

# 🏗 System Architecture

## Global System Architecture

![System Architecture](./images/system_architecture.png)

### Explanation

1. **Sensors Subsystem**

The wearable device uses sensors to collect user data:

- Accelerometer
- Gyroscope
- Physiological sensors

These sensors capture movement and biometric signals.

---

2. **Embedded Processing Unit**

The microcontroller processes the sensor data through:

- Data acquisition
- Signal processing
- Feature extraction

This step prepares the data for analysis.

---

3. **Detection & Decision Unit**

The system analyzes the processed data to detect:

- Falls
- Abnormal behaviour
- Dangerous thresholds

If the system detects a problem, it triggers an alert.

---

4. **Communication Module**

Once a danger is detected, the system sends notifications through:

- Bluetooth
- GSM
- WiFi

---

5. **Mobile Application**

The mobile application allows users to:

- Visualize alerts
- Monitor the user's state
- View event history
- Synchronize with cloud services

---

6. **Caregiver Notification**

Caregivers receive alerts through:

- Push notifications
- SMS messages

This allows quick response to emergencies.

---

# 🔄 Data Flow

![Data Flow Diagram](./images/data_flow.png)

### Explanation

1. The elderly user wears the monitoring device.

2. Sensors collect raw data including:

- Movement
- Acceleration
- Physiological signals

3. Data preprocessing removes noise and filters signals.

4. The danger detection algorithm analyzes the cleaned data.

5. If the system detects normal activity, monitoring continues.

6. If a dangerous state is detected:

- An alert is generated
- A message is sent to caregivers

---

# 🔧 Hardware Components

| Component | Purpose |
|---------|---------|
| **ESP32 S2** | Main microcontroller with wireless capabilities |
| **MAX30102** | Heart rate and blood oxygen monitoring |
| **MPU6050** | Motion tracking and fall detection |
| **GSM Module** | Sending emergency messages |
| **TVS Protection** | Static discharge protection |

---

# 💻 Technologies Used

- **JavaScript**
- Embedded systems
- IoT sensors
- Mobile communication
- Signal processing

---

Clone the repository:

```bash
git clone https://github.com/stiiven-dev/State-Tracker.git
cd State-Tracker
