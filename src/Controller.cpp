// 
// 
// 

#include "Controller.h"
#include "General.h"

controller::controller(int Order, float * NumVal, float * DenVal)
{
    // Update order
    this->Order = Order;
    
    // Clear allocations
    delete[] Num;
    delete[] Den;
    delete[] eHist;
    delete[] uHist;

    // Reallocate
    Num     = new float[Order];
    Den     = new float[Order];
    eHist   = new float[Order];
    uHist   = new float[Order];

    // Initialize coefficients
    for (int i = 0; i < Order; i++) {
        *(Num + i) = *(NumVal + i)/(*DenVal); // Divide by first denominator value to ensure monic polynomials
        *(Den + i) = *(DenVal + i)/(*DenVal); // Divide by first denominator value to ensure monic polynomials
        *(eHist + i) = 0;
        *(uHist + i) = 0;
    }
    
}

void controller::SampleController(float error)
{

    // Version 2.0 implementation for general transfer function
    float CtrlSignal = 0;
    int i;

    // Update error and control signal
    for (i = Order - 1; i >= 1; i--) {
        *(eHist + i) = *(eHist + i - 1);
        *(uHist + i) = *(uHist + i - 1);
    }
    // Update most recent error
    *eHist = error;

    // Calculate control law
    CtrlSignal = CtrlSignal + (*eHist)*(*Num);
    for (i = 1; i < Order; i++) {
        CtrlSignal = CtrlSignal + (*(Num + i))*(*(eHist + i)) - (*(Den + i)) * (*(uHist + i));
    }

    // Saturation - Anti Wind up
    *uHist = General::saturate(CtrlSignal, uMin, uMax);

    // Update speed
    u = *uHist;

    return;
};

void controller::ResetController(void) 
{
    // Reset history
    for (int i = 0; i < Order; i++) {
        *(uHist + i) = 0;
        *(eHist + i) = 0;
    }
    
}

void controller::PrintCoefficients()
{
    Serial.println("Numerator:");
    for (int i = 0; i < Order; i++) {
        Serial.print(*(Num + i));
        Serial.print(" ");
    }
    Serial.println("");

    Serial.println("Denominator:");
    for (int i = 0; i < Order; i++) {
        Serial.print(*(Den + i));
        Serial.print(" ");
    }
    Serial.println("");

}

void controller::PrintHistory() {
    Serial.println("Error:");
    for (int i = 0; i < Order; i++) {
        Serial.print(*(eHist + i));
        Serial.print(" ");
    }
    Serial.println("");

    Serial.println("Control Signal:");
    for (int i = 0; i < Order; i++) {
        Serial.print(*(uHist + i));
        Serial.print(" ");
    }
    Serial.println("");

}
