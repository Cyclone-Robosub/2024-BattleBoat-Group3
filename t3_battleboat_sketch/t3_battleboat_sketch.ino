#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

#define LMOTOR_1 2
#define LMOTOR_2 3
#define RMOTOR_1 4
#define RMOTOR_2 5

#define LMOTOR_A 9
#define RMOTOR_A 10

#define MAXSPEED 200

//#define OVERRIDE_REMOTE_CONTROL 0.5
#define DO_PRINTOUT

void drive_single(uint32_t p1, uint32_t p2, uint32_t ps, float req) {
  uint32_t spd = (uint32_t)(abs(req) * MAXSPEED);
  if (abs(req) <= 0.1) {
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
  } else if (req < 0) {
    digitalWrite(p1, LOW);
    digitalWrite(p2, HIGH);
  } else {
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);
  }

  analogWrite(ps, spd);
}

void drive(float l_req, float r_req) {
  drive_single(LMOTOR_1, LMOTOR_2, LMOTOR_A, l_req);
  drive_single(RMOTOR_1, RMOTOR_2, RMOTOR_A, r_req);
}

void setup() {
  pinMode(LMOTOR_1, OUTPUT);
  pinMode(LMOTOR_2, OUTPUT);
  pinMode(RMOTOR_1, OUTPUT);
  pinMode(RMOTOR_2, OUTPUT);
  
  pinMode(LMOTOR_A, OUTPUT);
  pinMode(RMOTOR_A, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Dabble.processInput();

  float l_req = 0.0;
  float r_req = 0.0;

#ifdef OVERRIDE_REMOTE_CONTROL
  l_req = 0.5;
  r_req = 0.5;
#else
  float steer = GamePad.getXaxisData() / 7.0;

  float fwd = (GamePad.isCirclePressed() ? 1.0 : (GamePad.isCrossPressed() ? -1.0 : 0.0));
  float afwd = GamePad.getYaxisData() / 7.0;

  if (abs(afwd) > 0.2) {
    fwd += afwd;
  }

  l_req = fwd - steer;
  r_req = fwd + steer;
  
  if (l_req > 1.0) l_req = 1.0;
  if (l_req < -1.0) l_req = -1.0;
  if (r_req > 1.0) r_req = 1.0;
  if (r_req < -1.0) r_req = -1.0;
#endif

  drive(l_req, r_req);

#ifdef DO_PRINTOUT
  Serial.print("L:");
  Serial.print(l_req);
  Serial.print(", R:");
  Serial.println(r_req);
#endif
}
