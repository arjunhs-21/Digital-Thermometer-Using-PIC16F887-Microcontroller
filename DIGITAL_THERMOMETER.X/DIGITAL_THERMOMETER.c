/*
 * File:   DIGITAL_THERMOMETER.c
 * Author: arjun
 *
 * Created on 17 April, 2025, 12:36 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#include <xc.h>
#define RS RE0 
#define EN RE1
int high,low,temp=0;
int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,g1,h1,i1,j1,k1,m1,l1,n1;

void delay(unsigned int a)
{
    while(a--);
}
void enable()
{
    EN = 1;
    delay(50);
    EN = 0;
    delay(50);
}void lcd(char rs,char dta)
{
    RS = rs;
    PORTD = dta;
    enable();
}
void lcd1(char *dta)
{
    while(*dta)
    {
      RS = 1;
      PORTD = *dta++;
      enable();  
    }
}
void string(char *str)
{
    while(*str)
    {
        TXREG = *str++;
        while(TXIF==0);
    }
}
void adc()
{
    ADCON0 = 0X81;
    ADCON1 = 0X90;
    high = ADRESH;
    low = ADRESL;
    GO = 1;
    while(GO)
    {
     if(high == 0)
        {
            temp = low; // ADRESl + 0
        }
        if(high == 1)
        {
            temp = low +256;
        }
        if(high == 2)
        {
            temp = low + 512; // ADRESl + 512
        }
        if(high == 3)
        {
            temp = low + 768; // ADRESl + 768
        }
    }
}
void sample()
{
    adc();
    a = temp/1000;
    b = temp%1000;
    c = b/100;
    d = b%100;
    e = d/10;
    f = d%10;
    
    lcd(0,0X80);
    lcd1(" SAMPLING: ");
    lcd(1,a+48);
    lcd(1,c+48);
    lcd(1,e+48);
    lcd(1,f+48);
}
void cel()
{
    sample();
        g = temp;
        h = (float)g*(150.0/1023.0);//celcius formula
        o = ((float)h*9.0/5.0)+32.0;//fahrenheit formula
        i = o/1000;
        j = o%1000;
        k = j/100;
        l = j%100;
        m = l/10;
        n = l%10;
        
        lcd(0,0X90);
        lcd1("FAHREN:");
        lcd(1,k+48);
        lcd(1,m+48);
        lcd(1,n+48);
        lcd(1,'.');
        lcd(1,i+48);
        lcd(1,' F');
        
        i1 = h/1000;
        j1 = h%1000;
        k1 = j1/100;
        l1 = j1%100;
        m1 = l1/10;
        n1 = l1%10;
        
        lcd(0,0XC0);
        lcd1(" CELCIUS: ");
        lcd(1,k1+48);
        lcd(1,m1+48);
        lcd(1,n1+48);
        lcd(1,'.');
        lcd(1,i1+48);
        lcd(1,'C'); 
}
void split(int a)
{
    TXREG = ((a%100)/10)+48;
    while(TXIF==0);
    TXREG = (a%10)+48;
    while(TXIF==0);
    string(".");
    TXREG = (a/100)+48;
    while(TXIF==0);
}
void split1(int a)
{
    TXREG = ((a%1000)/100)+48;
    while(TXIF==0);
    TXREG = ((a%100)/10)+48;
    TXREG = (a%10)+48;
    while(TXIF==0);
    string(".");
    TXREG = (a/1000)+48;
    while(TXIF==0);
    
}
void main()
{
    PORTA=PORTB=PORTC=PORTD=PORTE=0X00;
    TRISA=0XFF;TRISB=0X03;TRISC=TRISD=TRISE=0X00;
    ANSEL=0X01;ANSELH=0X00;
    SPBRG = 103;//BRAUD RATE (9600)
    BRG16=SPEN=1;//BRG16-16BIT BAUD RATE ; SPEN = SERIAL PORT ENABLE (TX,RX)
    TXSTA = 0X26;
    SSPSTAT = 0x80;
    SSPCON  = 0X28;
    SSPCON2 = 0X00;
    lcd(0,0X0E);
    lcd(0,0X38);
    lcd(0,0X80);
    while(1)
    {
        cel();
        if(RB0==1)
        {
            string("AT\r");
            for(int i=0;i<40;i++)
            {
                delay(2000);
            }
            string("AT+CMGF=1\r");
            for(int i=0;i<40;i++)
            {
                delay(2000);
            }   
            string("AT+CMGS=\"+918838706469\"\r");
            for(int i=0;i<40;i++)
            {
                delay(2000);
            }
            string("TEMPERATURE:");
            split1(h);
            
            string(" C\r");
            for(int i=0;i<40;i++)
            {
                delay(2000);
            }
            string("FAHRENHIET:");
            split1(o);
            string(" F\r");
            for(int i=0;i<40;i++)
            {
                delay(2000);
            }
            TXREG = 26;
            while(TXIF==0);
            while(RB0==1);
        }
    }
}