#include "mbed.h"
#include "amaradiaga_binaryutils.hpp"
#include "USBSerial.h"

#define PIN0 (uint32_t*)0x50000000 //GPIO port, location found in manual
#define DIRSET (uint32_t*)0x50000514 //DIR set register
#define OUTSET (uint32_t*)0x50000508 //Set individual bits in GPIO port
#define OUTCLR (uint32_t*)0x5000050C //Clear individual bits in GPIO port

/*
- Author: Anya Maradiaga
- Date: Tuesday September 21, 2021
- Description: The following project shows how to create a handler thread with a diagnostic LED. 
*/

//Three different LED blinking speeds for diagnostics
typedef enum {
    NO_ERROR,
    ATTN_REQ,
    FATAL_ERROR,
} 
State;

//use mail class
Mail<State, 9> mail_box;
//creating thread
Thread thread; 

USBSerial serial;

/*leg_diag_handler: thread responsible for driving the LEDs in different runtime states*/
void led_diag_handler() 
{
    setbit(DIRSET, 14);
    uint32_t t1 = 5000;

    while(true)
    {
        serial.printf("This is led_diag_handler \r\n");
        osEvent event = mail_box.get(0);
        serial.printf("Mail_boxget \r\n");
        State* priority = (State*) event.value.p;
        serial.printf("Mail for priority: %i \r\n", priority);
        if(*priority == NO_ERROR)
        {
            serial.printf("NO_ERROR\r\n");
            t1 = 2000;
            mail_box.free(priority); 
        }
        if(*priority == ATTN_REQ)
        {
            serial.printf("ATTN_REQ\r\n");
            t1 = 1000;
            mail_box.free(priority);
        }
        if(*priority == FATAL_ERROR)
        {
            serial.printf("FATAL_ERROR\r\n");
            t1 = 500;
            mail_box.free(priority);
        }

        //handoff cpu to other threads
        setbit(OUTSET, 14);
        thread_sleep_for(t1); 
        setbit(OUTCLR, 14);
        thread_sleep_for(t1);
    }
}

/*diag_tester: thread responsible for simulating different runtime states*/
void diag_tester() 
{
    int count = 0;
    while(true)
    {
    serial.printf("This is diag_tester \r\n");
    //serial.printf("Counter: %i\n", count); 
    serial.printf("Counter: %i\r\n", count); //check counter
    State* priority = mail_box.try_alloc();
        if(count == 0)
        {
            *priority = NO_ERROR;
            serial.printf("setting priority -> NO_ERROR \r\n");
        }
        if(count == 1)
        {
            *priority = ATTN_REQ;
            serial.printf("setting priority -> ATTN_REQ \r\n");
        }
        if(count == 2)
        {
            *priority = FATAL_ERROR;
            serial.printf("setting priority -> FATAL_ERROR \r\n");
            count = -1;
        }
    count++;
    serial.printf("Mail_boxput: \r\n");
    mail_box.put(priority);
    serial.printf("Thread sleep function \r\n");
    thread_sleep_for(5000); 
    }
}

// main() runs in its own thread in the OS
int main()
{
    serial.printf("Start of thread \r\n");
    thread.start(callback(diag_tester)); //using diag_tester thread
    thread.start(callback(led_diag_handler)); //using led_diag_handler thread
}

