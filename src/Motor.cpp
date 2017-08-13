/*! @file Motor.cpp
* @brief This file holds all method definitions of motor class
*/

#include "Motor.h"

/** Constructs a motor from two arduino pins
* @param PA Pin A of the motor
* @param PB Pin B of the motor
* @return The motor object
*/
motor::motor(int PA, int PB){
    // Assign the pins
	PinA = PA;
	PinB = PB;

    // Set Pin mode
    pinMode(PinA, OUTPUT);
    pinMode(PinB, OUTPUT);
}

/** Retrieve speed of the motor
* @param[out] PresentSpeed 
* @return The present speed of the motor as a PWM byte value
*/
int motor::GetPresentDutyCycle() 
{
	// Return the speed
    return PresentDuty;
}

/** Sets the speed of the motor
* The requested speed must be a relative fraction of maximum speed in the range [-1; 1].
*
* Positive speed is understood as forward, whereas negative speed means backward.
*
* The speed is saturated to the defined Safe Relative speed of the motor.
*
* @param [in] RelativeSpeed The fractional speed to assign the motor.
* 
*/
void motor::ApplySpeedDutyCycle(float RelativeSpeed)
{
    // Saturate the desired speed to the safe range
    float SafeSpeed = General::saturate(RelativeSpeed, MinSafeRelativeSpeed, MaxSafeRelativeSpeed);
	
    // Convert the relative speed to PWM byte range
    int ConvertedSpeed = (int)(SafeSpeed*MaxPWMPinVal);

    // Test if a new speed is requested
	if (PresentDuty != ConvertedSpeed) {
		
/*
        Serial.print("Setting new speed: ");
		Serial.println(ConvertedSpeed);
*/
        // Assign the desired speed to the present speed
		PresentDuty = ConvertedSpeed;

        // Depending on speed sign, let the motor go either forward, backward or brake
		if (ConvertedSpeed < 0) {
			backward(-ConvertedSpeed);
		}
		else if (ConvertedSpeed > 0) {
			forward(ConvertedSpeed);
		}
		else {
			brake();
		}
	}
}

/*! Assign a forward speed to the motor
* @param [in] ConvertedSpeed Desired speed as a PWM byte value
*/
void motor::forward(int ConvertedSpeed) 
{
    // Set direction to backwards
	digitalWrite(PinB, false);

    // Set speed as a PWM duty cycle
	analogWrite(PinA, ConvertedSpeed);
}

/*! Assign a backward speed to the motor
* @param [in] ConvertedSpeed Desired speed as a PWM byte value
*/
void motor::backward(int ConvertedSpeed) 
{
	// Set direction to backward
    digitalWrite(PinB, true);
	
    // Set speed as a PWM duty cycle
    analogWrite(PinA, 255-ConvertedSpeed);
}

/*! Brake the motor
*/
void motor::brake() 
{	
    // Pull down both pins of the motor
    digitalWrite(PinA, false);
	digitalWrite(PinB, false);
}

/*! Set the speed reference of the motor
 *
 */
void motor::SetAbsoluteSpeedReference(float speed_ref) {
	SpeedReference = General::saturate(speed_ref, -MaxSpeedReference, MaxSpeedReference);
}

int motor::GetPresentSpeedReference() {
	return SpeedReference;
}

