/////////////////



#INT_TIMER1								// Set timer for 1 ms
void  TIMER1_isr(void) 
{
	set_timer1(_1ms);
	compteur++;
    compteur2++;							// compteur the 1ms
	// tgl_led;
}

#INT_EXT2
void  EXT_isr(void) 
{
	

switch(flag_d){

		case 0:									// if falling edge
			Val_st = compteur;					// read value compteur
			flag_d = true;						// prepar flag for other edge
			ext_int_edge(2, L_TO_H );			// int on rising edge
		break;
		case 1:									// if rising edge
			Val_fin = compteur;					// read value compteur
			flag_d = false;						// prepar flag for other edge
			mes_tot = mes_tot + (val_fin - val_st); // accumulator for number of 1ms
			ext_int_edge(2, H_TO_L );			// int on falling edge
		break;
	}
}


