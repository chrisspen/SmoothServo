#include "SmoothServo.h"

#define GET_PIN A1
#define SET_PIN 6 // PWM

SmoothServo s = SmoothServo(SET_PIN, GET_PIN, 1400, 600, 90-60, 90+60);

unsigned long last;
unsigned long i;

void setup()
{
	s.set_easing(0.05);
	Serial.begin(57600); // must match ino.ini
}

void loop()
{
	Serial.println("Going to 0..."); Serial.flush();
	
	s.go_to_degree(0);
	last = millis();
	i = 0;
	while(millis() - last < 5000 && s.is_enabled()){
		i += 1;
		s.update();
		Serial.println(String("t:")+String(millis()-last)+String(" cp:")+String(s.get_current_pos())); Serial.flush();
		delay(25);
	}

	Serial.println("Going to 180..."); Serial.flush();
	
	s.go_to_degree(180);
	last = millis();
	i = 0;
	while(millis() - last < 5000 && s.is_enabled()){
		i += 1;
		s.update();
		Serial.println(String("t:")+String(millis()-last)+String(" cp:")+String(s.get_current_pos())); Serial.flush();
		delay(25);
	}
	
}