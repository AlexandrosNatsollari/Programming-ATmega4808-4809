#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>


#define del2 10
#define ped 120

int interr=0; //logic flag
int g=0;
int w=0;

int main(void){
	
	short int rnd,tyxaios_arithmos;
	//fanari megalou dromou
	PORTD.DIR |= PIN0_bm; //PIN is output
	PORTD.OUT |= PIN0_bm;
	
	//fanari pezwn
	PORTD.DIR |= PIN1_bm; //PIN is output
	
	
	//fanari mikrou dromou
	PORTD.DIR |= PIN2_bm; //PIN is output
	
	
	
	
	
	while (1) {
		
		//pullup enable and Interrupt enabled with sense on both edges
		PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
		sei(); //enable interrupts
		
		//dinoume mia random timh opws orizei h ekfwnhsh
		tyxaios_arithmos=rand();
		
		//kanoume extract to teleutaio psifio gia  sygrishsh parakatw
		rnd=tyxaios_arithmos % 10;
		printf("%d",rnd);
		
		
		if(rnd==0 || rnd==5 || rnd==8)
		{
			//anavoume to fanari toy mikrou dromou kai kleinoume tou mikrou
			PORTD.OUTCLR= PIN0_bm; //kane kokkino to fanari tou megalou dromou
			_delay_ms(del2); //mikrh kathusterhsh apo thn stigmh pou ginetai prassino to fanari
			PORTD.OUT |= PIN2_bm; //kane prassino to fanari toy mikrou dromou
			g=1;
		}
		else{
			PORTD.OUTCLR= PIN2_bm;//kane kokkino to fanari toy mikrou dromoy ean den einai kokkino
			
			_delay_ms(del2);//kathusterhsh gia to pote tha ginei prassino to fanari
			PORTD.OUT |= PIN0_bm;//kane prasino to fanari toy megaloy dromou ean den einai prassino
			g=0;
		}
		
		if (interr==1)
		{
			PORTD.OUTCLR= PIN0_bm;//kane kokkino to fanari toy megaloy dromou
			_delay_ms(del2); // 2sec
			PORTD.OUT |= PIN1_bm;//kane prasino to fanari twn pezwn
			
			
			
			PORTD.OUT |= PIN2_bm;//kane prasino to fanari toy mikrou dromou
			
			w=0;
			TCA0.SINGLE.CNT = 0; //clear counter
			TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc)
			TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; 
			TCA0.SINGLE.CTRLA |=1;//Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10)
			
			
			
			interr=0;
			
			while(w==0){
				
			}
			PORTD.OUTCLR= PIN1_bm;//kane kokkino to fanari twn pezwn
			cli(); //disenable interrupts
			
			if(g==0){
				PORTD.OUTCLR= PIN2_bm;//kane kokkino to fanari toy mikrou dromou
				_delay_ms(del2);//kathusterhsh gia to pote tha ginei prassino to fanari
				PORTD.OUT |= PIN0_bm;//kane prasino to fanari toy megaloy dromou
			}
		}
	}
	
	
	
}

ISR(PORTF_PORT_vect){
	//clear the interrupt flag
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	interr=1;
	sei(); //enable interrupts
}


ISR(TCA0_CMP0_vect){
	TCA0.SINGLE.CTRLA = 0; //Disable
	//clear flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;
	w=1;
	sei(); //enable interrupts
}
