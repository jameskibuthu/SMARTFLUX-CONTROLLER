/*
 * SMARTFLUX CONTROLLER - Final Version
 * Controls:
 * - Relay 1 (pin 4): Powers the motor (load) when solar is available
 * - Relay 2 (pin 3): Activates grid feed-in when excess solar (>4V)
 * 
 * LCD shows:
 * - Solar voltage
 * - Relay status
 * - System mode
 */

#include <Wire.h>
#include <LiquidCrystal.h>

// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Pin Definitions
const int solarPin = A0;      // Solar panel voltage input
const int relay1 = 4;          // Relay 1 - Powers the motor (load)
const int relay2 = 3;          // Relay 2 - Grid feed-in (excess power)

// Voltage thresholds
const float EXCESS_VOLTAGE = 4.0;    // When solar exceeds this, activate grid feed-in
const float MIN_VOLTAGE = 2.5;        // Minimum voltage to run motor
const float HYSTERESIS = 0.2;          // Prevents rapid switching

// Variables
float solarVoltage = 0;
bool motorRunning = false;
bool gridFeedActive = false;

// For display timing
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_INTERVAL = 300;

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  
  // Set relay pins as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  
  // Start with both relays OFF (assuming LOW = OFF, check your relay module)
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  
  // Initialize LCD
  lcd.begin(16, 2);
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("SmartFlux Ready");
  lcd.setCursor(0, 1);
  lcd.print("Solar Controller");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Solar Voltage:");
}

void loop() {
  // Read solar voltage (averaged for stability)
  solarVoltage = readSolarVoltage();
  
  // Control relays based on solar voltage
  controlRelays();
  
  // Update display (but not too fast)
  if (millis() - lastDisplayUpdate >= DISPLAY_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  // Small delay for stability
  delay(100);
}

float readSolarVoltage() {
  // Take multiple readings and average them for accuracy
  const int numReadings = 10;
  long total = 0;
  
  for (int i = 0; i < numReadings; i++) {
    total += analogRead(solarPin);
    delay(10);
  }
  
  int averageReading = total / numReadings;
  
  // Convert to voltage
  // Formula from your code: (reading * 5.0 / 1024.0) * 6
  // The *6 factor is because you're using a voltage divider
  float voltage = (averageReading * 5.0 / 1024.0) * 6.0;
  
  return voltage;
}

void controlRelays() {
  // RELAY 1 CONTROL - Motor/Load Power
  // Motor runs when solar voltage is high enough
  if (solarVoltage >= MIN_VOLTAGE && !motorRunning) {
    // Turn on motor (Relay 1)
    digitalWrite(relay1, HIGH);
    motorRunning = true;
    Serial.println("Motor ON - Solar power available");
  }
  else if (solarVoltage < (MIN_VOLTAGE - HYSTERESIS) && motorRunning) {
    // Turn off motor if solar too low (with hysteresis to prevent bouncing)
    digitalWrite(relay1, LOW);
    motorRunning = false;
    Serial.println("Motor OFF - Solar too low");
  }
  
  // RELAY 2 CONTROL - Grid Feed-in (Excess Power)
  // This is what you specifically asked for:
  // "When solar reaches about 4V, relay 2 is activated assuming excess energy goes to grid"
  if (solarVoltage >= EXCESS_VOLTAGE && !gridFeedActive) {
    // Excess solar available - activate grid feed-in (Relay 2)
    digitalWrite(relay2, HIGH);
    gridFeedActive = true;
    Serial.println("EXCESS POWER - Feeding to grid");
  }
  else if (solarVoltage < (EXCESS_VOLTAGE - HYSTERESIS) && gridFeedActive) {
    // No more excess - turn off grid feed
    digitalWrite(relay2, LOW);
    gridFeedActive = false;
    Serial.println("Grid feed OFF");
  }
  
  // Note: Relay 1 (motor) continues running even when Relay 2 is active
  // This is exactly what you wanted - "load continues to run"
}

void updateDisplay() {
  lcd.clear();
  
  // Line 0: Solar Voltage
  lcd.setCursor(0, 0);
  lcd.print("Solar:");
  lcd.print(solarVoltage, 2);
  lcd.print("V");
  
  // Show voltage status indicator
  lcd.setCursor(12, 0);
  if (solarVoltage >= EXCESS_VOLTAGE) {
    lcd.print("EXCESS");
  } else if (solarVoltage >= MIN_VOLTAGE) {
    lcd.print("GOOD");
  } else {
    lcd.print("LOW");
  }
  
  // Line 1: Relay Status
  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.print(digitalRead(relay1) ? "ON " : "OFF");
  
  lcd.setCursor(6, 1);
  lcd.print("G:");
  lcd.print(digitalRead(relay2) ? "FEED" : "IDLE");
  
  // Show system mode
  lcd.setCursor(12, 1);
  if (digitalRead(relay1) == HIGH && digitalRead(relay2) == HIGH) {
    lcd.print("S+G");  // Solar powering motor + Excess to grid
  } else if (digitalRead(relay1) == HIGH && digitalRead(relay2) == LOW) {
    lcd.print("SOLAR"); // Solar only
  } else {
    lcd.print("OFF");   // System off
  }
}

// Optional: Function to calibrate your voltage reading
void calibrateVoltage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration");
  lcd.setCursor(0, 1);
  lcd.print("Use multimeter");
  delay(3000);
  
  // Read current value
  float rawVoltage = readSolarVoltage();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Raw:");
  lcd.print(rawVoltage, 2);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Actual: __._V");
  
  // Here you would manually enter the actual voltage
  // Then calculate correction factor
}
