/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
#include <stdlib.h>
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 0
//Week 2 - Assignment 3
void zmain(void)
{
    ADC_Battery_Start();        
    
    int16 adcresult = 0;
    float volts = 0.0;

    printf("\nBoot Successfully!\n");
  
    uint8 button;
    button = SW1_Read();

    for(;;)
    {
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {
            adcresult = ADC_Battery_GetResult16();
            volts = adcresult * 5.00 *1.50 / 4098;
            printf("ADC result: %d\nVoltage: %.1f V\n\n",adcresult, volts);
        }
        
        if (volts < 4.0) {
            if (SW1_Read() == 0) {
                BatteryLed_Write(1);
                vTaskDelay(4000);
                //turn on Led light for 4s.
            } else {
                while(SW1_Read() == 1) {
                    BatteryLed_Write(1);
                    vTaskDelay(500);
                    BatteryLed_Write(0);
                    vTaskDelay(500);
                }
            }
        } else {
            BatteryLed_Write(1);
            vTaskDelay(2000);
        }
    }
 }   
#endif

#if 0
//Week 3 - Assignment 1
void zmain(void)
{
    motor_start();
    motor_forward(0,0);

    vTaskDelay(3000);
    
    motor_forward(150,2400); 
    motor_turn(165,50,750);
    motor_forward(150,1500); 
    motor_turn(170,50,780);
    motor_forward(150,1700);
    motor_turn(125,45,2500);
    motor_turn(45,100,900);
    motor_turn(125,45,1000);
    motor_forward(130,1000);
    motor_forward(0,0);
    
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    for(;;)
    {

    }
}
#endif

