#include <LedCtrl.h>

#define LED_NUM 2
#define DIP_OPT_NUM 16
#define PATN_NUM 8

//PWM C6~C4
#define LED1_R_PIN 16
#define LED1_G_PIN 15
#define LED1_B_PIN 14

//PWM B6~B4
#define LED2_R_PIN 26
#define LED2_G_PIN 25
#define LED2_B_PIN 24

#define DIP_PIN_3 4
#define DIP_PIN_2 5
#define DIP_PIN_1 6
#define DIP_PIN_0 7

//Interrupt pin
#define STOP_PIN 2

//State Varibles
volatile int running = 1;
unsigned char state = 0;

//Global Declarations for controlling LEDs
//Pointer Arrays
pLED p_all_leds[LED_NUM];
ppattern p_all_patn_f[LED_NUM][DIP_OPT_NUM];
//Instance Arrays
pattern all_patns[PATN_NUM];
LED led[LED_NUM];

void setup()   {                
    int i,k;
	int pins[LED_NUM*3] = {LED1_R_PIN,LED1_G_PIN,LED1_B_PIN,LED2_R_PIN, LED2_G_PIN, LED2_B_PIN};

	init_patns();

    //Initialize LED objects
	for(i = 0;i< LED_NUM;i ++) {
    	new_led(&led[i],(p_all_leds+i),
    	 	pins[i*3],pins[i*3+1],pins[i*3+2],
    	    p_all_patn_f[i][state]);

        for(k = 0;k< 3;k ++)
            pinMode(led[i].pins[k],OUTPUT);
    }

	//Input Pins
    pinMode(STOP_PIN, INPUT);
    pinMode(6,OUTPUT);//For Debug
    digitalWrite(6,HIGH);//For Debug
    pinMode(DIP_PIN_3, INPUT);
    pinMode(DIP_PIN_2, INPUT);
    pinMode(DIP_PIN_1, INPUT);
    pinMode(DIP_PIN_0, INPUT);

    //attachInterrupt(STOP_PIN,stop,HIGH);
}

void loop()                     
{
    int i,j;
    pRGB wr_rgb;
	unsigned char new_state;//!!!!

    new_state = get_key();//!!!!  
    for(i = 0;i < LED_NUM;i ++) {
		if((state != new_state) && (p_all_patn_f[i][new_state] != 0)){
            p_all_leds[i]->patn = p_all_patn_f[i][new_state];
            reset_led(p_all_leds[i]);
		}
        if(running) {
            wr_rgb = step(p_all_leds[i]);
            analogWrite(p_all_leds[i]->pins[0], wr_rgb->R);
            analogWrite(p_all_leds[i]->pins[1], wr_rgb->G);
            analogWrite(p_all_leds[i]->pins[2], wr_rgb->B);
        }
		else {//Turn off LEDs
    		for(j = 0;j < 3;j ++)
            	analogWrite(p_all_leds[i]->pins[j], 0);
		}
    }
    state = new_state;
    //Serial.println("Loop running");
    //delayMicroseconds(100);
    delay(100);
}

//void stop() {
//}


void init_patns() {
	int i,j;

	for(i = 0;i<LED_NUM;i ++)
		for(j = 0;j<DIP_OPT_NUM;j ++)
			p_all_patn_f[i][j] = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//Color Pattern Function Defines
	//int f_hold_white(pRGB * );
	//int f_hold_red(pRGB * );
	//int f_hold_green(pRGB * );
	//int f_hold_blue(pRGB * );
	//int f_hold_yellow(pRGB * );
	//////////////////////////////////////////////////////////////////////////

	//From sequence To patterns
    //Initialize pattern objects
    new_patn(&all_patns[0],&f_hold_white);
    new_patn(&all_patns[1],&f_hold_red);
    new_patn(&all_patns[2],&f_hold_green);
    new_patn(&all_patns[3],&f_hold_blue);
    new_patn(&all_patns[4],&f_hold_yellow);
    new_patn(&all_patns[5],&f_hold_breath);
    new_patn(&all_patns[6],&f_hold_color);

	//System State mapping to pattern
    //LED1->Pattern Mapping
	p_all_patn_f[0][0] = &all_patns[0];
	p_all_patn_f[0][1] = &all_patns[1];
	p_all_patn_f[0][3] = &all_patns[2];
	p_all_patn_f[0][7] = &all_patns[3];
	p_all_patn_f[0][15] = &all_patns[4];

	p_all_patn_f[0][2] = &all_patns[5];

    //LED2->Pattern Mapping
	p_all_patn_f[1][0] = &all_patns[0];
	p_all_patn_f[1][1] = &all_patns[1];
	p_all_patn_f[1][3] = &all_patns[2];
	p_all_patn_f[1][7] = &all_patns[3];
	p_all_patn_f[1][15] = &all_patns[4];

	p_all_patn_f[1][2] = &all_patns[5];
	p_all_patn_f[1][4] = &all_patns[6];
}

