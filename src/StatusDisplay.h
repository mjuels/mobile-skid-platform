/*
 * StatusDisplay.h
 *
 *  Created on: Jul 30, 2017
 *      Author: mju
 */

#ifndef SRC_STATUSDISPLAY_H_
#define SRC_STATUSDISPLAY_H_

#include <LiquidCrystal.h>

enum DisplayState{
	none,
	joystick_not_registerred,
	deadman_not_pressed,
	joystick_control
};

class StatusDisplay: public LiquidCrystal {
public:
	StatusDisplay(uint8_t rs, uint8_t enable,uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
	  :LiquidCrystal(rs, enable, d0, d1, d2, d3){};
	virtual ~StatusDisplay(){};

	void set_state(DisplayState state){
		this->state = state;
	}

	DisplayState get_state(void){
			return state;
	}

	void conditional_clear(DisplayState state){
		if(this->state != state){
			clear();
			set_state(state);
		}
	}

	void counted_print(const char msg[]){
		if(print_counter_freq > 0){
			if(print_counter_freq == print_counter){
				print(msg);
			}
		}
	}

	void counted_print(double val){
		if(print_counter_freq> 0){
			if(print_counter_freq == print_counter){
				print(val);
			}
		}
	}

	void count(void){
		print_counter++;
		if(print_counter_freq < print_counter){
			print_counter = 0;
		}
	}

	void set_print_counter_freq(int freq){
		if(freq > 0){
			print_counter_freq = freq;
		}
	}



private:
	int print_counter = 0;
	int print_counter_freq = 1;
	DisplayState state = none;

};

#endif /* SRC_STATUSDISPLAY_H_ */
