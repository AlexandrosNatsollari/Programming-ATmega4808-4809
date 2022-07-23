#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>


int counter=0;
int interr=0;
int main(){
	
	
	//LED1 gia eutheia
	PORTD.DIR |= PIN1_bm; //PIN is output
	PORTD.OUT |= PIN1_bm;
	
	//LED2 gia aristera
	PORTD.DIR |= PIN2_bm; //PIN is output
	
	//LED3 gia dexia
	PORTD.DIR |= PIN3_bm; //PIN is output
	
	
	
	
	
	

	while(counter<8 && counter!=-1){
		//pullup enable and Interrupt enabled with sense on both edges
		PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
		sei(); //enable interrupts
		
		printf("%d",counter);
		
		//initialize the ADC for Free-Running mode
		ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
		ADC0.CTRLA |= ADC_FREERUN_bm; //Free-Running mode enabled
		ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
		ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //The bit
		//Enable Debug Mode
		ADC0.DBGCTRL |= ADC_DBGRUN_bm;
		//Window Comparator Mode
		ADC0.WINLT |= 10; //Set threshold
		ADC0.INTCTRL |= ADC_WCMP_bm; //Enable Interrupts for WCM
		ADC0.CTRLE |= ADC_WINCM0_bm; //Interrupt when RESULT < WINLT
		sei();
		ADC0.COMMAND |= ADC_STCONV_bm; //Start Conversion
		
		//pullup enable and Interrupt enabled with sense on both edges
		//PORTC PIN5 SWITCH gia interrupt gia na ksekinisei anapodh poreia
		PORTC.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
		
		if(interr==1){
			while(counter>(-1)){
				
				printf("%d",counter);
				//pullup enable and Interrupt enabled with sense on both edges
				PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
				sei(); //enable interrupts
				
				//initialize the ADC for Free-Running mode
				ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
				ADC0.CTRLA |= ADC_FREERUN_bm; //Free-Running mode enabled
				ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
				ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //The bit
				//Enable Debug Mode
				ADC0.DBGCTRL |= ADC_DBGRUN_bm;
				//Window Comparator Mode
				ADC0.WINLT |= 10; //Set threshold
				ADC0.INTCTRL |= ADC_WCMP_bm; //Enable Interrupts for WCM
				ADC0.CTRLE |= ADC_WINCM0_bm; //Interrupt when RESULT < WINLT
				sei();
				ADC0.COMMAND |= ADC_STCONV_bm; //Start Conversion
				
				
				
			}
		}
		
		
		
	}
	
	//entolh gia na kleisei to LED1 afou ektelestei
	//kai na termatisei h diadikasia
	PORTD.OUTCLR= PIN1_bm;
	printf("%d",counter);
}

//Se ayto to interrupt ginontai allages twn LED
//sumfwna me ton aisththra(ADC se emas) poy deixnei apostash apo ton toixo
//kai analoga thn poreia ths syskeuhs(kanonikh h anapodh dhladh)
ISR(ADC0_WCOMP_vect){
	int intflags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = intflags; //edw thelei breakpoint kai
	//molis ftasei se auto to breakpoint to run thelei Allagh h timh
	//tou ADC se kati megalutero tou 10 gia na treksei mia fora
	//an den allaksei twra tote trexei perissoteres apo 1 fores
	if(interr==1){
		PORTD.OUTCLR= PIN1_bm; //kleinei to LED1 molis ektelestei
		PORTD.OUT |= PIN3_bm;//anoigei to LED3 molis ektelestei
		_delay_ms(5);
		PORTD.OUTCLR= PIN3_bm; //kleinei to LED3 molis ektelestei
		PORTD.OUT |= PIN1_bm;//anoigei to LED1 molis ektelestei
		counter--;
	}
	else if (interr==0){
		PORTD.OUTCLR= PIN1_bm; //kleinei to LED1 molis ektelestei
		PORTD.OUT |= PIN2_bm;//anoigei to LED2 molis ektelestei
		_delay_ms(5);
		PORTD.OUTCLR= PIN2_bm; //kleinei to LED2 molis ektelestei
		PORTD.OUT |= PIN1_bm;//anoigei to LED1 molis ektelestei
		counter++;
	}
	
}


//Se auto to interrupt strivei deksia an erthei interrupt apo to
//PORTF PIN5 sthn kanonikh poreia h aristera an eimaste sthn anapodh
//poreia
ISR(PORTF_PORT_vect){
	//clear the interrupt flag
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	if(interr==1){
		PORTD.OUTCLR= PIN1_bm; //kleinei to LED1 molis ektelestei
		PORTD.OUT |= PIN2_bm;//anoigei to LED2 molis ektelestei
		_delay_ms(5);
		PORTD.OUTCLR= PIN2_bm; //kleinei to LED2 molis ektelestei
		PORTD.OUT |= PIN1_bm;//anoigei to LED1 molis ektelestei
		counter--;
	}
	else if (interr==0){
		PORTD.OUTCLR= PIN1_bm; //kleinei to LED1 molis ektelestei
		PORTD.OUT |= PIN3_bm;//anoigei to LED3 molis ektelestei
		_delay_ms(5);
		PORTD.OUTCLR= PIN3_bm; //kleinei to LED3 molis ektelestei
		PORTD.OUT |= PIN1_bm;//anoigei to LED1 molis ektelestei
		counter++;
	}
	
}


//Se auto to interrupt ginetai anastrofh(strofh 180 moires afou anoigoun
//kai ta 3 LED)
//kai energopoiei thn anapodh poreia
ISR(PORTC_PORT_vect){
	//clear the interrupt flag
	int intflags = PORTC.INTFLAGS;
	PORTC.INTFLAGS=intflags;
	//To LED1 den xreiazetai na to anoiksoume einai hdh anoixto
	PORTD.OUT |= PIN2_bm;//anoigei to LED2 molis ektelestei
	PORTD.OUT |= PIN3_bm;//anoigei to LED3 molis ektelestei
	_delay_ms(10);
	PORTD.OUTCLR= PIN2_bm; //kleinei to LED2 molis ektelestei
	PORTD.OUTCLR= PIN3_bm; //kleinei to LED3 molis ektelestei
	interr=1;
	counter--;
}