unsigned char get_key() {
	int i;
	char dip_pins[4] = {DIP_PIN_3, DIP_PIN_2, DIP_PIN_1, DIP_PIN_0};
	unsigned char dip_val = 0;

	for(i = 0;i< 4;i ++) {
		dip_val |= (digitalRead(dip_pins[i]) & 0x1);
        if(i < 3)
            dip_val = (dip_val << 1)&0xFE;
	}
    //delay(100);
    running = digitalRead(STOP_PIN);
    //Serial.println(dip_val, DEC);
	return dip_val;
}


//////////////////////////////////////////////////////////////////////////
//Color Pattern Function Defines
int f_hold_white(pRGB * cmd_queue) {
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB));
    (*cmd_queue)->R = 255;
    (*cmd_queue)->G = 255;
    (*cmd_queue)->B = 255;
	(*cmd_queue)->repeat = 255;
    return 1;
}

int f_hold_red(pRGB * cmd_queue) {
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB));
    (*cmd_queue)->R = 255;
    (*cmd_queue)->G = 0;
    (*cmd_queue)->B = 0;
	(*cmd_queue)->repeat = 255;
    return 1;
}

int f_hold_green(pRGB * cmd_queue) {
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB));
    (*cmd_queue)->R = 0;
    (*cmd_queue)->G = 255;
    (*cmd_queue)->B = 0;
	(*cmd_queue)->repeat = 255;
    return 1;
}

int f_hold_blue(pRGB * cmd_queue) {
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB));
    (*cmd_queue)->R = 0;
    (*cmd_queue)->G = 0;
    (*cmd_queue)->B = 255;
	(*cmd_queue)->repeat = 255;
    return 1;
}

int f_hold_yellow(pRGB * cmd_queue) {
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB));
    (*cmd_queue)->R = 255;
    (*cmd_queue)->G = 255;
    (*cmd_queue)->B = 0;
	(*cmd_queue)->repeat = 255;
    return 1;
}

int f_hold_breath(pRGB * cmd_queue) {
    int i;
    (*cmd_queue) = (pRGB)malloc(sizeof(RGB)*80);//80 = 32+8+32
    for(i = 0;i < 72;i ++) {
        (*cmd_queue+i)->R = (i<32)?(255-i*8):
                            ((i>40)?(255-31*8+(i-40)*8):
                            (255-31*8));
        (*cmd_queue+i)->G = (*cmd_queue+i)->R;
        (*cmd_queue+i)->B = (*cmd_queue+i)->R; 
        (*cmd_queue+i)->repeat = 255;
    }
    return 72;
}

int f_hold_color(pRGB * cmd_queue) {
    int i;

    (*cmd_queue) = (pRGB)malloc(sizeof(RGB)*3);
    for(i = 0;i < 3;i ++) {
        (*cmd_queue+i)->R = (i == 0)?255:0;
        (*cmd_queue+i)->G = (i == 1)?255:0;
        (*cmd_queue+i)->B = (i == 2)?255:0;
        (*cmd_queue+i)->repeat = 255;
    }
    return 3;
}
//////////////////////////////////////////////////////////////////////////
