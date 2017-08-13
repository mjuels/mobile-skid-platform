/*
 * MotorWithSpeedControl.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: mju
 */

#include <MotorWithSpeedControl.h>
#include <math.h>

MotorWithSpeedControl::MotorWithSpeedControl(int PA, int PB, int Enc_PA,int Enc_PB, float Ts, int enc_ppr)
							:motor(PA, PB)
							,speed_encoder(Enc_PA,Enc_PB)
{
	// Sample time for speed control
	sample_time = Ts;

	// Encoder resolution
	encoder_PPR = enc_ppr;
}

MotorWithSpeedControl::~MotorWithSpeedControl() {
	// TODO Auto-generated destructor stub
}

void MotorWithSpeedControl::SampleSpeedController() {

	// Measure the actual speed through the encoder
	ticks = speed_encoder.read();
	measured_speed = ((float) ticks)/encoder_PPR/sample_time*60;

	// Compensate for left/right rotation
	if(reversed){
		measured_speed = -measured_speed;
	}

	// Reset encoder
	speed_encoder.write(0);

	// Calculate speed error
	float speed_error = this->GetPresentSpeedReference() - measured_speed;

	// Update controller
	speed_controller.SampleController(speed_error);

	// Update speed reference
	if(fabs(this->GetPresentSpeedReference()) < 0.1){
		this->ApplySpeedDutyCycle(0); // To avoid humming at low dutycycle with zero error
	}else{
		this->ApplySpeedDutyCycle(speed_controller.u);
	}
}

void MotorWithSpeedControl::AssignSpeedController(controller spd_ctrl) {
	speed_controller = spd_ctrl;
}

void MotorWithSpeedControl::SetReverse(boolean reverse_status) {
	reversed = reverse_status;
}

float MotorWithSpeedControl::GetMeasuredSpeed() {
	return measured_speed;
}

int MotorWithSpeedControl::GetTicks() {
	return ticks;
}
