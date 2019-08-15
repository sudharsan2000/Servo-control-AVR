#include <avr/io.h>

#define OCR1_2ms 3999
#define OCR1_1ms 1800

class servo
{
public:
    void begin(int pin)
    {
        if (pin == 9)
        {
            /*Set pre-scaler of 8 with Fast PWM (Mode 14 i.e TOP value as ICR1)  non-inverting mode */
            DDRB |= 1 << PINB1;
            TCCR1A |= (1 << WGM11) | (1 << COM1A1);
            TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

            ICR1 = 39999; // Set pwm period as 2ms
        }
    }
    void write(int angle, int offset = 800)
    {
        OCR1A = map(angle, 0, 180, OCR1_1ms - offset, OCR1_2ms + offset);
    }
};
class usart
{
public:
    void init(unsigned long baud = 9600)
    {
        if (baud == 9600)
        {
            UCSR0A = 0X00;
            UCSR0B = (1 << RXEN0) | (1 << TXEN0);
            UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
            UBRR0 = 103; //Set baud rate to 9600
        }
    }
    void print(String data)
    {
        for (int i = 0; data[i] != '\0'; i++)
        {
            while (!(UCSR0A & (1 << UDRE0)))
                ;
            UDR0 = data[i];
        }
    }
    char read()
    {
        while (!(UCSR0A & (1 << RXC0)))
            ;
        return UDR0;
    }
};
servo D9servo;
usart serial;
volatile int overflows=0;
ISR(TIMER2_OVF_vect)
{
  ++overflows;
}
void delayinms(int time_)
{
  overflows = 0;
  TCCR2B = 1 << CS21 | 1 << CS20;      //32 bit prescale gives 0.512ms per overflow
  while (overflows * 0.512 <= time_) // wait till count of overflows equals time_
    ;
  TCCR2B = 0; // turn timer off after use
}
int main()
{
    D9servo.begin(9);
    TIMSK2 = 1 << TOIE2;
    sei();
    serial.init();
    while (1)
    {
        serial.print("0 start\n");
  D9servo.write(0);
       serial.print("0 end\n");
           delayinms(2000);
       serial.print("180 start\n");
           D9servo.write(180);
       serial.print("180 end\n");
        delayinms(2000);
    }
    return 0;
}