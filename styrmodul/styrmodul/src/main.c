/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
//#include <avr/signal.h>
//#define F_CPU 14.745E6
#define F_CPU 1.843E6
#include <util/delay.h>
#include <avr/interrupt.h>

//Sensordata 
uint8_t control_command, left_short_s, right_short_s, left_long_s, right_long_s, 
		forward_left_s, forward_right_s, forward_center_s, tape_value;
signed e; //reglerfelet

//header och data som s�nds och tas emot
volatile uint8_t header_s = 0;
volatile uint8_t data_s = 0;
volatile uint8_t header_r = 0;
volatile uint8_t data_r = 0x06;

//startknapp och switch
uint8_t button, switch_;

#include <communication.h> // kommunikationen
#include <man_functions.h> // manuella funktioner
#include <auto_functions.h> // automatiska funktioner som kr�ver sensormodulen

#include "twi_slave.h" // I2C


ISR(INT2_vect) //Avbrott fr�n sensormodulen: sluta rotera
{
	keep_turning = 0;
}


int main (void)
{
	board_init();

	TWI_slave_initialize(TWI_CONTROL_MODULE_ADDRESS);
	
	pwm_start_L();
	pwm_start_R();
	pwm_start_G();
	
	sei();
	//cli();

	while(1)
	{
		button = PINA & 0x02; // read PortA, pin 1
		switch_ = PINA & 0x01; // read PortA, pin 0

		while(switch_ != 0) // man
		{	
			//TWI_slave_receive_message(&header, &data); 
			if(header_r == 0x00)
			{
				manual_action(data_r);
			}
		switch_ = (PINA & 0x01);
		}
		
		while(switch_ == 0) //autonomt l�ge
		{
			/*
			if(button != 0) //startar autonomt l�ge
			{
				find_start();
				find_goal();
				return_to_start();
					
			
				
				//find start
				//find goal
				//grab target
				//return to start
				//stop
			
			}
			*/
			switch_ = PINA & 0x01;
		}

	}
	
	// Insert application code here, after the board has been initialized.
}

