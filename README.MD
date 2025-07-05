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

## 🚀 Getting Started

### Prerequisites
- Arduino IDE with ESP32 board support
- Node-RED installation
- MQTT broker access (test.mosquitto.org)
- Required Arduino libraries:
  - WiFi
  - NTPClient
  - DHT sensor library
  - Servo library
  - SSD1306 (OLED display)
  - PubSubClient (MQTT)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/smart-medibox.git
   cd smart-medibox
   ```

2. **Hardware Setup**
   - Connect components according to the circuit diagram
   - Update pin configurations in the code if needed

3. **Software Configuration**
   - Update WiFi credentials in the Arduino code
   - Configure MQTT broker settings
   - Import Node-RED flow from the provided JSON file

4. **Upload and Run**
   - Upload the Arduino sketch to ESP32
   - Deploy the Node-RED flow
   - Access the dashboard through Node-RED interface

## 📁 Project Structure

```
smart-medibox/
├── arduino/
│   └── medibox_main.ino          # Main Arduino sketch
├── node-red/
│   └── dashboard_flow.json       # Node-RED flow configuration
├── documentation/
│   ├── circuit_diagram.png       # Hardware connection diagram
│   └── demo_video.mp4            # Project demonstration
├── README.md                     # This file
└── LICENSE                       # Project license
```

## 🎥 Demo Video

A comprehensive demonstration video is included showing:
- System setup and configuration
- Real-time monitoring capabilities
- Dashboard interaction
- Code walkthrough and explanation

## 🏆 Assessment Criteria

This project was evaluated based on:
- **Technical Implementation** (80%): Sensor integration, MQTT communication, servo control
- **User Interface** (10%): Dashboard design and functionality
- **Code Quality** (10%): Documentation, structure, and creativity

## 🔮 Future Enhancements

- Mobile app integration
- Multiple medication compartments
- Voice notifications
- Cloud data storage and analytics
- Integration with healthcare providers
- Advanced scheduling algorithms

## 🤝 Contributing

This project was developed as part of an academic assignment. Feel free to fork and enhance the functionality for your own projects.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**John Doe**  
Index No: XXXXX  
EN2853: Embedded Systems and Applications  
University of Moratuwa

## 📞 Contact

For questions or suggestions, please open an issue in this repository or contact [your-email@example.com].

---

*This project demonstrates the integration of embedded systems, IoT technologies, and user interface design to create a practical healthcare solution.*