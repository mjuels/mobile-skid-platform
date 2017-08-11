// 
// 
// 

#include "ControllerStructures.h"


controller ControllerStructures::PID_Controller(float Ts, float Kp, float Ki, float Kd) {
    const int Order = 3;
    float Num[3];
    float Den[3];

    // Numerator coefficients
    Num[0] = 4 / pow(Ts, 2) + 2*Kp/(Ts*Kd) + Ki/Kd;
    Num[1] = 2 * Ki / Kd - 8 / pow(Ts, 2);
    Num[2] = 4 / pow(Ts, 2) - 2 * Kp / (Ts*Kd) + Ki / Kd;

    // Denominator Coefficients
    Den[0] = 2 / Ts;
    Den[1] = 0;
    Den[2] = -2 / Ts;

    return controller(Order, &Num[0], &Den[0]);
}

controller ControllerStructures::P_Controller(float Kp) {
	const int Order = 1;
	float Num[1];
	float Den[1];

	// Numerator coefficients
	Num[0] = Kp;

	// Denominator Coefficients
	Den[0] = 1;

	return controller(Order, &Num[0], &Den[0]);

}

controller ControllerStructures::Lead_Controller(float Ts, float q, float p, float K)
{
    const int Order = 3;
    float Num[3];
    float Den[3];

    // Numerator coefficients
    Num[0] = K * p / q * (q*Ts + 2);
    Num[1] = K * p / q * (q*Ts - 2);
    Num[2] = 0;

    // Denominator Coefficients
    Den[0] = p*Ts + 2;
    Den[1] = p*Ts - 2;
    Den[2] = 0;

    return controller(Order, &Num[0], &Den[0]);
}
