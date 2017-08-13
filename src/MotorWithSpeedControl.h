/*
 * MotorWithSpeedControl.h
 *
 *  Created on: Aug 12, 2017
 *      Author: mju
 */

#ifndef SRC_MOTORWITHSPEEDCONTROL_H_
#define SRC_MOTORWITHSPEEDCONTROL_H_

#include <Motor.h>
#include "Controller.h"
#include "Encoder.h"

class MotorWithSpeedControl: public motor {
public:
	MotorWithSpeedControl(int PA, int PB, int Enc_PA, int Enc_PB, float Ts, int enc_ppr);
	virtual ~MotorWithSpeedControl();
	void AssignSpeedController(controller spd_ctrl); //!< Add a speed controller
	void SampleSpeedController(); //!< Sample the speed controller
	void SetReverse(boolean reverse_status);
	float GetMeasuredSpeed();
	int GetTicks();

private:
	controller speed_controller;
	Encoder speed_encoder;
	int encoder_PPR = 1;
	float sample_time = 1;
	float measured_speed = 0;
	bool reversed = false;
	int ticks = 0;
};

#endif /* SRC_MOTORWITHSPEEDCONTROL_H_ */
