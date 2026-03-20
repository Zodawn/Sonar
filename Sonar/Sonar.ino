/*
Developers: CorDae Clark, Matthew Smith

Resources: ChatGPT, Arduino Studio, Processing IDE
Arduino Project Documentation:
https://projecthub.arduino.cc/diegogalvan_1294/building-an-ultrasonic-radar-using-arduino-and-processing-59053e
*/
#include <Servo.h>

// --- Pin Definitions ---
const int SERVO_PIN = 11;
const int TRIG_PIN = 8;
const int ECHO_PIN = 9;
const int BUZZER_PIN = 6;

// --- LED Pins ---
const int LED1 = 2; // first
const int LED2 = 3; // last
const int LED3 = 4; // third
const int LED4 = 5; // second

// --- Servo Radar Settings ---
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;
const int ANGLE_STEP = 1;
const int SWEEP_DELAY = 18;

// --- Ultrasonic Constant ---
const float SOUND_SPEED_FACTOR = 58.2;

Servo radarServo;

int angle = 0;
int direction = 1;

void setup() {

  Serial.begin(9600);

  radarServo.attach(SERVO_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  randomSeed(analogRead(0));
}

void loop() {

  radarServo.write(angle);

  int distance = calculateDistance();

  controlLEDs(distance);

  controlBuzzer(distance);

  printData(angle, distance);

  angle += direction * ANGLE_STEP;

  if (angle >= MAX_ANGLE || angle <= MIN_ANGLE) {
    direction = -direction;
  }

  delay(SWEEP_DELAY);
}

// --- Distance Function ---
int calculateDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 200;
  }

  int distance = duration / SOUND_SPEED_FACTOR;

  return distance;
}

// --- LED Control (UPDATED ORDER) ---
void controlLEDs(int distance) {

  // CRAZY MODE when very close
  if (distance < 20) {

    digitalWrite(LED1, random(0,2));
    digitalWrite(LED4, random(0,2));
    digitalWrite(LED3, random(0,2));
    digitalWrite(LED2, random(0,2));

    delay(40);
    return;
  }

  // Turn all OFF first
  digitalWrite(LED1, LOW); // pin 2
  digitalWrite(LED2, LOW); // pin 3
  digitalWrite(LED3, LOW); // pin 4
  digitalWrite(LED4, LOW); // pin 5

  // Sequential order: 2 → 5 → 4 → 3
  if (distance < 80) {
    digitalWrite(LED1, HIGH);
  }
  if (distance < 60) {
    digitalWrite(LED4, HIGH);
  }
  if (distance < 40) {
    digitalWrite(LED3, HIGH);
  }
  if (distance < 20) {
    digitalWrite(LED2, HIGH);
  }
}

// --- Buzzer Control ---
void controlBuzzer(int distance) {

  if (distance < 15) {
    tone(BUZZER_PIN, 1200);
  }
  else if (distance < 30) {
    tone(BUZZER_PIN, 900);
  }
  else {
    noTone(BUZZER_PIN);
  }
}

// --- Send Data to Processing ---
void printData(int angle, int distance) {

  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}
