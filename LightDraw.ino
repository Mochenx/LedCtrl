#include <LedCtrl.h>

#define LED_NUM 1
#define LED1_R_PIN 16
#define LED1_G_PIN 15
#define LED1_B_PIN 14

int wr_red(pRGB * );
int stopPin = 2;//Interrupt pin

int state = 1;
//Declarations of funtions for controlling LEDs
//int stop();
void wr_led_0();
pLED p_all_leds[LED_NUM];

void setup()   {                
    pattern hold_red;
    LED led1;
    int i;

    //Initialize pattern objects
    new_patn(&hold_red,&wr_red);
    //Initialize led1 object
    new_led(&led1,
        LED1_R_PIN,LED1_G_PIN,LED1_B_PIN,
        &hold_red);
    p_all_leds[0] = &led1;//!!!!!!!!TEMP CODE

    for(i = 0;i< 3;i ++)
        pinMode(led1.pins[i],OUTPUT);
    //pinMode(stopPin, INPUT);
    //attachInterrupt(stopPin,stop,CHANGE);
}

void loop()                     
{
    int i;
    pRGB wr_rgb;

    for(i = 0;i < LED_NUM;i ++) {
        if(state) {
            wr_rgb = step(p_all_leds[i]);
            analogWrite(p_all_leds[i]->pins[0], wr_rgb->R);
            analogWrite(p_all_leds[i]->pins[1], wr_rgb->G);
            analogWrite(p_all_leds[i]->pins[2], wr_rgb->B);
        }
    }
    delay(500);
}

//void stop() {
//    state = !state;
//}
int wr_red(pRGB * cmd_queue) {
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB));
    (*cmd_queue)->R = 255;
    (*cmd_queue)->G = 255;
    (*cmd_queue)->B = 255;
    return 1;
    //analogWrite(redPin, 30);
    //delay(500);
    //analogWrite(greenPin, 200);
    //delay(500);
    //analogWrite(bluePin, 40);
    //delay(500);
    //analogWrite(redPin, 150);
    //delay(500);
    //analogWrite(greenPin, 0);
    //delay(500);
    //analogWrite(bluePin, 250);
    //delay(500);
}
