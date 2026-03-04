/*
Developers: CorDae Clark, Matthew Smith

Resoures: Chat gpt , Arduino studio, Prosessing IDE
Arduino Project Documentation: https://projecthub.arduino.cc/diegogalvan_1294/building-an-ultrasonic-radar-using-arduino-and-processing-59053e

/*
Developers: CorDae Clark, Matthew Smith

Resources:
ChatGPT, Arduino IDE, Processing IDE
Project Reference:
https://projecthub.arduino.cc/diegogalvan_1294/building-an-ultrasonic-radar-using-arduino-and-processing-59053e
*/

#include <Servo.h>

// --- Pin Definitions ---
const int SERVO_PIN = 11;
const int TRIG_PIN = 8;
const int ECHO_PIN = 9;

// --- LED Pins ---
const int LED1 = 2;  // Farthest distance
const int LED2 = 3;
const int LED3 = 4;
const int LED4 = 5;  // Closest distance

// --- Servo Constants ---
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;
const int ANGLE_STEP = 1;
const int SWEEP_DELAY = 15;

// --- Ultrasonic Constant ---
const float SOUND_SPEED_FACTOR = 58.2;

Servo myServo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  myServo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  sweepAndMeasure(MIN_ANGLE, MAX_ANGLE, ANGLE_STEP);
  sweepAndMeasure(MAX_ANGLE, MIN_ANGLE, -ANGLE_STEP);
}

void sweepAndMeasure(int startAngle, int endAngle, int step) {
  for (int angle = startAngle; 
       (step > 0) ? (angle <= endAngle) : (angle >= endAngle); 
       angle += step) {

    myServo.write(angle);
    delay(SWEEP_DELAY);

    int distance = calculateDistance();
    updateLEDs(distance);
    printData(angle, distance);
  }
}

int calculateDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  int distance = duration / SOUND_SPEED_FACTOR;

  return distance;
}

void updateLEDs(int distance) {

  // Turn all LEDs off first
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  if (distance > 100) {
    // Nothing close
  }
  else if (distance > 75) {
    digitalWrite(LED1, HIGH);
  }
  else if (distance > 50) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }
  else if (distance > 25) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  }
  else {
    // Very close
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }
}

void printData(int angle, int distance) {
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}