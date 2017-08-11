/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include "Arduino.h"
#include <PS3BT.h>
#include <usbhub.h>
#include "StatusDisplay.h"
#include "Motor.h"

#define TRACK_WIDTH 0.16 // meters
#define WHEEL_RADIUS 0.06 // meters

#define LINEAR_SCALE 1.2	// Max speed m/s
#define ANGULAR_SCALE 9 // Max rotation rad/s

#define MOTOR_SPEED_RATING 291 // rpm

#define JOY_THRES 0.02

#define DEADMAN_BUTTON R1
#define FULL_RANGE 1000.0

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

// Motor Design
// - Motors are specified by two pins: A and B
// 	 A is used as pwm pin
//		For mega, this must be pins 2-13 and 44-46
//   B is used as digital H/L to control directionq
//		This can be any digital pin


// Create motors
motor left_back_motor 	= motor(2, 3);
motor left_front_motor 	= motor(4, 5);
motor right_front_motor = motor(44, 42);
motor right_back_motor 	= motor(45, 43);


float left_pwm_rel;
float right_pwm_rel;

void setup() {

	// initialize Timer4
	noInterrupts(); // disable all interrupts
	TCCR4A = 0;
	TCCR4B = 0;
	TCNT4 = 0;

	OCR4A = 6250; // compare match register 16MHz/256/10Hz
	TCCR4B |= (1 << WGM12); // CTC mode
	TCCR4B |= (1 << CS12); // 256 prescaler
	TIMSK4 |= (1 << OCIE4A); // enable timer compare interrupt
	interrupts(); // enable all interrupts

	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Booting Arduino");
	lcd.set_print_counter_freq(100);

	Serial.begin(115200);
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
	LED_STATE = !LED_STATE;
	digitalWrite(LED_PIN,LED_STATE);
}


void loop() {
  Usb.Task();
  lcd.setCursor(0, 0);
  lcd.count();

  left_pwm_rel = 0;
  right_pwm_rel = 0;

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
	  if(PS3.getButtonPress(DEADMAN_BUTTON)){
		  lcd.conditional_clear(joystick_control);
		  	  int angular_axis = PS3.getAnalogHat(RightHatX);
		  	  int linear_axis  = PS3.getAnalogHat(LeftHatY);

		  	  float angular_rel = map(angular_axis, 0, 255, -FULL_RANGE, FULL_RANGE)/FULL_RANGE;
		  	  float linear_rel = map(linear_axis, 0, 255, FULL_RANGE, -FULL_RANGE)/FULL_RANGE;

		  	  if(fabs(angular_rel) < JOY_THRES){
		  		  angular_rel = 0;
		  	  }

		  	  if(fabs(linear_rel) < JOY_THRES){
		  	  		  linear_rel = 0;
		  	  	  }

		  	  float angular = angular_rel*ANGULAR_SCALE;
		  	  float linear = linear_rel*LINEAR_SCALE;

		  	  float vel_longi = angular*TRACK_WIDTH/2;

		  	  float left_mps = linear - vel_longi;
		  	  float right_mps = linear + vel_longi;

		  	  float left_rpm = left_mps*60/(2*PI*WHEEL_RADIUS);
		  	  float right_rpm = right_mps*60/(2*PI*WHEEL_RADIUS);

		  	  left_pwm_rel = left_rpm/MOTOR_SPEED_RATING*MOTOR_VOLTAGE_RATING/BATTERY_VOLTAGE;
		  	  right_pwm_rel = right_rpm/MOTOR_SPEED_RATING*MOTOR_VOLTAGE_RATING/BATTERY_VOLTAGE;

		  	  lcd.counted_print("Left:  ");
		  	  lcd.counted_print(left_pwm_rel);
		  	  lcd.counted_print(" %");

		  	  lcd.setCursor(0, 1);
		  	  lcd.counted_print("Right: ");
		  	  lcd.counted_print(right_pwm_rel);
		  	  lcd.counted_print(" %");

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
  right_back_motor.SetSpeed(right_pwm_rel);
  right_front_motor.SetSpeed(right_pwm_rel);
  left_back_motor.SetSpeed(left_pwm_rel);
  left_front_motor.SetSpeed(left_pwm_rel);

}
