#include <Servo.h>

Servo head;

const int sonar_trigger = 2 , sonar_echo = 3 ;
const int servo_pin  = 11;
const int left_motor_in[2]  = {6, 7};
const int right_motor_in[2] = {8, 9};
const int led_pin = 13;
const int enA = 4;
const int enB = 5;

const int FORWARD = 1 , BACKWARD = 2 , RIGHT  = 3 , LEFT  = 4 , BRAKE = 5;

const int maxLowDistance = 20;
const int maxSpeed = 150;

int right, left, front;
int bluetooth_Received_Data;

void setup() {
  //init motor pins
  for (int i = 0 ; i < 2 ; ++i) {
    pinMode(left_motor_in[i], OUTPUT);
    pinMode(right_motor_in[i], OUTPUT);
  }

  //low motor pins
  for (int i = 0 ; i < 2 ; ++i) {
    digitalWrite(left_motor_in[i], LOW);
    digitalWrite(right_motor_in[i], LOW);
  }

  //motor driver: enable pin to control speed
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  // set the speed
  analogWrite(enA, maxSpeed);
  analogWrite(enB, maxSpeed);

  //init led
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  //servo init
  head.attach(servo_pin);
  head.write(90);

  //sonar init
  pinMode(sonar_trigger, OUTPUT);
  pinMode(sonar_echo, INPUT);

  Serial.begin(9600);
}

void loop() {
  /*if(Serial.available() > 0){
    bluetooth_Received_Data = Serial.read();
    }

    if(bluetooth_Received_Data == '0'){
    Serial.println("STOP");
    move(BRAKE);
    } else if (bluetooth_Received_Data == '1'){
    Serial.println("START");
  */
  if (Serial.available() > 0)
  {
    bluetooth_Received_Data = Serial.read();
    Serial.print(bluetooth_Received_Data);
    Serial.print("\n");
  }
  if (bluetooth_Received_Data == 191) {
  
    int dis  = distance();

    if (dis > maxLowDistance) {
      digitalWrite(led_pin, LOW);
      move(FORWARD);
      delay(300);
    }
    else {
      move(BRAKE);
      digitalWrite(led_pin, HIGH);
      move(BACKWARD);
      delay(300);
      move(BRAKE);

      rotateHead();

      if (right > maxLowDistance) {
        move(RIGHT);
        delay(300);
        move(FORWARD);
      } else if (left > maxLowDistance) {
        move(LEFT);
        delay(300);
        move(FORWARD);
      } else {
        move(BACKWARD);
        delay(300);
        move(BRAKE);
      }
    }
  }
  else if (bluetooth_Received_Data == 189)
    move(BRAKE);

  //}
}

void rotateHead() {
  int right, left, front;
  head.write(45);
  delay(500);
  left = distance();
  head.write(90);
  delay(500);
  front = distance();
  head.write(135);
  delay(500);
  right = distance();
  head.write(90);
}
/*void rotateHead(int _direction) {
  if (_direction == LEFT) {
    head.write(0);
    delay(1000);
  }
  else if (_direction == RIGHT) {
    head.write(180);
    delay(1000);
  }
  else if (_direction == FORWARD) {
    head.write(90);
    delay(1000);
  }
  }
*/
int  distance() {

  digitalWrite(sonar_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_trigger, LOW);

  return (int)((pulseIn(sonar_echo, HIGH)) * 0.034 / 2);
}

void move(int  _direction) {

  if (_direction == FORWARD) {
    digitalWrite(left_motor_in[0], HIGH);
    digitalWrite(left_motor_in[1], LOW);
    digitalWrite(right_motor_in[0], HIGH);
    digitalWrite(right_motor_in[1], LOW);
  }
  else if (_direction == BACKWARD) {
    digitalWrite(left_motor_in[0], LOW);
    digitalWrite(left_motor_in[1], HIGH);
    digitalWrite(right_motor_in[0], LOW);
    digitalWrite(right_motor_in[1], HIGH);
  }
  else if (_direction == LEFT) {
    digitalWrite(left_motor_in[0], LOW);
    digitalWrite(left_motor_in[1], HIGH);
    digitalWrite(right_motor_in[0], HIGH);
    digitalWrite(right_motor_in[1], LOW);
  }
  else if (_direction == RIGHT) {
    digitalWrite(left_motor_in[0], HIGH);
    digitalWrite(left_motor_in[1], LOW);
    digitalWrite(right_motor_in[0], LOW);
    digitalWrite(right_motor_in[1], HIGH);
  }
  else if (_direction == BRAKE) {
    for (int i = 0 ; i < 2 ; ++i) {
      digitalWrite(left_motor_in[i], LOW);
      digitalWrite(right_motor_in[i], LOW);
    }
  }
}
