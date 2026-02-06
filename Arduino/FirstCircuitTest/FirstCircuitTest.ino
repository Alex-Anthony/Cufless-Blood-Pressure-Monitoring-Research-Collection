
/*
// Define the analog pins for the pressure sensors
const int PressureSensorPin1 = A0;
const int PressureSensorPin2 = A1;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Print a message to the Serial Monitor
  Serial.println("Pressure Sensor Test");
}

void loop() {
  // Read the analog values from the pressure sensors
  int sensorValue1 = analogRead(PressureSensorPin1);
  int sensorValue2 = analogRead(PressureSensorPin2);

  // Get the current time in milliseconds since the Arduino started
  unsigned long currentTime = millis();

  // Print the sensor values and the current time to the Serial Monitor
  Serial.print("Time: ");
  Serial.print(currentTime);
  Serial.print(" ms | Sensor 1: ");
  Serial.print(sensorValue1);
  Serial.print(" | Sensor 2: ");
  Serial.println(sensorValue2);

  // Delay before the next reading
  delay(100);
}
*/

// Define the analog pins for the pressure sensors
const int PressureSensorPin1 = A0;
const int PressureSensorPin2 = A1;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Print a message to the Serial Monitor
  Serial.println("Pressure Sensor Test");
}

void loop() {
  // Read the analog values from the pressure sensors
  int sensorValue1 = analogRead(PressureSensorPin1);
  int sensorValue2 = analogRead(PressureSensorPin2);

  // Print the sensor values to the Serial Plotter
  // Format: "Sensor1:<value> Sensor2:<value>"
  Serial.print("Sensor1:");
  Serial.print(sensorValue1);
  Serial.print(" Sensor2:");
  Serial.println(sensorValue2 - 100);

  // Delay before the next reading
  delay(50);
}
