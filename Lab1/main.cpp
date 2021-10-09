#include "mbed.h"
#include "amaradiaga_binaryutils.hpp"
#include "USBSerial.h"

/*
- RED LED: PIN 24
- GREEN LED: PIN 16
- BLUE LED: PIN 6
*/
#define RED (uint32_t*)0x50000760 
#define GREEN (uint32_t*)0x50000740 
#define BLUE (uint32_t*)0x50000718 
#define DIRSET (uint32_t*)0x50000514
#define OUTSET (uint32_t*)0x50000508
#define OUTCLR (uint32_t*)0x5000050C
uint32_t count1 = 0;


int vdc=333;
typedef struct {
    uint32_t period;
    int van_dc; //duty cycle for vanilla
    int choc_dc; //duty cycle for chocolate
} message_t;

MemoryPool<message_t, 9> VNpool; //max 9 elements
/*queues for each vanilla, chocolate, strawberry*/
Queue<message_t, 9> VNqueue;
Queue<message_t, 9> CHqueue;
Queue<message_t, 9> STqueue;
Ticker tick;
Semaphore qMessage(1); //wait until resources become available to get message

PwmOut blue(LED4);

Thread thread1, thread2, thread3;
USBSerial serial;

void toggle(){
    if(count1 <= 333){
        setbit(OUTCLR, 16);
    }
    else{
        setbit(OUTSET, 16);
    }

    if(count1 == 1000){
        count1 = -1;
    }
    count1++;
}

//VANILLA
void Vanilla()
{
    setbit(DIRSET, 16);
    tick.attach(&toggle, 2); //attach the ADDRESS of toggle function and set interval for 2s
    while(true){
        osEvent event = VNqueue.get(0);
        if (event.status == osEventMessage) {
            message_t* vn_state = (message_t*) event.value.p;
            if(vn_state-> van_dc == 1){ //led with one third of brightness
                vdc = 2000;
            }
            else if(vn_state->van_dc == 2){
                vdc = 1400;
            }
            else if(vn_state->van_dc == 3){
                vdc = 800;
            }
            else if(vn_state->van_dc == 4){
                vdc = 200;
            }
            else if(vn_state->van_dc == 5){
                vdc = 0;
            }
            VNpool.free(vn_state);
            }

            thread_sleep_for(10);
    }
}

//STRAWBERRY
void Strawberry()
{
    while(true){}  //start with nrf_pwn_enable
}

/*CHOCOLATE : you will write a ‘Consumer’ thread named “chocolate” that creates a PWM signal to drive the blue LED using MBed’s PWMOut class. 
Like Part 2a, this thread should rely on a Queue defined in Part 2 for what percentage PWM it should display. As a test, run the LED at three-quarters brightness*/
void Chocolate()
{
float choc_dutycycle; 
    blue.period_us(500); 
    while(true){
        qMessage.acquire(); //get message, then free message and remember to set thread to sleep. 
        osEvent event = CHqueue.get(0);
        if (event.status == osEventMessage) {
            message_t* DC = (message_t*) event.value.p;
            if(DC->choc_dc == 1){
                choc_dutycycle = 1.0;
            }
            else if(DC->choc_dc == 2){
                choc_dutycycle = 0.6;
            }
            else if(DC->choc_dc == 3){
                choc_dutycycle = 0.4;
            }
            else if(DC->choc_dc == 4){
                choc_dutycycle = 0.2;
            }
            else if(DC->choc_dc == 5){
                choc_dutycycle = 0;
            }
            else if(DC->choc_dc == 6){
                choc_dutycycle = 0.2;
            }
            else if(DC->choc_dc == 7){
                choc_dutycycle = 0.4;
            }
            else if(DC->choc_dc == 8){
                choc_dutycycle = 0.6;
            }
            else if(DC->choc_dc == 9){
                choc_dutycycle = 0.8;
            }
            else if(DC->choc_dc == 10){
                choc_dutycycle = 1.0;
            }
            VNpool.free(DC);
        }
        //led.write 
        //mpool.free(dutycycle);
    }
    }


void Producer()
{
    int i = 0;
    int ch_flag = 0;
    int ch_state = 0;
    while(true)
    {
        if(ch_state == 0){
            ch_flag = 0;
        }
        if(ch_state >= 10){
            ch_flag = 1;
        }
        switch(ch_flag){
            case 0:
                    ch_state++;
                    break;
            case 1:
                    ch_state--;
                    break;
            default:
                    ch_state++;
                    break;
        }
        if(i >= 100){
            i = 0;
        }
        serial.printf("producer loop \r\n");

        message_t *message = VNpool.alloc();
        //message_t *message = Cmpool.alloc();
        //message_t *message = Smpool.alloc();
        message->choc_dc = ch_state;
        message->van_dc = ch_state;
        serial.printf("count:   %i\r\n", i);
        i++;
        VNqueue.put(message);
        CHqueue.put(message);
        thread_sleep_for(100);

    }
}


// main() runs in its own thread in the OS
int main()
{
    thread1.start(callback(Producer));
    thread2.start(callback(Vanilla));
    thread3.start(callback(Chocolate));

    
    while (true) {

    }
}

