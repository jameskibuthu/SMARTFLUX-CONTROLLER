# SMARTFLUX-CONTROLLER
Project Overview
SmartFlux Controller is an intelligent, automated energy management system designed for hybrid solar-grid installations in institutional and commercial settings. The system automatically switches between solar and grid power sources based on energy availability, enables bidirectional energy flow for feeding excess solar power back to the grid, and provides real-time monitoring through an LCD display.

This project was developed as a final year engineering project addressing Sustainable Development Goal 7: Affordable and Clean Energy.

Key Features
Automatic Source Switching: Seamlessly switches between solar/battery and grid power based on real-time voltage monitoring

Excess Power Management: Automatically activates grid feed-in when solar panel output exceeds 4V (indicating excess energy)

Dual Relay Control: Two relays manage motor load and grid feed-in independently

Real-time Display: 16x2 LCD shows voltage readings, relay status, and system mode

Energy Tracking: Monitors and logs energy consumption from solar vs grid sources

Hysteresis Protection: Prevents rapid relay switching during voltage fluctuations

Modular Design: Easy to customize thresholds and add additional sensors

System Architecture
text
                    ┌─────────────────────────────────────┐
                    │         SMARTFLUX CONTROLLER        │
                    │                                      │
    Solar Panel ────┤  Voltage Divider ──── Arduino Mega  │
                    │                           │         │
    Battery     ────┤  Voltage Divider         │         │
                    │                           │         │
    LCD Display ────┤  I2C/Parallel            │         │
                    │                           │         │
    Relay 1     ────┤  Digital Pin 4  ─────────┼── Motor │
                    │                           │         │
    Relay 2     ────┤  Digital Pin 3  ─────────┼── Grid Display
                    │                           │         │
                    └─────────────────────────────────────┘
Components Required
Component	Quantity	Specification
Arduino Mega 2560	1	Microcontroller
Solar Panel	1	5V output (or higher with voltage divider)
5V Battery	1	For energy storage
Relay Module	2	5V coil, SPDT, rated for your load
16x2 LCD Display	1	With or without I2C adapter
10kΩ Resistors	4	For voltage dividers
DC Motor	1	Load to demonstrate switching
Grid Display Module	1	60-240V AC power monitor
Connecting Wires	As needed	Jumper wires for breadboard/prototype
Breadboard/PCB	1	For circuit assembly
Fuse	1	3-5A for 240V AC protection
Wiring Instructions
Arduino Mega Connections
text
LCD CONNECTION (Parallel Mode):
  LCD Pin 1 (VSS)  ──── GND
  LCD Pin 2 (VDD)  ──── 5V
  LCD Pin 3 (V0)   ──── 10k Potentiometer to GND
  LCD Pin 4 (RS)   ──── Pin 13
  LCD Pin 5 (RW)   ──── GND
  LCD Pin 6 (E)    ──── Pin 12
  LCD Pin 11 (D4)  ──── Pin 11
  LCD Pin 12 (D5)  ──── Pin 10
  LCD Pin 13 (D6)  ──── Pin 9
  LCD Pin 14 (D7)  ──── Pin 8
  LCD Pin 15 (A)   ──── 5V (backlight)
  LCD Pin 16 (K)   ──── GND (backlight)

VOLTAGE DIVIDER CIRCUITS (for Solar & Battery):
  Solar Panel (+) ──── 10kΩ ──── A0 ──── 10kΩ ──── GND
  Battery (+)     ──── 10kΩ ──── A1 ──── 10kΩ ──── GND

RELAY MODULES:
  Relay 1 IN ──── Pin 4 (Motor Control)
  Relay 2 IN ──── Pin 3 (Grid Feed-in)
  Relay VCC ───── 5V (or external if >5V relay)
  Relay GND ───── GND

RELAY OUTPUT SIDE (Motor Connection):
  Power Supply (+) ──── Relay 1 COM
  Relay 1 NO ────────── Motor (+)
  Motor (-) ─────────── Power Supply GND

RELAY OUTPUT SIDE (Grid Display - ⚠️ HIGH VOLTAGE!):
  240V Live ──── Fuse (3A) ──── Relay 2 COM
  Relay 2 NO ──── Grid Display Live Input
  240V Neutral ──── Grid Display Neutral Input
Software Installation
Prerequisites
Arduino IDE (version 1.8.0 or later)

Required Libraries:

