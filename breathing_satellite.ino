#include <math.h>
int PINS[] = {9, 10, 11, 12};

int hall_pin_1 = 2;
int hall_pin_2 = 3;

int cur_pin = 0;
int no_move = 0;

int hall_val_1 = 0;
int hall_val_2 = 0;
long int time_1 = 0;
long int time_2 = 0;
long int START_DELAY = 10000;
long int delay_ = START_DELAY;
double magn_vel = -1;
double coil_vel = -1;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++)
    pinMode(PINS[i], OUTPUT);
  pinMode(hall_pin_1, INPUT);
  pinMode(hall_pin_2, INPUT);
}

void loop() {
  coil_vel = 1 / ( 4 * delay_ * pow(10, -6) );
  
  hall_val_1 = digitalRead(hall_pin_1);
  if (hall_val_1)
    time_1 = micros();
    
  hall_val_2 = digitalRead(hall_pin_2);
  if (hall_val_2)
    time_2 = micros();

  // Меняем катушки
  for (int i = 0; i < 4; i++) {
    if (cur_pin % 4 == i)
      digitalWrite(PINS[i], HIGH);
    else
      digitalWrite(PINS[i], LOW);
  }
  cur_pin += 1;

  // correct the coils velocity
  if (time_1 && time_2) {
    magn_vel = 1 / ( 2 * abs(time_2 - time_1) * pow(10, -6) );
    time_1 = 0;
    time_2 = 0;
  }
  else {
    no_move += 1;
  }
  
  if (no_move >= 100) {
    magn_vel = -1;
    no_move = 0;
  }
  
  if (magn_vel < 83) {
    if (magn_vel >= coil_vel) {
      delay_ = delay_ * 9 / 10;
    }
    else {
      if (delay_ < START_DELAY)
        delay_ = delay_ * 10 / 9;
    }
  }

  Serial.print(delay_);
  Serial.print(" ");
  Serial.print(coil_vel);
  Serial.print(" ");
  Serial.print(magn_vel);
  Serial.print(" ");
  Serial.print(time_1);
  Serial.print(" ");
  Serial.println(time_2);
  
  delay(delay_ / 1000);
  //delayMicroseconds(delay_);
}
