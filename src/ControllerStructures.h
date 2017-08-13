// ControllerStructures.h

#ifndef _CONTROLLERSTRUCTURES_h
#define _CONTROLLERSTRUCTURES_h


#include "Controller.h"
#include <math.h>

class ControllerStructures
{
    public:
		static controller P_Controller(float Kp);
		static controller PI_Controller(float Ts, float Kp, float Ki);
        static controller PID_Controller(float Ts, float Kp, float Ki, float Kd);
        static controller Lead_Controller(float Ts, float q, float p, float K);
};


#endif

