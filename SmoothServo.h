/*
 * A variation of the Servo class that supports smooth proportional motion.
 *
 * References:
 *
 * 		http://www.paulodowd.com/2011/11/arduino-smooth-rc-servo-motor-control.html
 *
 * */
#ifndef SmoothServo_h
#define SmoothServo_h

#include <Servo.h>

#define SMOOTH_SERVO_LOWER 600
#define SMOOTH_SERVO_CENTER 1500
#define SMOOTH_SERVO_UPPER 2400
#define SMOOTH_SERVO_TIMEOUT_MS 1000

class SmoothServo{

private:

	Servo _servo;

    // The pin attached to the servo's signal wire.
    int _set_pin;

    // The pin attached to the servo's potentiometer.
    int _get_pin;

    // If true, indicates we're powering the servo. Otherwise, servo is slack.
    bool _attached = false;

    // The desired position of the servo, in the range [600:2400] with 1500 as center.
    float _target_pos= SMOOTH_SERVO_CENTER;

    // The position we're currently telling the servo to move to, in the range [600:2400]
    // with 1500 as center.
    float _current_pos = SMOOTH_SERVO_CENTER;

    // The amount of delay at which the servo position changes when a new target is set.
    // Should be between [0.0:1.0]
    // 0.0 = an infinite amount of easing, i.e. no movement
    // 1.0 = no easing, servo moves as fast as physically possible
    float _easing = 0.7;

    // If true, then even after target position is made, position will continuously be checked.
    // Otherwise, once target position has been made, servo will go slack.
    bool _enforce = false;

    // If true, indicates the first update cycle. Otherwise false.
    bool _first = true;

    // Timestamp of when the servo was enabled and motion began.
    unsigned long _motion_start;

public:

	SmoothServo(int set_pin, int get_pin){

        _set_pin = set_pin; // digital pin that supports PWM
        _get_pin = get_pin; // analog

        pinMode(_set_pin, OUTPUT);
        pinMode(_get_pin, INPUT);

	}

	void set_enforce(bool v){
		_enforce = v;
	}

	bool is_enabled(){
		return _attached;
	}

    void enable(){
    	// Activate servo and enforce position.
    	// It's recommended to specify a target position before calling this.
        if(!_attached){
            _servo.attach(_set_pin);
            _servo.write(_target_pos);
            _motion_start = millis();
        }
        _attached = true;
    }

    void disable(){
        _servo.detach();
        _attached = false;
    }

    void set_position(float position){
    	// Sets the target position.
    	_target_pos = constrain(position, SMOOTH_SERVO_LOWER, SMOOTH_SERVO_UPPER);
    }

    void go_to(float position){
    	set_position(position);
    	enable();
    }

    void go_to_degree(int degree){
    	float position = (float)constrain(degree, 0, 180);
    	position = map(position, 0, 180, SMOOTH_SERVO_LOWER, SMOOTH_SERVO_UPPER);
    	go_to(position);
    }

    void set_easing(float e){
    	_easing = e;
    }

    bool timed_out(){
    	return millis() - _motion_start > SMOOTH_SERVO_TIMEOUT_MS;
    }

    void update(){
		int pot; // raw actual position as read from potentiometer
		//float actual_pos;
		float diff; // difference of position

		if(_attached){

			// If this is the first time we've tried updating the position,
			// then read the actual position so we know how much easing to apply.
			if(_first){
				pot = analogRead(_get_pin);
				pot = map(pot, 0, 1023, SMOOTH_SERVO_LOWER, SMOOTH_SERVO_UPPER);
				_current_pos = (float)pot;
				_first = true;
			}

			// Work out the required travel.
			diff = _target_pos - _current_pos;

			// Avoid any strange zero condition
			if( diff != 0.00 ) {
				_current_pos += diff * _easing;
			} else {
				// if diff == 0, we don't need to move!
				if(!_enforce){
					disable();
				}
			}

			// Move the servo to our eased intermediate position.
			_servo.writeMicroseconds( (int)_current_pos );

			// If we haven't been told to indefinitely try to enforce position,
			// and we've exceeded our timeout, then deactivate servo.
			if(!_enforce && timed_out()){
				disable();
			}

			// Delay, need to play with this, use millis()
			//delay(20);
		}

    }

};


#endif
