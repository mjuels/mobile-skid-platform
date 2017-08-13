// Controller.h

#ifndef _CONTROLLER_h
#define _CONTROLLER_h


class controller {
private:

    // Controller Coefficients
    float *Num = 0;
    float *Den = 0;

    // Control action and error history
    float *eHist = 0;
    float *uHist = 0;

    // Controller order
    int Order = 0;

    // Control Limits
    float uMax = 1.0;
    float uMin = -1.0;

public:
    // Parameters
    float u = 0.0;

    // Methods
    controller(int Order, float * NumVal, float * DenVal);
    controller(void){};
    void SampleController(float error);
    void ResetController(void);
    void PrintCoefficients(void);
    void PrintHistory();

};

#endif

