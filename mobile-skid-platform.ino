/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3BT.h>
#include <usbhub.h>
#include <Encoder.h>

#include "StatusDisplay.h"
#include "MotorWithSpeedControl.h"
#include "ControllerStructures.h"

#define TRACK_WIDTH 0.16 // meters
#define WHEEL_RADIUS 0.06 // meters

#define LINEAR_SCALE 1.2	// Max speed m/s
#define ANGULAR_SCALE 9 // Max rotation rad/s

#define MOTOR_SPEED_RATING 291 // rpm

#define JOY_THRES 5

#define DEADMAN_BUTTON R1
#define FULL_RANGE_LINEAR 160
#define FULL_RANGE_ANGULAR 80

#define BATTERY_VOLTAGE 11.1
#define MOTOR_VOLTAGE_RATING 7.2

bool LED_STATE = false;
#define LED_PIN 39

// initialize the library with the numbers of the interface pins
StatusDisplay lcd(23, 22, 25, 24, 27, 26); // Changed from 8, 9, 4, 5, 6, 7

USB Usb;

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS3BT PS3(&Btd, 0x00, 0x1A, 0x7D, 0xDA, 0x71, 0x13); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;

float SampleTime = 0.03; // [s]
//float WheelDia = 0.1207; // [m]
//float WheelCircum;

float EncoderPPR = 12000;


// Motor Design
// - Motors are specified by two pins: A and B
// 	 A is used as pwm pin
//		For mega, this must be pins 2-13 and 44-46
//   B is used as digital H/L to control directionq
//		This can be any digital pin
// - First 2 arguments are motor pins
// - Second 2 arguments are encoder pins
// *** It was attempted to use a composite pattern for the motors, but the control got too bumpy ***
MotorWithSpeedControl left_back_motor 	= MotorWithSpeedControl(2, 3, 19, 33,  SampleTime, EncoderPPR);
MotorWithSpeedControl left_front_motor 	= MotorWithSpeedControl(4, 5, 18, 32,  SampleTime, EncoderPPR);
MotorWithSpeedControl right_front_motor = MotorWithSpeedControl(44, 42, 20, 31,SampleTime, EncoderPPR);
MotorWithSpeedControl right_back_motor 	= MotorWithSpeedControl(45, 43, 21, 30,SampleTime, EncoderPPR);

float speed_kp = .01;
float speed_ki = .1;

float left_rpm;
float right_rpm;

void setup() {
	Serial.begin(115200);

	left_back_motor.AssignSpeedController(ControllerStructures::PI_Controller(SampleTime, speed_kp, speed_ki));
	left_front_motor.AssignSpeedController(ControllerStructures::PI_Controller(SampleTime, speed_kp, speed_ki));
	right_back_motor.AssignSpeedController(ControllerStructures::PI_Controller(SampleTime, speed_kp, speed_ki));
	right_front_motor.AssignSpeedController(ControllerStructures::PI_Controller(SampleTime, speed_kp, speed_ki));

	// Compensate for direction
	right_front_motor.SetReverse(true);
	right_back_motor.SetReverse(true);

	float SampleFreq = 1/SampleTime; // [Hz]
	// initialize Timer4
	noInterrupts(); // disable all interrupts
	TCCR4A = 0;
	TCCR4B = 0;
	TCNT4 = 0;

	int Prescaler = 256;

	OCR4A = (int) (16000000/Prescaler/SampleFreq); // compare match register 16MHz/256/10Hz

	TCCR4B |= (1 << CS12); // 256 prescaler
	TCCR4B |= (1 << WGM12); // CTC mode
	TIMSK4 |= (1 << OCIE4A); // enable timer compare interrupt
	interrupts(); // enable all interrupts

	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Booting Arduino");
	lcd.set_print_counter_freq(100);


	#if !defined(__MIPSEL__)
		while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
	#endif
		if (Usb.Init() == -1) {
			Serial.print(F("\r\nOSC did not start"));
			while (1); //halt
		}
	Serial.print(F("\r\nPS3 Bluetooth Library Started"));

	pinMode(LED_PIN,OUTPUT);
}


ISR(TIMER4_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)

	left_back_motor.SampleSpeedController();
	left_front_motor.SampleSpeedController();
	right_back_motor.SampleSpeedController();
	right_front_motor.SampleSpeedController();

}


void loop() {
  Usb.Task();
  lcd.setCursor(0, 0);
  lcd.count();

  left_rpm = 0.0;
  right_rpm = 0.0;

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
	  if(PS3.getButtonPress(DEADMAN_BUTTON)){
		  lcd.conditional_clear(joystick_control);
		  	  int angular_axis = PS3.getAnalogHat(RightHatX);
		  	  int linear_axis  = PS3.getAnalogHat(LeftHatY);

		  	  float angular = map(angular_axis, 0, 255, -FULL_RANGE_ANGULAR, FULL_RANGE_ANGULAR); // [Input in RPM]
		  	  float linear = map(linear_axis, 0, 255, FULL_RANGE_LINEAR, -FULL_RANGE_LINEAR); // [Input in RPM]

		  	  if(fabs(angular) < JOY_THRES){
		  		  angular = 0;
		  	  }

		  	  if(fabs(linear) < JOY_THRES){
		  	  		  linear = 0;
		  	  	  }

		  	  left_rpm 	= linear - angular*TRACK_WIDTH/WHEEL_RADIUS/2;
		  	  right_rpm = linear + angular*TRACK_WIDTH/WHEEL_RADIUS/2;

		  	  lcd.counted_print("L: ");
		  	  lcd.counted_print((int) left_front_motor.GetMeasuredSpeed());
		  	  lcd.counted_print("/");
		  	  lcd.counted_print((int) left_front_motor.GetPresentSpeedReference());
		  	  lcd.counted_print(" rpm   ");

		  	  lcd.setCursor(0, 1);
		  	  lcd.counted_print("R: ");
		  	  lcd.counted_print((int) right_front_motor.GetMeasuredSpeed());
		  	  lcd.counted_print("/");
		  	  lcd.counted_print((int) right_front_motor.GetPresentSpeedReference());
		  	  lcd.counted_print(" rpm   ");
	  }
	  else{
		  // Deadman button not pressed
		  lcd.conditional_clear(deadman_not_pressed);
		  lcd.print("Deadman Button");
		  lcd.setCursor(0, 1);
		  lcd.print("Not Pressed (R1)");
	  }
  }else{
	  // PS3 Not registered
	  lcd.conditional_clear(joystick_not_registerred);
	  lcd.print("Pending Joystick");
	  lcd.setCursor(0, 1);
	  lcd.print("Registration");
  }

  // Apply speed reference to motors
  left_front_motor.SetAbsoluteSpeedReference(left_rpm);
  left_back_motor.SetAbsoluteSpeedReference(left_rpm);
  right_front_motor.SetAbsoluteSpeedReference(right_rpm);
  right_back_motor.SetAbsoluteSpeedReference(right_rpm);




}