Wire.h (built-in)

LiquidCrystal.h (built-in)

Setup Instructions
Clone the repository

bash
git clone https://github.com/yourusername/smartflux-controller.git
Open the Arduino IDE

Load the sketch

Navigate to smartflux_controller.ino in the cloned repository

Open the file in Arduino IDE

Select your board

Tools → Board → Arduino Mega 2560

Select the correct port

Tools → Port → COMx (Windows) or /dev/ttyUSBx (Linux/Mac)

Upload the code

Click the Upload button or press Ctrl+U

Code Structure
text
smartflux_controller.ino
├── Library Includes
├── Pin Definitions
├── Threshold Constants
├── setup() - Initialization
├── loop() - Main Program
├── readSolarVoltage() - Voltage reading function
├── controlRelays() - Relay logic
├── updateDisplay() - LCD output
└── calibrateVoltage() - Calibration helper
Key Parameters to Customize
cpp
// Adjust these values at the top of the code
const float EXCESS_VOLTAGE = 4.0;    // Voltage threshold for grid feed-in
const float MIN_VOLTAGE = 2.5;        // Minimum voltage to run motor
const float HYSTERESIS = 0.2;          // Prevents rapid switching
const float VOLTAGE_DIVIDER_RATIO = 6.0; // Adjust based on your divider circuit
How It Works
Operating Logic
Solar Voltage	Relay 1 (Motor)	Relay 2 (Grid)	System State
< 2.5V	OFF	OFF	Insufficient solar - system off
2.5V - 4.0V	ON	OFF	Motor running on solar power
> 4.0V	ON	ON	Motor running + Excess to grid
Voltage Reading Formula
cpp
// Converts analog reading (0-1023) to actual voltage
float voltage = (analogRead(pin) * 5.0 / 1024.0) * VOLTAGE_DIVIDER_RATIO;
Testing Procedure
Phase 1: Low-Voltage Testing
Disconnect all high-voltage components

Use a variable power supply (0-5V) to simulate solar panel

Connect LEDs to relay outputs for visual feedback

Verify switching at thresholds:

0-2.4V: Both relays OFF

2.5-3.9V: Relay 1 ON, Relay 2 OFF

4.0-5.0V: Both relays ON

Phase 2: Motor Test
Connect a small DC motor to Relay 1 output

Verify motor runs when voltage > 2.5V

Verify motor continues running when grid feed activates (>4V)

Phase 3: Grid Display Integration (⚠️ SAFETY FIRST!)
Double-check all 240V wiring

Ensure proper fusing on live line

Test with no load initially

Verify display activates only when voltage > 4V

Safety Warnings
IMPORTANT: This project involves 240V AC power!

⚡ Always disconnect AC power before making wiring changes

⚡ Use proper fusing (3-5A) on the AC live line

⚡ Ensure relay modules are rated for AC voltage/current

⚡ Keep low-voltage and high-voltage wiring separated

⚡ Use appropriate wire gauge for current requirements

⚡ Enclose all components in a non-conductive housing

⚡ Have your wiring inspected by a qualified electrician

⚡ Never work on live circuits

Troubleshooting
Problem	Possible Cause	Solution
Relay not clicking	Low power to relay	Check 5V supply to relay module
LCD not displaying	Contrast wrong	Adjust 10k potentiometer
Voltage readings inaccurate	Wrong divider ratio	Recalculate based on multimeter
Rapid relay switching	No hysteresis	Ensure HYSTERESIS > 0.1
Breaker trips when relay activates	Short circuit	Check 240V wiring, ensure proper isolation
Future Enhancements
Add current sensors for accurate energy measurement

Implement IoT connectivity for remote monitoring

Add SD card logging for long-term data collection

Integrate with mobile app via Bluetooth/WiFi

Add support for multiple solar panels

Implement MPPT charge controller functionality

Add overcurrent and short-circuit protection

 References
IEA. (2023). Renewable Energy Market Update

Bhattacharya, M. et al. (2018). Microcontroller-based energy monitoring systems

Zhang, J. (2019). Smart Grids: Challenges and Opportunities

ADE7757 Datasheet, Analog Devices (2021)

License
This project is licensed under the MIT License - see the LICENSE file for details.

Authors
JAMES MUCHERU 

Acknowledgments
Project Supervisor
JAMES KIBUTHU
Institution Name

Department of Electrical/Computer Engineering

All contributors and testers
