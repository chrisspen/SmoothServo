#include "SmoothServo.h"

#define GET_PIN A1
#define SET_PIN 6 // PWM

SmoothServo s = SmoothServo(SET_PIN, GET_PIN);

unsigned long last;

void setup()
{
	//s.set_easing(0.7);
}

void loop()
{
	Serial.println("Going to 0..."); Serial.flush();
	s.go_to_degree(0);
	last = millis();
	while(millis() - last < 5000 && s.is_enabled()){
		s.update();
		delay(5);
	}

	Serial.println("Going to 180..."); Serial.flush();
	s.go_to_degree(180);
	last = millis();
	while(millis() - last < 5000 && s.is_enabled()){
		s.update();
		delay(5);
	}
}
