// This version of code is the last updated one, done on 240624 & fed into Atmega8 of the HAMAK UNIVERSITY/ RAGHID HOMSI.
// Atmega8 power pins, 7 & 20 Vcc, 8 & 22 Gnd., 5 volts DC power.
// Inputs of LDRs sensors./////////////
int ldr_right = A0;// pin#23 on atmega8 blue/white
int ldr_left = A1;// pin#24 on atmega8 blue
int ldr_up = A2;// pin#25 on atmega8 green/white
int ldr_down = A3;// pin#26 on atmega8 green
int vary_sens = A4;//pin#27 on atmega8

int center_led = 6;// pin#12 on atmega8

// Motors pins/////////////////////////
int motor_pin_right_left_plus = 12;// pin#18 on atmega8
int motor_pin_right_left_minus = 11;// pin#17 on atmega8
int motor_pin_up_down_plus = 10;// pin#16 on atmega8
int motor_pin_up_down_minus = 9;// pin#15 on atmega8

//Global variables/////////////////////
volatile int val_sens;
volatile int val_right;
volatile int val_left;
volatile int val_up;
volatile int val_down;


/////beeper constants///////////////////
volatile bool oscilllator_state = 0;
unsigned long previousMillis_osc = 0;// will store last time LED was updated
const long interval_osc = 300UL;// flashing rate

const int ledCount = 6; // 4,5,6,11,13,14 on atmega8
int ledPins[] = {2, 3, 4, 5, 7, 8};

void setup() {
  pinMode (A0, INPUT);
  pinMode (A1, INPUT);
  pinMode (A2, INPUT);
  pinMode (A3, INPUT);
  pinMode (A4, INPUT);

  pinMode (motor_pin_right_left_plus, OUTPUT);
  pinMode (motor_pin_right_left_minus, OUTPUT);
  pinMode (motor_pin_up_down_plus, OUTPUT);
  pinMode (motor_pin_up_down_minus, OUTPUT);
  pinMode ( center_led, OUTPUT);

  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
}

void varysenss(void) {
  val_sens = analogRead(vary_sens);  //When this value increases, the sensivity to light change decreases. make it larger to prevent system from responding to cluods changes periodically!
  val_sens = map (val_sens, 0, 1023, 1, 102);//was 102
}

void right_left_up_down_stop(void) {

  if (val_right > val_left + val_sens) {
    delay(20);
    digitalWrite(motor_pin_right_left_plus, HIGH);
    digitalWrite(motor_pin_right_left_minus, LOW);
    beeper_void();
  }

  else if (val_left > val_right + val_sens) {
    delay(20);
    digitalWrite(motor_pin_right_left_plus, LOW);
    digitalWrite(motor_pin_right_left_minus, HIGH);
    beeper_void();
  }

  else if (val_up > val_down + val_sens) {
    delay(20);
    digitalWrite(motor_pin_up_down_plus, HIGH);
    digitalWrite(motor_pin_up_down_minus, LOW);
    beeper_void();
  }

  else if (val_down > val_up + val_sens) {
    delay(20);
    digitalWrite(motor_pin_up_down_plus, LOW);
    digitalWrite(motor_pin_up_down_minus, HIGH);
    beeper_void();
  }

  else {
    digitalWrite(motor_pin_right_left_plus, LOW);
    digitalWrite(motor_pin_right_left_minus, LOW);
    digitalWrite(motor_pin_up_down_plus, LOW);
    digitalWrite(motor_pin_up_down_minus, LOW);

    if (oscilllator_state == HIGH) {
      oscilllator_state = LOW;
    }
    digitalWrite(center_led, oscilllator_state);
  }
}


void beeper_void(void) {// beeper or ORANGE LED indicator.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis_osc >= interval_osc) {
    previousMillis_osc = currentMillis;
    if (oscilllator_state == LOW) {
      oscilllator_state = HIGH;
    }
    else {
      oscilllator_state = LOW;
    }
    digitalWrite(center_led, oscilllator_state);
  }
}

void bargraph(void) {
  val_right;
  int ledLevel = map (val_right, 0, 200, 0, 5);// full range as per LDRs used is between 0 and 80
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);
    }
    else {
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
}

void loop() {
  val_right = analogRead(ldr_right);
  val_right = map (val_right, 0, 1023, 0, 512);

  val_left = analogRead(ldr_left);
  val_left = map (val_left, 0, 1023, 0, 512);

  val_up = analogRead(ldr_up);
  val_up = map (val_up, 0, 1023, 0, 512);

  val_down = analogRead(ldr_down);
  val_down = map (val_down, 0, 1023, 0, 512);

  varysenss();
  right_left_up_down_stop();
  bargraph();
}
