#include <avr/io.h>

class servo
{
public:
    void begin()
    {
    }
    void servo_write(int angle)
    {
    }
};
class usart
{
public:
    void usart_init(unsigned long baud)
    {
        if (choice == 9600)
        {
            UCSR0A = 0X00;
            UCSR0B = (1 << RXEN0) | (1 << TXEN0);
            UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
            UBRR0 = 103; //Set baud rate to 9600
        }
    }
    void usart_print(String data)
    {
        for (int i = 0; data[i] != '\0'; i++)
        {
            while (!(UCSR0A & (1 << UDRE0)))
                ;
            UDR0 = data[i];
        }
    }
    char usart_read()
    {
        while (!(UCSR0A & (1 << RXC0)))
            ;
        return UDR0;
    }
};

int main()
{
    return 0;
}