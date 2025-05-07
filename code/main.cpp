#include "mbed.h"

#define BLINKING_RATE 250ms
#define LOCKOUT_TIME 60s

int main()
{
    int alarmState = 0;
    int emergency = 0;
    int attempts = 0;
    
    Timer lockoutTimer;

    // Inputs
    DigitalIn d1(BUTTON1);
    DigitalIn d2(D2); 
    DigitalIn d3(D3, PullDown); 
    DigitalIn d4(D4, PullDown); 
    DigitalIn d5(D5); 
    DigitalIn d6(D6); 
    DigitalIn d7(D7); 
    DigitalIn enterButton(D9); 

    // Outputs
    DigitalOut led1(LED1); 
    DigitalOut led2(LED2); 
    DigitalOut led3(LED3); 
    DigitalOut incorrectCodeLed(LED2); 
    DigitalOut systemBlockedLed(LED3); 

    // Set pull-down resistors
    d1.mode(PullDown);
    d2.mode(PullDown);
    d3.mode(PullDown);
    d4.mode(PullDown);
    d5.mode(PullDown);
    d6.mode(PullDown);
    d7.mode(PullDown);
    
    
    led1 = 0;  
    incorrectCodeLed = 0;  
    systemBlockedLed = 0;  

    while (true) {
        if (d3.read() && d4.read()) {
            emergency = 1;
        }
        else if (d3.read() || d4.read()) {
            alarmState = 1;
      
        }

       
        if (emergency == 1) {
            while (emergency) {
                led1 = !led1;
                ThisThread::sleep_for(BLINKING_RATE);

               
                if (d5.read() && d6.read() && !d7.read() && d2.read()) {
                    emergency = 0;
                    alarmState = 0;
                    led1 = 0;
                    attempts = 0; 
                    break;
                }
                 if (d2.read()){
                attempts++;
                
              if (attempts == 5) {
            lockoutTimer.start();  

            while (lockoutTimer.elapsed_time() < LOCKOUT_TIME) {
                led3 = !led3;  
                ThisThread::sleep_for(BLINKING_RATE);  
            }

           
            led3 = 0;
            attempts = 0;  
            lockoutTimer.stop();
            lockoutTimer.reset();  

            }
        }
            }
        }

       
        if (emergency == 0) {
            led1 = alarmState;
        }

        ThisThread::sleep_for(100ms); 
    }
}
