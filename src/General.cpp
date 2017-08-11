#include "General.h"

/** Saturate float between minimum and maximum
* @param in number to be saturated
* @param min minimum limit for saturation
* @param max maximum limit for saturation
* @return Saturated value
*/
float General::saturate(float in, float min, float max)
{
    float out = in;
    if (min >= max) {
        min = max;
    }
    if (out >= max) {
        out = max;
    }
    if (out <= min) {
        out = min;
    }
    return out;
}

void General::SetupTimer0(int Freq)
{
    // Disable interrupts during setup
    cli();

    // Setting up timer for interupt
    // - Used by controller
    // Lingo:   TCCR:   Timer/Counter Control Register
    //          OCR:    Output Compare Register
    //          TCNT:   Timer Count
    //          CS:     Clock Select
    //          CS02:0  Clock select 0, bits 2,1,0 
    //          TIMSK:  Timer Interrupt Mask Register
    //          WGM:    Waveform Generation Mode
    //          CTC:    Clear Timer on Compare mode

    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B 
    TCNT0 = 0;//initialize counter value to 0

              // Cannot be slower than 62 Hz when running on Timer0 as the OCR0A value has to be below 255
    int RegisterValue = 15625 / Freq - 1;
    if (RegisterValue > 255) {
        RegisterValue = 255;
        float ActualFreq = 15625 / 256;
        Serial.println("Limited Interrupt Frequency on TImer0!");
        Serial.print("Actial Frequency: ");
        Serial.println(ActualFreq);
    }

    // Set intterupt count target
    OCR0A = RegisterValue;
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS02 and CS00 bits for 1024 prescaler
    TCCR0B |= (1 << CS02) | (1 << CS00);
    // enable timer compare interrupt by comparison to OCR0A
    TIMSK0 |= (1 << OCIE0A);

    // Reenable interrupts
    sei();
}

void General::DisableTimer0(void)
{
    // Disable interrupts during setup
    cli();

    // Setting up timer for interupt
    // - Used by controller
    // Lingo:   TCCR:   Timer/Counter Control Register
    //          OCR:    Output Compare Register
    //          TCNT:   Timer Count
    //          CS:     Clock Select
    //          CS02:0  Clock select 0, bits 2,1,0 
    //          TIMSK:  Timer Interrupt Mask Register
    //          WGM:    Waveform Generation Mode
    //          CTC:    Clear Timer on Compare mode

    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B 
    TCNT0 = 0;//initialize counter value to 0

              // Reenable interrupts
    sei();
}
