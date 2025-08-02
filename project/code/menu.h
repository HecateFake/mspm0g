/*
 * menu.h
 *
 *  Created on: 2025年7月5日
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

//按键枚举
typedef enum {
    key1,
    key2,
    key3,
    key4,
}key_type ;

//当前小车状态
typedef enum  {
    CAR_EDITING = 0,    //调试模式
    CAR_RUNNING,        //行进模式
}car_state_type;

//当前屏幕模式
typedef enum{
    MENU_GUIDE = 0,     //导航模式
    MENU_EDIT,          //编辑模式
}menu_mode_type;

// 菜单项结构体
typedef struct {
    char name[16];      //内容
    float value;        //数值
    float step;   //步长
		float min;
		float max;
    int is_action;     //是否为功能项
} Menuitem;

extern car_state_type car_state;


int key_check(gpio_pin_enum key);//按键消抖
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


