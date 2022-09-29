#include <stdint.h>
#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"
#include "../inc/Motor.h"
#include "../inc/BumpInt.h"
#include "../inc/TExaS.h"
#include "../inc/TimerA1.h"
#include "../inc/TimerA2.h"
#include "../inc/Reflectance.h"
#include "../inc/UltrasonicSensorMultiple.h"
#include "../inc/TA3InputCapture.h"
#include "../inc/Nokia5110.h"
#include "../inc/Tachometer.h"

int turning = 0;
int RightSteps;
int LeftSteps;
int rightSpeedOffset;
int leftSpeedOffset;
int forwardSpeed;
int speedChanged;
//Wall Sprint
void WallSprint(void){
    if(lineCrossed < 2 && turning == 1 && RightSteps < TURNRISINGEDGES){
        int turningSpeed = 10000 * (1 - RightSteps/TURNRISINGEDGES);
        Motor_Left(turningSpeed,turningSpeed+RIGHTMOTOROFFSET);
    }
    else if(lineCrossed < 2 && turning == 1 && RightSteps > TURNRISINGEDGES){
        TIMER_A3->CTL &= ~0x0030;
        Motor_Stop();
        turning = 0;
        while(Reflectance_Read(1000) != 0){
            Motor_Forward(forwardSpeed + leftSpeedOffset, forwardSpeed + rightSpeedOffset);
        }
        CollisionFlag = 1;

    }
    else if(lineCrossed < 2){
        Motor_Forward(forwardSpeed + leftSpeedOffset, forwardSpeed + rightSpeedOffset); //3000, 3065
    }
    else if(lineCrossed == 2 && CollisionFlag == 1){
        Motor_Stop();
    }

}

#define P2_4 (*((volatile uint8_t *)(0x42098070)))
#define P2_3 (*((volatile uint8_t *)(0x4209806C)))
#define P2_2 (*((volatile uint8_t *)(0x42098068)))
#define P2_1 (*((volatile uint8_t *)(0x42098064)))
#define P2_0 (*((volatile uint8_t *)(0x42098060)))
#define P1_0 (*((volatile uint8_t *)(0x42098040)))

void countRightTach(void){
    P1_0 = P1_0^0x01;           // thread profile, P2.0
    RightSteps++;
}
int countLeftTach(void){
    P2_0 = P2_0^0x01;
    LeftSteps++;
}
int speedDirection;
//int turning =0;
void BumpWall(void){
    if(((CollisionFlag == 0 && lineCrossed == 0) || (CollisionFlag == 1 && lineCrossed > 0)) && turning == 0){
        Motor_Stop();
        CollisionData = Bump_Read();
        Clock_Delay1ms(1);
        while(Bump_Read() > 0){
        }
    }
    else if(CollisionFlag == 0 && lineCrossed == 1 && turning == 0){
        Motor_Stop();
        Clock_Delay1ms(1);
        Motor_Backward(3000,3000+RIGHTMOTOROFFSET);
        while(Bump_Read() > 0){

        }
        Motor_Stop();
        Clock_Delay1ms(1);
        Motor_Left(5000,5000+RIGHTMOTOROFFSET);
        RightSteps = 0;
        turning = 1;
        TimerA3Capture_Init01(&countLeftTach, &countRightTach);
        //Clock_Delay1ms(1680);//1650
    }
}

void ReflectanceCheck3(void){
    reflectanceRead = Reflectance_Read(1000);
    if(reflectanceRead >= 3 && reflectanceRead <= 15){//3 //1 // || reflectanceRead == 6 || reflectanceRead == 12
        speedChanged = 1;
        rightSpeedOffset += 30* (1- reflectanceRead/15);
    }
    else if (reflectanceRead > 15 && reflectanceRead < 240){ //192 //128 // || reflectanceRead == 96 || reflectanceRead == 48
        speedChanged = 1;
        leftSpeedOffset+= 30 * reflectanceRead/240;
    }
    else if (reflectanceRead == 0 && speedChanged == 1){
        leftSpeedOffset = LEFTMOTOROFFSET;
        rightSpeedOffset = RIGHTMOTOROFFSET;
        speedChanged = 0;
    }
    if(lineCrossed == 0 && reflectanceRead == 255){
        lineCrossed = 1;
        lineCrossing = 1;

    }
    else if (lineCrossed == 1 && reflectanceRead < 255){
        lineCrossing = 0;
    }
    else if(lineCrossed == 1 && lineCrossing == 0 && reflectanceRead == 255 && CollisionFlag == 1){
        lineCrossing = 1;
        lineCrossed = 2;
        Clock_Delay1ms(750);
        Motor_Stop();

    }
}

int main(void){
    Clock_Init48MHz();
    LaunchPad_Init();
    Motor_Init();
    Reflectance_Init();
    TExaS_Init(LOGICANALYZER_P7);
    turning = 0;
    CollisionFlag = 0;
    lineCrossing = 0;
    lineCrossed = 0;
    turning = 0;
    RightSteps = 0;
    LeftSteps = 0;
    rightSpeedOffset = RIGHTMOTOROFFSET;
    leftSpeedOffset = LEFTMOTOROFFSET;
    forwardSpeed = 12000;
    speedChanged = 0;
    speedDirection = 1;
    while(Bump_Read() == 0){

    }
    TimerA2_Init(&ReflectanceCheck3,5000);
    BumpInt_Init(&BumpWall);
    EnableInterrupts();

    while(1){
        WallSprint();
    }

}