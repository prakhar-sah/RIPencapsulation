//#pragma DATA_SECTION(ticks, ".ipe_vars")
volatile unsigned long ticks __attribute__((section(".ipe_vars"))) = 0;
//#pragma DATA_SECTION(start, ".ipe_vars")
volatile unsigned long start __attribute__((section(".ipe_vars")));

// Timer A0 interrupt service routine
//#pragma CODE_SECTION(TimeA0, ".ipe:_isr")
//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void TimeA0 (void)
void __attribute__((interrupt(TIMER0_A0_VECTOR), section(".ipe_isr"))) TimeA0 (void)
{
  ticks++;
}

#define startTimer() TA0CCTL0 = CCIE;                 /* CCR0 interrupt enabled */    \
                     TA0CTL = TASSEL_2 + MC_1 + ID_3; /* SMCLK/8 (1 MHz/8), upmode */ \
                     TA0CCR0 =  12;                  /* ~ 10 KHz */                     \
                     __bis_SR_register(GIE);         /* Enable all interrupts */     \
                     start = ticks

#define getElapsedTime(elapsed) elapsed = (ticks - start)
