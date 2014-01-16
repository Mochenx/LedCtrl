#include "LedCtrl.h"

void new_patn(ppattern new_patn,init_handler hdl) {
    new_patn->patn_init_hdl = hdl;
    new_patn->cmd_q_size = (*new_patn->patn_init_hdl)(&new_patn->cmd_queue);//Initiallizing pattern queue
}

void new_led(pLED new_led,
            int pin_r,int pin_g,int pin_b,
            ppattern use_patn) {
    int i;

    new_led->pins[0] = pin_r;
    new_led->pins[1] = pin_g;
    new_led->pins[2] = pin_b;
    new_led->_step_idx = 0;
    new_led->_repeat_idx = 0;
    new_led->patn = use_patn;
    
}

pRGB step(pLED led_obj) {
    pRGB pret_val;
    int i;

    pret_val = (led_obj->patn->cmd_queue + led_obj->_step_idx);
    
    //if(led_obj->_repeat_idx == 0) {
    //}
    if(led_obj->_repeat_idx >= pret_val->repeat) {
        led_obj->_repeat_idx = 0;
        led_obj->_step_idx ++;
    }
    if(led_obj->_step_idx >= led_obj->patn->cmd_q_size)
        led_obj->_step_idx = 0;
    return pret_val;
}
    
