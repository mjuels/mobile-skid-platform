/*
 * SpeedEncoder.h
 *
 *  Created on: Aug 10, 2017
 *      Author: mju
 */

#ifndef SRC_SPEEDENCODER_H_
#define SRC_SPEEDENCODER_H_

#include <Encoder.h>

class SpeedEncoder{
public:
	SpeedEncoder(int PinA, int PinB,int TicksPerRevolution,float SampleTime);
	float UpdateSpeed(); // Should be called by Timed ISR
private:
	int TPM = 1;
	int SampleTime = 1;
	Encoder Enc;
};




#endif /* SRC_SPEEDENCODER_H_ */
