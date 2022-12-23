#include <Arduino.h>

#include "../state/scr/State.h"

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}
  State s;
  //PositionState p;
  //OrientationState o;
  //Quaternion q;
  //Vector v(3);
  //Matrix m(3,3);
  Serial.println(s.positionState.to_str());
  //Serial.println(p.to_str());
  //Serial.println(o.to_str());
  //Serial.println(q.to_str());
  //Serial.println(v.to_str());
  //Serial.println(m.to_str());
  //Serial.println(sizeof(State));//126
}

void loop() {


}
