#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

#define LMOTOR_1 4
#define LMOTOR_2 5
#define RMOTOR_1 6
#define RMOTOR_2 7

#define LMOTOR_A 9
#define RMOTOR_A 10

#define MAXSPEED 200

//#define OVERRIDE_REMOTE_CONTROL 0.5
#define DO_PRINTOUT

// global variables
float l_req;
float r_req;

// p1, p2 motor pin for direction, ps motor pin for speed, req requested speed
void drive_single(uint32_t p1, uint32_t p2, uint32_t ps, float req) {
  uint32_t spd = (uint32_t)(abs(req) * MAXSPEED);
  if (abs(req) <= 0.1) { // motor stop
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
  } else if (req > 0.1) { // motor forward
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);
  }

  // set speed
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

  Serial.begin(250000);
  Dabble.begin(9600);

  // set motor speed to 0
  l_req = 0.0;
  r_req = 0.0;
}

void process_degree(int degree) {
  if (degree == 90) {
    l_req = 1.0;
    r_req = 1.0;
  } else if (315 <= degree && degree <= 0) {
    // turn right
    l_req = 1.0;
    r_req = 0;
  } else if (180 <= degree && degree < 225) {
    // turn left
    l_req = 0;
    r_req = 1.0;
  } else if (0 < degree && degree <= 90) {
    // percentage right turn
    r_req = degree / 90.0;
    l_req = 1.0;
  } else if (90 < degree && degree <= 180) {
    // percentage left turn
    r_req = 1.0;
    l_req = (180 - degree) / 90.0;
  }
}

void process_radious(int radious) {
  l_req *= radious / 7.0;
  r_req *= radious / 7.0;
}

void loop() {
  Dabble.processInput();

  int a = GamePad.getAngle();

  process_degree(a);

  int r = GamePad.getRadius();

  process_radius(a);

  drive(l_req, r_req);

#ifdef DO_PRINTOUT
  Serial.print("L:");
  Serial.print(l_req);
  Serial.print(", R:");
  Serial.println(r_req);
#endif
}
