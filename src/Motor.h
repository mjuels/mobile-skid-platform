/*! @file Motor.h
* @brief This file holds the class definition of the motor class
*/

#ifndef _MOTOR_h
#define _MOTOR_h


#include "Arduino.h"

// Add general functionality
#include "General.h"

class motor {
private:
	int PinA; //!< Pin A of the Motor
	int PinB; //!< Pin B of the Motor
	int MaxPWMPinVal = 255; //!< Maximum allowed PWM value
	int PresentDuty = 0; //!< Present speed of the motor as a PWM byte value
	float SpeedReference = 0; //!< Present speed reference [rpm]
	float MaxSpeedReference = 160; //!< Maximum applicable speed reference [rpm]
	float MaxSafeRelativeSpeed = 0.7; //!< Maximum safe relative speed
	float MinSafeRelativeSpeed = -MaxSafeRelativeSpeed; //!< Minimum safe relative speed
    boolean Active = true;

    void forward(int ConvertedSpeed); //!< Sets the forward speed of the motor
	void backward(int ConvertedSpeed); //!< Sets the backward speed of the motor
	void brake(); //!< Brakes the motor

public:
	motor(int PA, int PB); //!< Constructor
	void SetAbsoluteSpeedReference(float speed_ref);
	void ApplySpeedDutyCycle(float duty); //!< Apply speed to the motor
	int GetPresentDutyCycle(); //!< Get the speed of the motor
	int GetPresentSpeedReference(); //!< Get the speed of the motor
    void Activate() { Active = true; };
    void Deactivate() { Active = false; };
    boolean isActive() { return Active; };
    int GetPinA(){return PinA;};
};

#endif
