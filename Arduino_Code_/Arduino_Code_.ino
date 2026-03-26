#include <Servo.h>

const int EMG_PIN = A1;

// 5 servos
Servo thumbServo;
Servo indexServo;
Servo middleServo;
Servo ringServo;
Servo pinkyServo;

// Servo positions
const int OPEN_ANGLE  =0;     // change to 180 if direction is opposite
const int CLOSE_ANGLE = 300;    // or 120/150 if  need more closing

// From  calibration:
const int THRESHOLD_FLEX  = 100;  // must go ABOVE this to close
const int THRESHOLD_RELAX = 50;   // must go BELOW this to open

bool handClosed = false;
int filtered = 0;
int relaxCounter = 0;

void setup() {
  Serial.begin(9600);

  // Attach servos to pins
  thumbServo.attach(3);   // Thumb
  indexServo.attach(5);   // Index
  middleServo.attach(6);  // Middle
  ringServo.attach(9);    // Ring
  pinkyServo.attach(10);  // Pinky

  // Start open
  setHand(OPEN_ANGLE);
}

void loop() {
  int raw = analogRead(EMG_PIN);

  // smoothing
  filtered = (filtered * 7 + raw) / 8;

  Serial.println(filtered);

  // ---------- FLEX → CLOSE ----------
  if (!handClosed && filtered > THRESHOLD_FLEX) {
    handClosed = true;
    relaxCounter = 0;
    setHand(CLOSE_ANGLE);
    delay(200);
  }

  // ---------- RELAX → OPEN ----------
  else if (handClosed) {
    if (filtered < THRESHOLD_RELAX) {
      relaxCounter++;
    } else {
      relaxCounter = 0;
    }

    if (relaxCounter >= 6) {   // ~100–150 ms relaxed
      handClosed = false;
      setHand(OPEN_ANGLE);
      relaxCounter = 0;
      delay(200);
    }
  }

  delay(20);
}

void setHand(int angle) {
  thumbServo.write(angle);
  indexServo.write(angle);
  middleServo.write(angle);
  ringServo.write(angle);
  pinkyServo.write(angle);
}
