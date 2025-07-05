# Smart Medibox 💊

An IoT-based medication management system that assists users in managing their medication schedules effectively while monitoring environmental conditions for optimal medicine storage.

## 🎯 Project Overview

The Smart Medibox is an embedded systems project developed as part of the EN2853: Embedded Systems and Applications course. This device combines medication scheduling, environmental monitoring, and automated storage condition management to ensure medicines are stored and taken properly.

## ✨ Features

### Core Functionality 
- **Time Management**: Fetch current time from NTP server with timezone configuration
- **Alarm System**: Set up to 2 alarms with snooze functionality
- **Menu Navigation**: Interactive menu system for device configuration
- **Environmental Monitoring**: Temperature and humidity monitoring with health warnings
- **Visual Display**: OLED display for time, menu, and system status

### Advanced Features 
- **Light Intensity Monitoring**: Real-time light level measurement using LDR sensor
- **Automated Shading System**: Servo-controlled sliding window for optimal light conditions
- **IoT Dashboard**: Node-RED based web dashboard for remote monitoring and control
- **MQTT Communication**: Real-time data transmission to cloud dashboard
- **Configurable Parameters**: User-adjustable sampling rates and control parameters

## 🔧 Hardware Components

- **ESP32 Development Board**: Main microcontroller
- **OLED Display**: For local interface and information display
- **DHT11 Sensor**: Temperature and humidity measurement
- **LDR (Light Dependent Resistor)**: Light intensity monitoring
- **Servo Motor**: Automated shading window control
- **Push Button**: User interaction and alarm control
- **Buzzer**: Audio notifications for alarms and warnings
- **LED**: Visual status indicators

## 📐 System Architecture
### Hardware Layer
```mermaid
graph TB
    ESP32[ESP32 Microcontroller]
    
    DHT11[DHT11 Sensor - Temperature and Humidity]
    LDR[LDR Sensor - Light Intensity 0-1]
    Button[Push Button - Menu Control and Alarm Stop]
    
    Servo[Servo Motor - Shading Window 0-180 degrees]
    OLED[OLED Display 128x64 - Time and Status Display]
    Buzzer[Buzzer and LED - Alarm and Warning Indicators]
    
    DHT11 --> ESP32
    LDR --> ESP32
    Button --> ESP32
    
    ESP32 --> Servo
    ESP32 --> OLED
    ESP32 --> Buzzer
```

### Communication Layer
```mermaid
graph TD
    ESP32[ESP32 Device]
    WiFi[WiFi Network]
    MQTT[MQTT Broker - test.mosquitto.org]
    NTP[NTP Server - Time Synchronization]
    NodeRED[Node-RED Dashboard]
    
    ESP32 --> WiFi
    WiFi --> MQTT
    WiFi --> NTP
    
    MQTT --> NodeRED
    ESP32 --> MQTT
    ESP32 --> NTP
    
    MQTT --> Topic1[medibox/light/data]
    MQTT --> Topic2[medibox/control/params]
    MQTT --> Topic3[medibox/servo/angle]
```

### Data Flow Architecture
```mermaid
graph TD
    Start([System Start])
    
    LDR_Read[LDR Reading Every 5 seconds]
    DHT_Read[DHT11 Reading Temperature and Humidity]
    
    LDR_Sample[5s Sampling]
    LDR_Avg[2min Average]
    Health_Check[Health Check Temp 24-32C Humidity 65-80 percent]
    
    Control_Algo[Servo Control Algorithm]
    
    MQTT_Pub[MQTT Publish Light Data]
    Servo_Control[Servo Motor Control Window Position]
    Alarm_Warning[Alarm Warning Notifications]
    
    Dashboard_Input[Dashboard Input Parameter Changes]
    Param_Update[Parameter Update theta_offset gamma T_med]
    
    Start --> LDR_Read
    Start --> DHT_Read
    
    LDR_Read --> LDR_Sample
    LDR_Sample --> LDR_Avg
    LDR_Avg --> MQTT_Pub
    
    DHT_Read --> Health_Check
    Health_Check --> Alarm_Warning
    
    LDR_Avg --> Control_Algo
    DHT_Read --> Control_Algo
    Control_Algo --> Servo_Control
    
    Dashboard_Input --> Param_Update
    Param_Update --> Control_Algo
```

### Software Architecture
```mermaid
graph TB
    subgraph App[Application Layer]
        AlarmMgr[Alarm Manager - Schedule and Notifications]
        MenuSys[Menu System - Navigation and User Input]
        ControlLogic[Control Logic - Servo Control and Parameters]
    end
    
    subgraph Comm[Communication Layer]
        MQTTClient[MQTT Client - Pub Sub and JSON Messages]
        WiFiMgr[WiFi Manager - Connection and Reconnection]
        NTPClient[NTP Client - Time Sync and Timezone]
    end
    
    subgraph HAL[Hardware Abstraction Layer]
        SensorDrivers[Sensor Drivers - DHT11 and LDR ADC]
        DisplayDriver[Display Driver - OLED SSD1306 and Graphics]
        ActuatorControl[Actuator Control - Servo PWM and Buzzer Tone]
    end
    
    AlarmMgr --> MQTTClient
    MenuSys --> DisplayDriver
    ControlLogic --> ActuatorControl
    
    MQTTClient --> WiFiMgr
    WiFiMgr --> NTPClient
    
    SensorDrivers --> ControlLogic
    DisplayDriver --> MenuSys
    ActuatorControl --> ControlLogic
```


## 🧮 Key Algorithms

### Servo Motor Control Algorithm
The shaded sliding window position is calculated using the following equation:

$$
\theta = \theta_{\text{offset}} + (180 - \theta_{\text{offset}}) \times I \times \gamma \times \ln\left(\frac{t_s}{t_u}\right) \times \left(\frac{T}{T_{\text{med}}}\right)
$$

### Where:
- $\theta$ : Motor angle (0–180°)  
- $\theta_{\text{offset}}$ : Minimum angle (default: 30°)  
- $I$ : Light intensity (range: 0–1)  
- $\gamma$ : Controlling factor (default: 0.75)  
- $t_s$ : Sampling interval (seconds)  
- $t_u$ : Sending interval (seconds)  
- $T$ : Measured temperature (°C)  
- $T_{\text{med}}$ : Ideal storage temperature (default: 30°C)

### Data Processing
- **LDR Sampling**: Every 5 seconds (configurable)
- **Data Averaging**: 2-minute rolling average (configurable)
- **Health Monitoring**: 
  - Temperature: 24°C ≤ T ≤ 32°C
  - Humidity: 65% ≤ H ≤ 80%

## 🌐 Dashboard Features

### Light Monitoring Group
- Real-time light intensity gauge (0-1 scale)
- Historical data visualization chart
- Configurable sampling interval slider
- Configurable sending interval slider

### Environmental Control Group
- Minimum angle adjustment (0-120°)
- Controlling factor adjustment (0-1)
- Ideal storage temperature setting (10-40°C)


**Pankaja Balasooriya**  
Index No: 220054N
EN2853: Embedded Systems and Applications  
University of Moratuwa

## 📞 Contact

For questions or suggestions, please contact balasooriyabapi.22@uom.lk.

---
