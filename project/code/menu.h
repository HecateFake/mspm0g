/*
 * menu.h
 *
 *  Created on: 2025��7��5��
 *      Author: daishuying
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_
#include "zf_common_typedef.h"
#include "zf_driver_gpio.h"
#include "zf_common_headfile.h"


typedef enum {
	TEST_1,
	TEST_2,
	TEST_3,
}test_enum;

//����ö��
typedef enum {
    key1,
    key2,
    key3,
    key4,
}key_type ;

//��ǰС��״̬
typedef enum  {
    CAR_EDITING = 0,    //����ģʽ
    CAR_RUNNING,        //�н�ģʽ
}car_state_type;

//��ǰ��Ļģʽ
typedef enum{
    MENU_GUIDE = 0,     //����ģʽ
    MENU_EDIT,          //�༭ģʽ
}menu_mode_type;

// �˵���ṹ��
typedef struct {
    char name[16];      //����
    float value;        //��ֵ
    float step;   //����
		float min;
		float max;
    int is_action;     //�Ƿ�Ϊ������
} Menuitem;

extern car_state_type car_state;


int key_check(gpio_pin_enum key);//��������
void key_init_io(void);
uint8 key_state_get(void);
void key_use(int key);
void menu_display(void);

extern int cursor;
extern test_enum current_test;

extern Menuitem menu1[];
extern Menuitem menu2[];
extern Menuitem menu3[];
extern Menuitem* menu[];      
extern const int item_number[];  

#endif /* CODE_MENU_H_ */


