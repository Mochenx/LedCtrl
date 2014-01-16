#include <LedCtrl.h>

#define LED_NUM 1
#define DIP_OPT_NUM 16
#define PATN_NUM 5

#define LED1_R_PIN 16
#define LED1_G_PIN 15
#define LED1_B_PIN 14

#define DIP_PIN_3 0
#define DIP_PIN_2 0
#define DIP_PIN_1 0
#define DIP_PIN_0 0

//Interrupt pin
#define STOP_PIN 2

//State Varibles
int running = 1;
unsigned char state = 0;

//Global Declarations for controlling LEDs
//Pointer Arrays
pLED p_all_leds[LED_NUM];
ppattern p_all_patn_f[LED_NUM][DIP_OPT_NUM];
//Instance Arrays
pattern all_patns[PATN_NUM];
LED led1;

void setup()   {                
    int i;
	int pins[LED_NUM*3] = {LED1_R_PIN,LED1_G_PIN,LED1_B_PIN};

	init_patns();

    //Initialize LED1 object
	for(i = 0;i< LED_NUM;i ++)
    	new_led(&led1,(p_all_leds+i),
    	 	pins[i*3],pins[i*3+1],pins[i*3+2],
    	    p_all_patn_f[i][state]);

    for(i = 0;i< 3;i ++)
        pinMode(led1.pins[i],OUTPUT);

	//Input Pins
    //pinMode(STOP_PIN, INPUT);
    //pinMode(DIP_PIN_3, INPUT);
    //pinMode(DIP_PIN_2, INPUT);
    //pinMode(DIP_PIN_1, INPUT);
    //pinMode(DIP_PIN_0, INPUT);

    //attachInterrupt(STOP_PIN,stop,CHANGE);
}

void loop()                     
{
    int i,j;
    pRGB wr_rgb;
	unsigned char new_state;//!!!!

    for(i = 0;i < LED_NUM;i ++) {
		new_state = get_dip();//!!!!  
		if((state != new_state) && (p_all_patn_f[j][new_state] != 0)){
			for(j = 0;j<LED_NUM;j ++) {
				p_all_leds[j]->patn = p_all_patn_f[j][new_state];
				reset_led(p_all_leds[j]);
				state = new_state;
			}
		}
        if(running) {
            wr_rgb = step(p_all_leds[i]);
            analogWrite(p_all_leds[i]->pins[0], wr_rgb->R);
            analogWrite(p_all_leds[i]->pins[1], wr_rgb->G);
            analogWrite(p_all_leds[i]->pins[2], wr_rgb->B);
        }
		else {//Turn off LEDs
    		for(j = 0;i < 3;j ++)
            	analogWrite(p_all_leds[i]->pins[j], 0);
		}
    }
    delay(100);
}

//void stop() {
//    running = !running;
//}


void init_patns() {
	int i,j;

	for(i = 0;i<LED_NUM;i ++)
		for(j = 0;j<LED_NUM;j ++)
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

	//System State mapping to pattern
	p_all_patn_f[0][0] = &all_patns[0];
	p_all_patn_f[0][1] = &all_patns[1];
	p_all_patn_f[0][2] = &all_patns[2];
	p_all_patn_f[0][3] = &all_patns[3];
	p_all_patn_f[0][4] = &all_patns[4];
}

unsigned char get_dip() {
	int i;
	char dip_pins[4] = {DIP_PIN_3, DIP_PIN_2, DIP_PIN_1, DIP_PIN_0};
	unsigned char dip_val = 0;

	for(i = 0;i< 4;i ++) {
		dip_val |= (digitalRead(dip_pins[i]) & 0x1);
		dip_val = (dip_val << 1)&0xFE;
	}
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
    (*cmd_queue)->R = 0;
    (*cmd_queue)->G = 255;
    (*cmd_queue)->B = 255;
	(*cmd_queue)->repeat = 255;
    return 1;
}
//////////////////////////////////////////////////////////////////////////
