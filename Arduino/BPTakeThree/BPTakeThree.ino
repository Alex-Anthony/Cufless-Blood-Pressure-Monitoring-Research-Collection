// Include necessary libraries
#include <Arduino.h>

// Define constants
const int sensorPin1 = A0;  // Pin where the first sensor is connected
const int sensorPin2 = A1;  // Pin where the second sensor is connected
const int numSamples = 500;  // Number of samples to take
const int windowSize = 3;  // Window size for moving average
const float alpha = 0.046;  // Sensitivity of BP to changes in PWV
const float beta = 5.1;  // Baseline BP value

// Arrays to store data
float timeData[numSamples];
int pointData1[numSamples];
int pointData2[numSamples];
float movingAvgData1[numSamples];
float movingAvgData2[numSamples];

// Variables to store current time
unsigned long currentTime;
unsigned long startTime;

// Function to calculate moving average
void calculateMovingAverage(int data[], float result[], int size, int window) {
  for (int i = 0; i < size; i++) {
    if (i < window) {
      result[i] = 0;
    } else {
      float sum = 0;
      for (int j = 0; j < window; j++) {
        sum += data[i - j];
      }
      result[i] = sum / window;
    }
  }
}

// Function to detect peaks
float detectPeaks(float data[], int size) {
  float peak = 0;
  for (int i = 1; i < size - 1; i++) {
    if (data[i] > data[i - 1] && data[i] > data[i + 1]) {
      peak = data[i];
    }
  }
  return peak;
}

// Function to calculate Blood Pressure
float calculateBP(float pwv) {
  return alpha * pwv * pwv + beta;
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize time variables
  startTime = millis();
}

void loop() {
  // Collect data
  for (int i = 0; i < numSamples; i++) {
    currentTime = millis() - startTime;
    timeData[i] = currentTime;
    pointData1[i] = analogRead(sensorPin1);
    pointData2[i] = analogRead(sensorPin2);

    // Delay to simulate the sampling rate
    delay(10);  // Adjust delay as needed for your sampling rate
  }

  // Calculate moving average
  calculateMovingAverage(pointData1, movingAvgData1, numSamples, windowSize);
  calculateMovingAverage(pointData2, movingAvgData2, numSamples, windowSize);

  // Detect peaks
  float peak1 = detectPeaks(movingAvgData1, numSamples);
  float peak2 = detectPeaks(movingAvgData2, numSamples);

  // Calculate PWV (Pulse Wave Velocity) - For demonstration purposes
  float pwv = 0.025/abs(peak1 - peak2);  // Simplified calculation for example

  // Calculate Blood Pressure
  float bp = calculateBP(pwv);

  // Print results for debugging
  for (int i = 0; i < numSamples; i++) {
    //Serial.print("Time: ");
    //Serial.print(timeData[i]);
    Serial.print("Sensor1: ");
    Serial.print(pointData1[i]);
    Serial.print(" Sensor2: ");
    Serial.println(pointData2[i]);
   // Serial.print(" Moving Avg1: ");
    //Serial.print(movingAvgData1[i]);
    //Serial.print(" Moving Avg2: ");
   // Serial.println(movingAvgData2[i]);
    //Serial.print(", BP: ");
    //Serial.println(bp);
  }

  // Add a delay before the next set of samples (optional)
  delay(500);  // Adjust delay as needed
}
