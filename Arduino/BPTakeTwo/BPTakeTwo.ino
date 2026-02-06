#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <math.h>
#include "wavemin.h"

// Define OLED display parameters (if using an OLED display for output)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin for sensor input
const int sensorPin = A0;

// Variables for storing sensor data
const int numReadings = 300; // Number of readings for 15-20 seconds at 9600 bits/sec
int readings[numReadings];
int readIndex = 0;
long total = 0;
int average = 0;

// Variables for peak detection
int peakThreshold = 100; // Set an initial threshold for peak detection

// Initialize wavelet objects for wavelet transforms
wave_object obj;
wt_object wt;
char *name = "db4";
const int waveletLevel = 3; // Set wavelet transform level

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Initialize wavelet transform
  obj = wave_init(name);
  wt = wt_init(obj, "dwt", numReadings, waveletLevel);
  setDWTExtension(wt, "sym");
  setWTConv(wt, "direct");

  // Initialize readings array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // Read data from sensor
  int sensorValue = analogRead(sensorPin);

  // Update readings array
  total -= readings[readIndex];
  readings[readIndex] = sensorValue;
  total += readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  average = total / numReadings;

  // Test different peak detection methods
  detectPeakSimpleThreshold(sensorValue);
  detectPeakMovingAverage(sensorValue);
  detectPeakDerivative(sensorValue);
  detectPeakAdaptiveThreshold(sensorValue);
  detectPeakWaveletTransform(sensorValue);

  // Calculate BP (placeholder for actual calculation)
  int systolicBP = calculateSystolicBP();
  int diastolicBP = calculateDiastolicBP();

  // Output BP values to the display
  displayBP(systolicBP, diastolicBP);

  delay(10); // Delay to simulate 9600 bits/sec sampling rate
}

void displayPeak(int peakValue) {
  // Display peak value on OLED (for debugging)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Peak: ");
  display.println(peakValue);
  display.display();
}

int calculateSystolicBP() {
  // Placeholder function for calculating systolic BP
  return 120; // Example value
}

int calculateDiastolicBP() {
  // Placeholder function for calculating diastolic BP
  return 80; // Example value
}

// FUNCTIONS

void detectPeakSimpleThreshold(int sensorValue) {
  static int lastPeakValue = 0;
  if (sensorValue > peakThreshold && lastPeakValue <= peakThreshold) {
    // A peak is detected
    displayPeak(sensorValue);
  }
  lastPeakValue = sensorValue;
}

void detectPeakMovingAverage(int sensorValue) {
  static int windowSize = 5;
  static int window[5] = {0};
  static int sum = 0;
  static int index = 0;

  sum -= window[index];
  window[index] = sensorValue;
  sum += window[index];
  index = (index + 1) % windowSize;

  int average = sum / windowSize;

  if (average > peakThreshold) {
    displayPeak(average);
  }
}

void detectPeakDerivative(int sensorValue) {
  static int lastValue = 0;
  static int lastDerivative = 0;

  int derivative = sensorValue - lastValue;
  if (derivative < 0 && lastDerivative > 0) {
    // A peak is detected
    displayPeak(sensorValue);
  }
  lastDerivative = derivative;
  lastValue = sensorValue;
}

void detectPeakAdaptiveThreshold(int sensorValue) {
  static int adaptiveThreshold = 100;
  static int minThreshold = 80;
  static int maxThreshold = 120;

  if (sensorValue > adaptiveThreshold) {
    displayPeak(sensorValue);
    adaptiveThreshold = min(sensorValue - 20, maxThreshold);
  } else {
    adaptiveThreshold = max(sensorValue + 20, minThreshold);
  }
}

void detectPeakWaveletTransform(int sensorValue) {
  // Shift readings and add new sensor value
  for (int i = numReadings - 1; i > 0; i--) {
    readings[i] = readings[i - 1];
  }
  readings[0] = sensorValue;

  // Perform wavelet transform
  dwt(wt, readings);

  // Detect peaks in wavelet-transformed data
  int lastPeakIndex = -1;
  for (int i = 1; i < wt->outlength - 1; i++) {
    if (wt->output[i] > peakThreshold && wt->output[i] > wt->output[i - 1] && wt->output[i] > wt->output[i + 1]) {
      lastPeakIndex = i;
      displayPeak(wt->output[i]);
    }
  }
}

// DISPLAY SETTINGS
void displayBP(int systolic, int diastolic) {
  // Display BP values on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("BP: ");
  display.print(systolic);
  display.print("/");
  display.println(diastolic);
  display.display();
}
