/*
 * AggregatedMotor.h
 *
 *  Created on: Aug 12, 2017
 *      Author: mju
 */

#ifndef SRC_AGGREGATEDMOTOR_H_
#define SRC_AGGREGATEDMOTOR_H_

#include <StandardCplusplus.h>
//#include <list>
#include <vector>
#include <MotorWithSpeedControl.h>
#include <Controller.h>

class AggregatedMotor {
public:
	AggregatedMotor();
	virtual ~AggregatedMotor();

	void AddMotor(MotorWithSpeedControl* new_motor){
		children[children_count] = new_motor;
		children_count++;

		Serial.println(children[children_count-1]->GetPinA());

	};

	void AssignControllerToEachMotor(controller new_controller){
		for (int i=0; i < children_count; i++) {
		    children[i]->AssignSpeedController(new_controller);
		}

	};

	void SampleAllControllers(void){
		for (int i = 0; i < children_count; i++) {
			children[i]->SampleSpeedController();
		}

	}

	void AssignReferenceToEachMotor(float* references){

		// Check that the necessary number of refs has been passed
		for (int i =0; i < children_count; i++) {
			children[i]->SetAbsoluteSpeedReference(references[i]);
		}

		};
//
//	MotorWithSpeedControl* GetChild(int i){
//		return children[i];
//	}

private:
	MotorWithSpeedControl *children[100];
	int children_count = 0;

};

#endif /* SRC_AGGREGATEDMOTOR_H_ */