#if 0
//Week 3 - Assignment 2
void zmain(void)
{
    vTaskDelay(2000);
    Ultra_Start();
    motor_start();
    
    while(1) {
        motor_forward(0,0);
        motor_forward(150,1000);
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\r\n", d);
        
        if (d <= 10) {
            
        vTaskDelay(200);
    }
}   
    

#if 0 
    //Week 3 - Assignment 3
   
//motor
void zmain(void)
{
   IR_Start();
    
    uint32_t IR_val; 
    struct accData_ data;
    int random3, random2, random_direction1, random_direction2;
    int speed = 8; //multiplied in code by 20, max speed 8
    int adjustX=2600;
    int previousX = 0;
    int previousY = 0; // for saving accelerometer data for comparison between iterations
    int backward_time = 700; // in ms, when bumping time of backward movement
    
    IR_flush(); // clear IR receive buffer
    
    // checking if the accelerometer works
    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    //check end
    
    //start button loop
    for (;;)
    {
       // if (SW1_Read() == 0) {break;} // user button
       if (IR_get(&IR_val, portMAX_DELAY)) {break;} //infrared input
    }
    vTaskDelay(500); //pause after activation
    IR_flush();
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors
    // movement loop start, forward movement and turn loops inside
    for (;;) {
        //declaration of random values for each movement iteration
        random3 = rand() % 21 + 10; //*100ms - random value for forward movement time  (1-3s)
        random2 = rand() % 21;  //*100ms - random value for turning movement time (0-2s)
        printf("random3 = %2d , random2 = %2d\n", random3, random2);
        
        
        // here is the forward movement logic
        
        for (int i = 0; i < random3; i++)
        {               
            motor_forward(20*speed,100);
            LSM303D_Read_Acc(&data);
            if (IR_get(&IR_val, portMAX_DELAY)) {motor_stop();} //motor stop on IR input
              
            // start of collision check
            if ((previousX - data.accX) > adjustX  && i > 2)  // need testing, i>2 because of false hits when going from backward to forward motion
                {
                    motor_forward(0,0);
                    printf("Hit on X!\n");
                    motor_backward(50, 200);
                    // now if else condition for turning randomly left or right
                    if (rand()%2==0){
                        motor_turn(200, 50, backward_time);
                    }
                    else {
                        motor_turn(50, 200, backward_time); // need to adjust to 90 degree turn
                    }
                    printf("end back right turn\n");
                    break;                                 
                }
             else if (previousY - data.accY > 1000)
                {
                    printf("Hit on Y!\n");
                } //end of collision check conditionals
            
            
            printf("%8d %8d\n",data.accX, data.accY);
            previousX = data.accX; // for comparing new and old accelerometer data
            previousY = data.accY;
            //printf("Previous values: %8d %8d \n", previousX, previousY);
        } //end of forward movement loop
        
        random_direction1 = rand() % 20 + 1; // random value between 1 and 20 for the turning data
        random_direction2 = rand() % 20 + 1;
        
        motor_forward(0,0); // to make sure it is going to turn in the forward movement
        
        //beginning of the turning loop
        for (int i = 0; i < random2; i++)
        {                                     
            motor_turn(random_direction1*10, random_direction2 * 10, 100); // max values (200, 200, 100)
            LSM303D_Read_Acc(&data);
           if (IR_get(&IR_val, portMAX_DELAY)) {motor_stop();}
           
            //collision check conditionals
            if ((previousX - data.accX) > adjustX && i > 2)  // need testing if 1000 is ok
                {
                    motor_forward(0,0);
                    printf("Hit on X!\n");
                    motor_backward(50, 200);
                    // now if else condition for turning randomly left or right
                    if (rand()%2==0){
                        motor_turn(200, 50, backward_time);
                        printf("end back left turn\n");
                        
                    }
                    else {
                        motor_turn(50, 200, backward_time); // need to adjust to 90 degree turn
                        printf("end back right turn\n");
                    
                    }
                    break; 
                                                    
                }
            else if (previousY - data.accY > 1000)
                {
                    printf("Hit on Y!\n");
                }
            //end of collicion check conditionals
            printf("Turn %8d %8d\n",data.accX, data.accY);
            previousX = data.accX; // for comparing new and old accelerometer data
            previousY = data.accY;
        }
        
        printf("Major iteration done \n");
        //motor_forward(0,0);
        //motor_stop();
        //for (;;) {}
        
    
    } // end of infinite loop
    
    
        
    for(;;)
    {

    }
}
#endif

#endif

#if 0
    //Week 4 - Assignment 1
    void goTillBlack(int speed);
    void reachEndOfLine(int speed);
    void zmain(void)
    {
    
   //IR_Start();  

   //uint32_t IR_val; 
   //struct sensors_ ref;
   struct sensors_ dig;

   //IR_flush(); // clear IR receive buffer
   int speed = 5; // speed values 0 to 20
    
    
    //start button loop
    //for (;;)
    //{
        //if (SW1_Read() == 0) {break;} // user button
    //}
    vTaskDelay(500); //pause after activation
    // start motor, move forward untill the line
    motor_start();
   

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 9000, 9000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000 nm    

    for (;;)
    {
        //reflectance_read(&ref); // read reflectance sensor data
        reflectance_digital(&dig);  //read reflectance sensor data in digital form
        //condition for seeing the black line with all of the reflectance sensors
        if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1 && dig.r3){break;}
        motor_forward(10*speed, 50);
    }
    motor_forward(0,0);
    motor_stop();
    // stop on the first line, waiting for IR input to continue
    //IR_wait();  //wait for IR input, temporary take out
    vTaskDelay(3000);
    
    motor_start();
    // reach the end of the black line
    
    reachEndOfLine(speed);
    goTillBlack(speed);
    reachEndOfLine(speed);
    goTillBlack(speed);
    reachEndOfLine(speed);
    goTillBlack(speed);
    
    motor_forward(0,0);
    motor_stop();
    for(;;) {
    vTaskDelay(1000);
    }
    }
    void goTillBlack(int speed){
        struct sensors_ dig;
        for (;;)
        {
        reflectance_digital(&dig);  //read reflectance sensor data in digital form
        //condition for seeing the black line with all of the reflectance sensors
        if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1 && dig.r3 == 1){break;}
        motor_forward(10*speed, 50);
        }
    
    }
    void reachEndOfLine(int speed) {
        struct sensors_ dig;
        for (;;){
        
        reflectance_digital(&dig);  //read reflectance sensor data in digital form 
        if (dig.l3 == 0 && dig.l2 == 0 && dig.r2 == 0 && dig.r3 == 0){break;} // if true end of black line reached
        motor_forward(10*speed, 50);
    }
        
    
    }
