#ifndef __PWM_H__
#define __PWM_H__

#define PWM_CHANNEL_MAX 5

struct pwm_single_param {
    uint16 gpio_set;
    uint16 gpio_clear;
    uint32 h_time;
};

struct pwm_param {
    uint32 period;
    uint16 freq;
    uint8  duty[PWM_CHANNEL_MAX];
};

#define PWM_DEPTH 255

#define PWM_1S 1000000

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_0_OUT_IO_NUM 15
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO15

#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_1_OUT_IO_NUM 14
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO14

#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_2_OUT_IO_NUM 13
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO13

#define PERI_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PERI_0_OUT_IO_NUM 12
#define PERI_0_OUT_IO_FUNC  FUNC_GPIO12


void pwm_init(uint16 freq, uint8 *duty, uint8 io_num, uint8 *io_id);
void pwm_start(void);

void pwm_set_duty(uint8 duty, uint8 channel);
uint8 pwm_get_duty(uint8 channel);
void pwm_set_freq(uint16 freq);
uint16 pwm_get_freq(void);
#endif


