#include "mbed.h"
// #include "mbed2/299/TARGET_NUCLEO_F303RE/TARGET_STM/TARGET_STM32F3/TARGET_NUCLEO_F303RE/PinNames.h"

#define INIT_ID 	0x100
#define DRIVE_ID 	0x102
#define POS_ID 		0x101

DigitalOut led1(LED1);
CAN can(D15, D14);
char data[8] = {1,0,0,0,0,0,0,0};
float val = 0;
float pi = 3.14159265;
DigitalIn usr_btn(PC_13);
InterruptIn init_btn(PA_15);
InterruptIn run_btn(PB_7);
 
void init() {
    char init_data[8] = {1,0,0,0,0,0,0,0};
    can.write(CANMessage(INIT_ID, init_data, 8));
    led1 = !led1;
}

void run() {
    char run_data[8] = {1,0,0,0,0,0,0,0};
    can.write(CANMessage(DRIVE_ID, run_data, 8));
    led1 = !led1;
}

int main() {
    init_btn.mode(PullUp);
    init_btn.fall(&init);
    run_btn.mode(PullUp);
    run_btn.fall(&run);
    // printf("main()\n");
    // char counter = 0;
    can.frequency(1000000);
    // CANMessage msg;
    led1 = 1;
    while(1) {
        // printf("loop()\r\n");
        // if(can.read(msg)) {
        //     for(int i = 0; i < msg.len; i++){
        //         printf("%d, ", msg.data[i]);
        //     }
        // } 
        // led1 =! usr_btn;
        if(!usr_btn){
            if(val < pi/2){
                val += 0.003;
            }else{
                val = pi/2;
            }
        }else{
            if(val > 0){
                val -= 0.003;
            }else{
                val = 0;
            }
        }
        wait(0.01);
        memcpy(&data[4], &val, 4);
        can.write(CANMessage(POS_ID, data, 8));
        
    }
}