#endif

#if 0
//Week 4 - Assignment 2
void goTillBlack(int speed);
    void reachEndOfLine(int speed);
    void zmain(void)
    {
    
   IR_Start();  

   //uint32_t IR_val; 
   //struct sensors_ ref;
   //struct sensors_ dig;

   IR_flush(); // clear IR receive buffer
   int speed = 10; // speed values 0 to 20
    
    
    //start button loop
    for (;;)
    {
        if (SW1_Read() == 0) {break;} // user button
    }
    vTaskDelay(500); //pause after activation
    // start motor, move forward untill the line
    motor_start();
   

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 9000, 9000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000 nm    

    goTillBlack(speed);
    // stop on the first line, waiting for IR input to continue
    //motor_forward(0,0);
    //motor_stop();
    //IR_wait();  //wait for IR input
    //motor_start();
    reachEndOfLine(speed);
    //
    // reach the end of the black line
    
    goTillBlack(speed);
    motor_turn(0, 120, 400); //turn left
    goTillBlack(speed);
    motor_turn(120, 0, 400); //turn right
    goTillBlack(speed);
    motor_turn(120, 0, 400); //turn right
    goTillBlack(speed);
    
    motor_forward(0,0);
    motor_stop();
    for(;;) {
    vTaskDelay(1000);
    }
    }
    void goTillBlack(int speed){
        struct sensors_ dig, ref;
        int time_stamp = 20;
        for (;;)
        {
        reflectance_read(&ref);
        reflectance_digital(&dig);  //read reflectance sensor data in digital form
        //condition for seeing the black line with all of the reflectance sensors
        if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1 && dig.r3){break;}
        printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3); 
        if (ref.r1>15000 && ref.l1>15000) {   //for a thin line go down to 8000 maybe less
        		motor_forward(10*speed, time_stamp);
        	}
        else if (ref.l2>18000) {  //for a thin line put less for all
		motor_turn(0, 100, time_stamp);
	        }
        else if (ref.r2>18000) {
        		motor_turn(100, 0, time_stamp);
        	}
        else if (ref.r1>16000 && ref.l1<16000) {
        		motor_turn(100, 0, time_stamp);
        	}
        else if (ref.l1>16000 && ref.r1<16000) {
        		motor_turn(0, 100, time_stamp);
        	}
        
        }
    
    }
    void reachEndOfLine(int speed) {
        struct sensors_ dig;
        for (;;){
        
        reflectance_digital(&dig);  //read reflectance sensor data in digital form 
        if (dig.l3 == 0 && dig.l2 == 0 && dig.r2 == 0 && !dig.r3){break;} // if true end of black line reached
        motor_forward(10*speed, 50);
    }
        
    }
#endif

