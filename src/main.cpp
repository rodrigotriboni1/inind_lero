#include <Arduino.h>

// Constants for pin assignments and readings
const int ldrPin = A0;         // LDR pin
const int ledPin = 9;          // LED pin
const int trigPin = 6;       // HC-SR04 Trigger pin
const int echoPin = 11;       // HC-SR04 Echo pin
const int numReadings = 10;
const unsigned long readInterval = 500;

// Global variables
int ldrReadings[numReadings];  // Array for LDR readings
int readIndex = 0;
long sum = 0;
unsigned long lastReadTime = 0;

// Function to initialize the LDR readings array
void initializeLDR() {
for (int thisReading = 0; thisReading < numReadings; thisReading++) {
ldrReadings[thisReading] = 0;
}
}

// Function to read and filter the LDR value using a moving average
int getFilteredLDRReading() {
int sensorValue = analogRead(ldrPin);

sum -= ldrReadings[readIndex];
ldrReadings[readIndex] = sensorValue;
sum += ldrReadings[readIndex];
readIndex = (readIndex + 1) % numReadings;

return sum / numReadings;
}

// // Function to set the LED brightness based on LDR reading
// void setLedBrightness(int ldrValue) {
// // Corrected mapping: brighter LED with lower LDR values (darker)
// float floatLedValue = map(ldrValue, 0, 1024, 255, 0);
// int ledValue = static_cast<int>(floatLedValue * floatLedValue / 255.0);
// analogWrite(ledPin, ledValue);
// }

// Function to read the HC-SR04 sensor and return the distance in cm
int getDistance() {
// Clear the trigger pin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Set the trigger pin HIGH for 10 microseconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Read the echo pin pulse duration
long duration = pulseIn(echoPin, HIGH);

// Calculate the distance in centimeters
// Sound speed = 343 m/s or 0.0343 cm/µs.  Divide by 2 for round trip.
return duration * 0.0343 / 2;
}

// void setLedBrightnessCombined(int ldrValue, int distance) {
//   // Map distance to LED brightness (0-18cm -> 0-255)
//   int distanceBrightness = map(distance, 0, 18, 0, 255);

//   // Combine LDR and distance brightness (you can adjust the weighting)
//   float combinedBrightness = (ldrValue / 1024.0) * 255 * (distanceBrightness / 255.0); // Brighter with higher distance and lower LDR reading(darker)

//     // Apply non-linear mapping for better visual perception (optional)
//   int ledValue = static_cast<int>(combinedBrightness);  // Linear mapping applied
//   analogWrite(ledPin, ledValue);

// }

// Function to set LED brightness based on distance with a limiter and maximum brightness
void setLedBrightnessByDistance(int distance) {
  int ledValue;

  // if (distance == 0) {
  //   ledValue = 0;  // LED off at 0cm
  // } else if 
  if (distance <= 20) {
    ledValue = map(distance, 0, 20, 0, 255);  // Map distance to brightness up to 20cm
  } else {
    ledValue = 255;  // Maximum brightness at 20cm or greater
  }

  analogWrite(ledPin, ledValue);
}

void setup() {
Serial.begin(9600);
pinMode(ledPin, OUTPUT);
pinMode(trigPin, OUTPUT);  // Set trigger pin as output
pinMode(echoPin, INPUT);   // Set echo pin as input
initializeLDR();
}

void loop() {
 unsigned long currentMillis = millis();

  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;

    int ldrReading = getFilteredLDRReading();
    int distance = getDistance();

    setLedBrightnessByDistance(distance);

    // Print values for debugging 
    Serial.print("LDR Reading: ");
    Serial.print(ldrReading);
    Serial.print("\tDistance: ");
    Serial.print(distance);
    Serial.print("\tLED Value (Distance-based): ");
    Serial.println(distance <=0 ? 0 : (distance <20 ? map(distance,0,20,0,255) : 255) ); // Print according to conditional logic



  }
}