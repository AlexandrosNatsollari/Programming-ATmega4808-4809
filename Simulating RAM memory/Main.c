#include <avr/io.h>
#include <avr/interrupt.h>


int sw5=0;
int sw6=0;
int TyxaiaTimh=0;
int memory[32];
int n=0;
int ArithmosPoudiavastike=0;
int main(){
	//LED palmwn
	PORTD.DIR |= PIN6_bm; //palmos CMPO
	PORTD.DIR |= PIN7_bm;//palmos CMP1
	//LED exodou
	PORTD.DIR |= PIN0_bm;//least significant bit
	PORTD.DIR |= PIN1_bm;
	PORTD.DIR |= PIN2_bm;
	PORTD.DIR |= PIN3_bm;//most significant bit
	
	//pullup enable and Interrupt enabled with sense on both edges
	//interrapt gia thn leitoyrgia WRITE
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	sei(); //enable interrupts
	
	//pullup enable and Interrupt enabled with sense on both edges
	//interrapt gia thn leitoyrgia READ
	PORTC.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
	//prescaler=1024
	TCA0.SINGLE.CTRLA=TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.PER = 254; //select the resolution
	TCA0.SINGLE.CMP0 = 127; //select the duty cycle
	TCA0.SINGLE.CMP1 = 90; //select the duty cycle
	
	//select Single_Slope_PWM
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	//enable interrupt Overflow
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	//enable interrupt COMP0
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP1_bm;
	
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable
	
	
		
	while (1){ }
}


ISR(TCA0_OVF_vect){
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	if(sw5==1){
		

		//vazoume if gia na mhn exoume memory overflow
		if(n<32){
			memory[n]=TyxaiaTimh;
			printf("memomry cell %d: %d",n,memory[n]);
			n++; //counter pou deixnei kathe fora se poio keli na apothikeusoume thn teleutaia timh
			//wste na mhn grafoyme panw se alles
		}
		else if(n>=32){
			printf("mememory is full\n");
		}
		
		sw5=0;
	}
	
	PORTD.OUT |= PIN6_bm; //Otan ektelestei h entolh anapse LED6
	PORTD.OUT |= PIN7_bm; //Otan ektelestei h entolh anapse LED7
}


ISR(TCA0_CMP0_vect){
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	if(sw5==1){
		

		//vazoume if gia na mhn exoume memory overflow
		if(n<32){
			memory[n]=TyxaiaTimh;
			printf("memomry cell %d: %d",n,memory[n]);
			n++; //counter pou deixnei kathe fora se poio keli na apothikeusoume thn teleutaia timh
			//wste na mhn grafoyme panw se alles
		}
		else if(n>=32){
			printf("mememory is full\n");
			
		}
		sw5=0;
	}
	
	PORTD.OUTCLR = PIN6_bm; //Otan ektelestei h entolh kleise to LED6
}

ISR(TCA0_CMP1_vect){
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	//sbhnoume ta LED exodoy meta apo mia periodo
	PORTD.OUTCLR = PIN0_bm;//0
	PORTD.OUTCLR = PIN1_bm;//0
	PORTD.OUTCLR = PIN2_bm;//0
	PORTD.OUTCLR = PIN3_bm;//0 
	if(sw6==1){
		ArithmosPoudiavastike=memory[n-1];
		printf("%d",ArithmosPoudiavastike);
		sw6=0;
		//ftiaxame ton pinaka alhtheias gia ta 4Bit ths RAM 
		//wste na anaboun ta katalhla LED pou anaparhstoun to READ 
		if(ArithmosPoudiavastike==0){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==1){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==2){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==3){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUTCLR = PIN3_bm;//0 most	significant bit		
		}
		else if (ArithmosPoudiavastike==4){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==5){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==6){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==7){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUTCLR = PIN3_bm;//0 most significant bit
		}
		else if (ArithmosPoudiavastike==8){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==9){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==10){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==11){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUTCLR = PIN2_bm;//0
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==12){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==13){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUTCLR = PIN1_bm;//0
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==14){
			PORTD.OUTCLR = PIN0_bm;//0 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
		else if (ArithmosPoudiavastike==15){
			PORTD.OUT |= PIN0_bm;  //1 least significant bit
			PORTD.OUT |= PIN1_bm;  //1
			PORTD.OUT |= PIN2_bm;  //1
			PORTD.OUT |= PIN3_bm;  //1 most significant bit
		}
																	
	}
	
	PORTD.OUTCLR = PIN7_bm; //Otan ektelestei h entolh kleise to LED7
}

//interrapt gia thn leitoyrgia WRITE
//energopoihte me to PIN5 tou PORTF
ISR(PORTF_PORT_vect){
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	TyxaiaTimh=rand() % (15 + 1 - 0) + 0;


	sw5=1;//timh poy dhlwnei oti energopoihthike o SWITCH5
}

//interrapt gia thn leitoyrgia READ
//energopoihte me to PIN6 tou PORTC
ISR(PORTC_PORT_vect){
	int intflags = PORTC.INTFLAGS;
	PORTC.INTFLAGS=intflags;

	sw6=1;//timh poy dhlwnei oti energopoihthike o SWITCH6
}

