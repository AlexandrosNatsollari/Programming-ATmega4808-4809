#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ped 120

int SW5=0;
int SW6=0;
//An InitialStateFan=0 anemhsthras kleistos alliws an InitialStateFan=1 anemhsthras anoiktos
int InitialStateFan=0;
int LED1=0;
//An InitialStateWLeakage=0 to water leakage apenergopoihmeno alliws an InitialStateWLeakage=1 to water leakage einai energopoihmeno
int InitialStateWLeakage=0;
//An InitialStateBoiler=0 o thermoshfonas einai kleistos alliws an InitialStateBoiler=1 o thermoshfonas einai anoiktos
int InitialStateBoiler=0;
int interr=0;
int main() {
	PORTD.DIR |= PIN2_bm;//LED2 gia water leakage
	
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
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	//pullup enable and Interrupt enabled with sense on both edges
	PORTC.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	//LED palmwn
	PORTD.DIR |= PIN1_bm; //LED1 gia anemhsthra
		
	
	
	
	PORTD.DIR |= PIN0_bm;//LED0 gia thermoshfonas

	
	sei(); //begin accepting interrupt signals
	

	
	sei(); //enable interrupts
	while(1){
		while (SW5==0) {//boiler
			
			if (SW6==1)
			{
				if (InitialStateBoiler==0){
					//Afou InitialStateBoiler=0 o thermoshfonas einai kleistos ara:
					InitialStateBoiler=1;
					PORTD.OUT |= PIN0_bm;//Anoikse to LED0
					TCA0.SINGLE.CNT = 0; //clear counter
					TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc
					TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024
					TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; //(= 0x7<<1  )
					TCA0.SINGLE.CTRLA |=1;//Enable
					TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10)

					
				}
				else if(InitialStateBoiler==1){
					//Afou InitialStateBoiler=1 o thermoshfonas einai anoiktos ara:
					PORTD.OUTCLR = PIN0_bm;//Kleise to LED0
					InitialStateBoiler=0;
					TCA0.SINGLE.CTRLC = 0; //Disable
					
				}
				SW6=0;
				SW5++;
			}
		}
		sei();
		while (SW5==1) {//fan
	
			
			if (SW6==1)
			{
				
				if (InitialStateFan==0){
					//Afou InitialStateFan=0 o anemhsthras einai kleistos ara:
					InitialStateFan=1;
					//prescaler=1024
					TCA0.SINGLE.CTRLA=TCA_SINGLE_CLKSEL_DIV1024_gc;
					TCA0.SINGLE.PER = 254; //select the resolution
					TCA0.SINGLE.CMP1 = 90; //select the duty cycle
					
					//select Single_Slope_PWM
					TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
					//enable interrupt Overflow
					TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

					TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP1_bm;
					
					TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable


					
				}
				else if(InitialStateFan==1){
					//Afou InitialStateFan=1 o anemhsthras einai anoiktos ara:
					PORTD.OUTCLR = PIN1_bm;//Kleise to LED1
					LED1=0;
					InitialStateFan=0;
					TCA0.SINGLE.CTRLC = 0x00; //Disable
					
				}
				
				SW6=0;
				SW5++;
			}
			
		}
		sei();
		while (SW5==2) {//water leakage
			
			
			if (SW6==1)
			{
				if (InitialStateWLeakage==0)
				{
					//synexizei na einai sbhsto to LED2
				}
				else if(InitialStateWLeakage==1)
				{
					PORTD.OUTCLR = PIN2_bm;//Kleise to LED2
					InitialStateWLeakage=0;
				}
				SW6=0;
				SW5++;
			}
			
		}
		sei();
		while (SW5==3) {//house lock
			
			
			if (SW6==1)
			{
				
				SW6=0;
				SW5=0;
			}
			
		}
	}
	
}

ISR(PORTF_PORT_vect){//interrupt gia to SWITCH5
	
	//clear the interrupt flag
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	if (interr==1)
	{
		PORTD.OUT |= PIN0_bm;//Anoikse to LED0
		interr=0;
	}
	
	if (SW5==3)
	{
		SW5=0;
	}
	else{
		SW5++;
	}
}

ISR(PORTC_PORT_vect){//interrupt gia to SWITCH6
	//clear the interrupt flag
	int intflags = PORTC.INTFLAGS;
	PORTC.INTFLAGS=intflags;
	if (interr==1)
	{
		TCA0.SINGLE.CTRLA = 0; //Disable
		interr=0;
	}
	SW6=1;
}


ISR(TCA0_OVF_vect){//interrupt gia ton palmo tou anemhsthra
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	if (LED1==0)
	{
		PORTD.OUT |= PIN1_bm;//Anoikse to LED1
		LED1=1;
	}
	else if (LED1==1)
	{
		PORTD.OUTCLR = PIN1_bm;//Kleise to LED1
		LED1=0;
	}
}




ISR(ADC0_WCOMP_vect){//interrupt gia to water leakage
	int intflags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = intflags;
	
	PORTD.OUT |= PIN2_bm;//Anoikse to LED2
	InitialStateWLeakage=1;
}


ISR(TCA0_CMP0_vect){//interrupt gia ton timer tou boiler
		//clear flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;
    PORTD.OUTCLR = PIN0_bm;//Kleise to LED0
	interr=1;
	sei(); //enable interrupts
}
