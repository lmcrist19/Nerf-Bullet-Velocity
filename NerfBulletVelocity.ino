/*
  Author: Lauren Crist
  Project: Nerf Bullet Velocity Project
  Date: 12/3/18
  Description: This Arduino code measures the velocity of a Nerf bullet by calculating the time it takes for the bullet to hit a target after a button press. It uses an ultrasonic sensor for distance measurement.
*/

int pushbutton = 4;
int readingPushbutton;
int pushbuttonState;
int lastPushbuttonState = LOW;
unsigned long lastDebPushbutton = 0;

int target = 5;
int readingTarget;
int targetState;
int lastTargetState = LOW;
unsigned long lastDebTarget = 0;
unsigned long debDelay = 20;

int echoPin = 6;
int trigPin = 7;

long duration;
float distance;
float timeChange;
float timeInitial;
float timeFinal;
float velocity;

int i;

void setup() {
  Serial.begin(9600);
  
  pinMode(target, INPUT);
  pinMode(pushbutton, INPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() { 
  // Read button state
  readingTarget = digitalRead(target);
  readingPushbutton = digitalRead(pushbutton);
  
  // Debounce push button to get proper reading in milliseconds
  if (readingPushbutton != lastPushbuttonState) {
    lastDebPushbutton = millis();
  }
  
  if ((millis() - lastDebPushbutton) > debDelay) {
    if (readingPushbutton != pushbuttonState) {
      pushbuttonState = readingPushbutton;
      if (pushbuttonState == HIGH) {
        storeInitialTime();
        calculateDistance();
      }
    }
  }
  lastPushbuttonState = readingPushbutton;
  
  // Debounce target state to get proper reading in milliseconds
  if (readingTarget != lastTargetState) {
    lastDebTarget = millis();
  }
  
  if ((millis() - lastDebTarget) > debDelay) {
    if (readingTarget != targetState) {
      targetState = readingTarget;
      if (targetState == HIGH) {
        storeFinalTime();
        calculateTimeChange();
        calculateSpeed();
      }
    }
  }
  lastTargetState = readingTarget;
}

void calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.00034 / 2;
  Serial.println(String("Distance: ") + distance + String("m"));
}

void calculateTimeChange() {
  timeChange = timeFinal - timeInitial;
  Serial.println(String("Time bullet takes to hit target: ") + timeChange + String("s"));
}

void calculateSpeed() {
  velocity = distance / timeChange;
  Serial.println(String("Speed: ") + velocity + String("m/s"));
  Serial.println("-------------"); 
}

void storeFinalTime() {
  timeFinal = millis() / 1000; // Store time when the ball hits the target
  Serial.println(String("Final time: ") + timeFinal + String("s"));
}

void storeInitialTime() {
  timeInitial = millis() / 1000;  // Store initial time button is hit
  Serial.println(String("Initial time: ") + timeInitial + String("s"));
}
