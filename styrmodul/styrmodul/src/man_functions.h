#define PULSE_WIDTH_L 0x00
#define PULSE_WIDTH_R 0x00
#define PULSE_WIDTH_G 0x15


void pwm_start_L(void);
void pwm_start_R(void);
void pwm_start_G(void);

void forward(int speed);
void backward(int speed);
void rotate_left(void);
void rotate_right(void);
void rotate_left90(void);
void rotate_right90(void);
void forward_right(void);
void forward_left(void);
void stop(void);
void grip_on(void);
void grip_off(void);
void open_grip(void);
void close_grip(void);
void manual_action(uint8_t control_command);

signed int e_last = 0;

volatile uint8_t keep_turning = 0;

void pwm_start_L() // Starta PWM f�r h�ger hjulpar
{
	OCR1AL = PULSE_WIDTH_L; // Pulsbredd 0, motorerna st�r stilla
	OCR1AH = 0;				// J�mf�r med 0 och pulsbredd
	TCCR1B = 1;				// ingen f�rskalning p� timern
}

void pwm_start_R() // Starta PWM f�r v�nster hjulpar
{
	OCR1BL = PULSE_WIDTH_R; // Pulsbredd 0, motorerna st�r stilla
	OCR1BH = 0;             // J�mf�r med 0 och pulsbredd
	TCCR1B = 1;				// ingen f�rskalning p� timern
}

void pwm_start_G() // Starta PWM f�r griparm
{
	OCR3AL = PULSE_WIDTH_G; //St�ll griparmen i �ppet l�ge
	OCR3AH = 0;	// J�mf�r med 0 och pulsbredd
	TCCR3B = 3; // skala ner timern till 1/8
}

void forward(int speed)
{
	PORTB = 0x03; 
	OCR1BL = speed; // right side
	OCR1AL = speed; // left side
	if(switch_ == 0)
	{
		send(0x01, 0x00);
	}
}

void backward(int speed)
{
	PORTB = 0x00;
	OCR1BL = speed; // right side
	OCR1AL = speed; // left side
	if(switch_ == 0)
	{
		send(0x01, 0x01);
	}	
}

void forward_left()
{
	PORTB = 0x03; 
	OCR1BL = 0xF0; // right side
	OCR1AL = 0x30; // left side
}

void forward_right()
{
	PORTB = 0x03;
	OCR1BL = 0x30; // right side
	OCR1AL = 0xF0; // left side
}


void rotate_left90()
{
	PORTB = 0x08;		// skicka avbrott till sensormodulen
	_delay_ms(1);		// v�nta
	PORTB = 0x01;		// sluta skicka avbrott, st�ll in hjulens rotationsriktning
	keep_turning = 1;	// aktivera roteringen
	
	if(switch_ == 0)
	{
		send(0x01, 0x05);
	}
	
	while(keep_turning == 1)	// rotera tills avbrott 
	{
		OCR1BL = 0xA0;			// hastighet v�nster sida
		OCR1AL = 0xA0;			// hastighet h�ger sida
	}
	stop();
}

void rotate_right90()
{
	PORTB = 0x08;		// skicka avbrott till sensormodulen
	_delay_ms(1);		// v�nta
	PORTB = 0x02;		// sluta skicka avbrott, st�ll in hjulens rotationsriktning
	keep_turning = 1;	// aktivera roteringen
	
	if(switch_ == 0)
	{
		send(0x01, 0x04);
	}
		
	while(keep_turning == 1)	// rotera tills avbrott
	{
		OCR1BL = 0xA0;			// hastighet v�nster sida
		OCR1AL = 0xA0;			// hastighet h�ger sida
	}
	stop();
}

void rotate_left()
{
	PORTB = 0x01;	// hjulens rotationsriktning
	OCR1BL = 0xF0;	// v�nster sida
	OCR1AL = 0xF0;	// h�ger sida
}

void rotate_right()
{
	PORTB = 0x02;	// hjulens rotationsriktning
	OCR1BL = 0xF0;	// v�nster sida
	OCR1AL = 0xF0;	// h�ger sida
}

void stop()
{
	PORTB = 0x00;	// hjulens rotationsriktning (stilla)
	OCR1BL = 0x00;	// v�nster sida
	OCR1AL = 0x00;	// h�ger sida
	
	if(switch_ == 0)
	{
		send(0x01, 0x06);
	}
}

void grip_on() //grip helt
{
	OCR3AL = 0x10; // 0x12 ca 1.25ms, 0x11 1.2ms, 0x07 0.5ms
}
void grip_off() //�ppna helt
{
	OCR3AL = 0x1B; // 0x1B ca 1.9ms, 0x1D 2ms, 0x23 2.5ms
}

uint8_t grip = 0x1B;

void open_grip() //�ppna griparmen ett steg
{
	if (grip < 0x1B)
		{
			grip++;
			OCR3AL = grip;
			_delay_ms(200); //v�nta 200 ms innan ett steg till 
		}	
}

void close_grip() //st�ng griparmen ett steg
{
	if (grip > 0x0F)
	{
		grip--;
		OCR3AL = grip;
		_delay_ms(200); //v�nta 200 ms innan ett steg till 
	}
}

void manual_action(uint8_t control_command_) //Kolla senaste kontrollkommandot fr�n kommunikationsmodulen och utf�r kommandot
{
	switch (control_command_) //Kommandon fr�n laptopen
	{
	case 0x00:
		forward(0xF0);
		break;
	case 0x01:
		backward(0xF0);
		break;
	case 0x02:
		forward_right();
		break;
	case 0x03:
		forward_left();
		break;
	case 0x04:
		rotate_right90();
		break;
	case 0x05:
		rotate_left90();
		break;
	case 0x06:
		stop();
		break;
	case 0x07:
		rotate_right();
		break;
	case 0x08:
		rotate_left();
		break;
	case 0x09:
		open_grip();
		break;
	case 0x0A:
		close_grip();
		break;
	}
}