#if 1
    //Week 4 - Assignment 3
    void goTillBlack(int speed);
    void reachEndOfLine(int speed);
    void zmain(void)
    {
    
   IR_Start();  

   //uint32_t IR_val; 
   //struct sensors_ ref;
   //struct sensors_ dig;

   
   int speed = 10; // speed values 0 to 20
    
    
    //start button loop
    vTaskDelay(2000);
    motor_start();
   

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 9000, 9000, 9000, 9000); // set center sensor threshold 9000 nm    
    for (;;) 
    {
         struct sensors_ dig;
        reflectance_digital(&dig);
        if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1 && dig.r3 == 1){
            motor_forward(0,0);
            vTaskDelay(2000);
            break;
        } 
        motor_forward(10*speed, 50);
    }
    vTaskDelay(3000);
    reachEndOfLine(speed);
    goTillBlack(speed);
    
    motor_forward(0,0);
    motor_stop();
    for(;;) {
    //vTaskDelay(1000);
    }
    }
    void goTillBlack(int speed){
        struct sensors_ dig, ref;
        int time_stamp = 20;
        for (;;)
        {
        reflectance_read(&ref);
        reflectance_digital(&dig);  //read reflectance sensor data in digital form
        //condition for seeing the black line with all of the reflectance sensors
        if (dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1){
            motor_forward(0,0);
            vTaskDelay(2000);
            break;
        } else {
        printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3); 
        while (ref.r1>8000 && ref.l1>8000 && ref.l2 < 3000 && ref.r2 < 3000) {   //for a thin line go down to 8000 maybe less
        		motor_forward(10*speed, 50);
                reflectance_read(&ref);
                reflectance_digital(&dig);
        	}
        if (ref.l2>8000) {  //for a thin line put less for all
		motor_turn(0, 100, 20);
        reflectance_read(&ref);
                reflectance_digital(&dig);
	        }
        else if (ref.r2>8000) {
        		motor_turn(100, 20, 20);
                reflectance_read(&ref);
                reflectance_digital(&dig);
        	}
        else if (ref.r1>8000 && ref.l1<8000) {
        		motor_turn(100, 20, 1);
                reflectance_read(&ref);
                reflectance_digital(&dig);
        	}
        else if (ref.l1>8000 && ref.r1<8000) {
        		motor_turn(20, 100, 20);
                reflectance_read(&ref);
                reflectance_digital(&dig);
        	}
        }
        
        }
    
    }
    void reachEndOfLine(int speed) {
        struct sensors_ dig;
        for (;;){
        
        reflectance_digital(&dig);  //read reflectance sensor data in digital form 
        if (dig.l3 == 0 && dig.l2 == 0 && dig.r2 == 0 && !dig.r3){break;} // if true end of black line reached
        motor_forward(10*speed, 50);
    }
        
    
    }
#endif

#if 0
   //Week 5 - Assignment 1
void zmain(void)
{    
    int hour, min;
    printf("Hour:");
    scanf("%d", &hour);
    printf("Minute:");
    scanf("%d", &min);

    RTC_Start();
    RTC_TIME_DATE now;
    now.Hour = hour;
    now.Min = min;
    RTC_WriteTime(&now); // write the time to real time clock

    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            print_mqtt("Zumo046/real time","%d:%d\n", now.Hour, now.Min);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif

#if 0
    //Week 5 - Assignment 3
    void goTillBlack(int speed);
    void reachEndOfLine(int speed);
    void zmain(void)
    {
    
   IR_Start();  
   TickType_t start, end, total;

   IR_flush(); // clear IR receive buffer
   int speed = 5; // speed values 0 to 20
    
    
    //start button loop
    for (;;)
    {
        if (SW1_Read() == 0) {break;} // user button
    }
    vTaskDelay(500); //pause after activation
    // start motor, move forward untill the line
    motor_start();
    reflectance_start(); //sub functions are using the reclectance sensors
    //reflectance_set_threshold(9000, 9000, 9000, 9000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000 nm    
    goTillBlack(speed);
    motor_forward(0,0);
    motor_stop();
    IR_wait();  //wait for IR input
    motor_start();
    start = xTaskGetTickCount();
    reachEndOfLine(speed);
    goTillBlack(speed);
    end = xTaskGetTickCount();
    total = (end - start)/1000;
    print_mqtt("Zumo046/lap", "time:, %d.2", total); ////////
    motor_forward(0,0);
    motor_stop();
    
    for(;;) {
    vTaskDelay(1000);
    }
    }
    void goTillBlack(int speed){
        struct sensors_ dig;
        for (;;)
        {
        reflectance_digital(&dig);  //read reflectance sensor data in digital form
        //condition for seeing the black line with all of the reflectance sensors
        if (dig.l3 == 1 && dig.l2 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 1 && dig.r3){break;}
        motor_forward(10*speed, 50);
        }
    
    }
    void reachEndOfLine(int speed) {
        struct sensors_ dig;
        for (;;){
            
        
        reflectance_digital(&dig);  //read reflectance sensor data in digital form 
        if (dig.l3 == 0 && dig.l2 == 0 && dig.r2 == 0 && !dig.r3){break;} // if true end of black line reached
        motor_forward(10*speed, 50);
    }
        
    
    } 
#endif



/* [] END OF FILE */
