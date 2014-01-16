#ifndef LEDCTRL
#define LEDCTRL
typedef struct __RGB {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char repeat;
}RGB,* pRGB;

typedef int (*init_handler)(pRGB *);
typedef struct __pattern {
    int cmd_q_size;
    pRGB cmd_queue;
    init_handler patn_init_hdl;
}pattern, * ppattern;

typedef struct __LED {
    int pins[3];
    int _step_idx;
    int _repeat_idx;
    ppattern patn;
}LED,* pLED;

void new_patn(ppattern ,init_handler );
void new_led(pLED ,pLED*,
            int ,int ,int ,
            ppattern );
pRGB step(pLED );
void reset_led(pLED );

#endif
