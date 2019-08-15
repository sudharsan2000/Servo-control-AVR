#include <avr/io.h>

#define OCR1_2ms 3999
#define OCR1_1ms 2000

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
        if (choice == 9600)
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
int main()
{
    D9servo.begin();
    serial.init();
    while (1)
    {
        D9servo.write(0);
        serial.print("0\n");
        delay(5000);
        D9servo.write(1);
        serial.print("1\n");
        delay(5000);
    }
    return 0;
}