/*
 * menu.c
 *
 *  Created on: 2025��7��5��
 *      Author: daishuying
 */
#include "zf_common_headfile.h"

test_enum current_test = 0;
car_state_type car_state = CAR_EDITING;
menu_mode_type menu_mode = MENU_GUIDE;
int cursor = 0; // �������

// �˵���
Menuitem menu1[] = {
    // ��ֵ ���� min max �Ƿ�����
    {"start", 0, 1, 0, 1, 2},        // 0
    {"num", 3, 1, 1, 5, 0},          // 1
    {"DIR", 0, 1, 0, 1, 3},          // 2
    {"tar_angle", 65, 1 , 0, 120, 0}, // 3 
    {"a", 450, 30, 0, 3000, 0},     {"b", 120, 10, 0, 3000, 0},
    {"c", 400, 10, 0, 3000, 0},     {"d", 200, 2, 0, 3000, 0},
};
Menuitem menu2[] = {
    {"start", 0, 1, 0, 1, 2}, // 0
    {"fun2", 0, 0, 0, 0, 1},  // 1
    {"fun1", 0, 0, 0, 0, 1}   // 2
};
Menuitem menu3[] = {
    {"start", 0, 1, 0, 1, 2},      // 0
    {"fun4", 0, 0, 0, 0, 1},       // 10
    {"angle", 0, 1, -180, 180, 0}, // 2
    {"fun3", 0, 0, 0, 0, 1}        // 3
};

Menuitem *menu[] = {menu1, menu2, menu3};

const int item_number[] = {sizeof(menu1) / sizeof(Menuitem),
                           sizeof(menu2) / sizeof(Menuitem),
                           sizeof(menu3) / sizeof(Menuitem)};

void fun1() {}
void fun2() {}
void fun3() {}
void fun4() {}

void start2(void) {
  state = 0;
  openLoopAng = 0.0f;
}

void start3(void) {
  state = 0;
  openLoopAng = menu[2][2].value;
}

// ����ʹ��
void key_use(int key) {

  if (menu_mode) { // �༭ģʽ
    switch (key) {
    case 0:
      if (menu[current_test][cursor].is_action <= 0 ||
          menu[current_test][cursor].is_action == 3) {
        if (menu[current_test][cursor].value >= menu[current_test][cursor].max)
          menu[current_test][cursor].value = menu[current_test][cursor].min;
        else
          menu[current_test][cursor].value += menu[current_test][cursor].step;
        tft180_clear();
      }
      break;

    case 1:
      if (menu[current_test][cursor].is_action == 0 ||
          menu[current_test][cursor].is_action == 3) {
        if (menu[current_test][cursor].value <= menu[current_test][cursor].min)
          menu[current_test][cursor].value = menu[current_test][cursor].max;
        else
          menu[current_test][cursor].value -= menu[current_test][cursor].step;
        tft180_clear();
      }
      break;

    case 2: // �˳��༭
      menu_mode = MENU_GUIDE;
      tft180_clear();
      break;

    case 3:
      switch (current_test) {
      case 0:
        break;
      case 1:
        if (cursor == 0)
          fun1();
        else if (cursor == 1)
          fun2();
        break;

      case 2:
        if (cursor == 0)
          fun3();
        else if (cursor == 1)
          fun4();
        break;
      }
      break;
    }
  }

  else { // ����ģʽ
    switch (key) {
    case 0:
      if (cursor == -1)
        cursor = (item_number[current_test] - 1);
      else
        cursor--;
      tft180_clear();
      break;
    case 1:
      if (cursor == item_number[current_test])
        cursor = 0;
      else
        cursor++;
      tft180_clear();
      break;
    case 2:
      menu_mode = MENU_EDIT;
      if (menu[current_test][cursor].is_action == 2) {
        switch (current_test) {
        case 0:
          trace_round_num = menu1[1].value;
          trace_mode = menu1[2].value;
          imu.tar_angle = menu1[3].value;
          a = menu1[4].value;
          b = menu1[5].value;
          c = menu1[6].value;
          d = menu1[7].value;
          car_state = CAR_RUNNING;
          break;
        case 1:
          start2();
          break;
        case 2:
          start3();
          break;
        }
      }
      tft180_clear();
      break;
    case 3:
      if (current_test == 0)
        current_test = 1;
      else if (current_test == 1)
        current_test = 2;
      else
        current_test = 0;
      tft180_clear();
      break;
    }
  }
}

// ��Ļ��ʾ
void menu_display() {

  if (current_test == 0) {
    show_sense_level();
    for (int i = 0; i < item_number[current_test]; i++) {
      int y = 35 + i * 15;
      tft180_show_string(7, y, menu[current_test][i].name);

      if (!menu[current_test][i].is_action)
        tft180_show_int(87, y, menu[current_test][i].value, 4);

      else if (menu[current_test][i].is_action == 3) {
        if (menu[current_test][2].value == 0)
          tft180_show_string(90, y, "CK");
        else
          tft180_show_string(85, y, "ATCK");
      }

      // ���
      if (i == cursor && menu_mode == MENU_GUIDE) {
        tft180_show_char(0, y, '>');
      }
      if (i == cursor && menu_mode == MENU_EDIT) {
        tft180_show_char(78, y, '>');
      }
    }
  }

  else {
    /*tft180_show_int(0, 0, target_data.y, 3);
    tft180_show_int(0, 20, target_data.x, 3);
    tft180_show_int(0, 40, laser_data.y, 3);
    tft180_show_int(0, 60, laser_data.x, 3);
    tft180_show_int(0, 80, vErr.xErr, 3);
    tft180_show_int(0, 100, vErr.yErr, 3);*/
    for (int i = 0; i < item_number[current_test]; i++) {
      int y = 80 + i * 18;
      tft180_show_string(8, y, menu[current_test][i].name);

      if (menu[current_test][i].is_action == 0)
        tft180_show_int(90, y, menu[current_test][i].value, 3);

      else if (menu[current_test][i].is_action == 1)
        tft180_show_string(90, y, "exe");

      // ���
      if (i == cursor && menu_mode == MENU_GUIDE) {
        tft180_show_char(0, y, '>');
      }
      if (i == cursor && menu_mode == MENU_EDIT) {
        tft180_show_char(78, y, '>');
      }
    }
  }
}

uint8 key_state_get(void) {
  if (key_check(key_1))
    return 0;
  else if (key_check(key_2))
    return 1;
  else if (key_check(key_3))
    return 2;
  else if (key_check(key_4))
    return 3;
  else
    return 4;
}

// ����״̬��ȡ+����������Ϊ1
int key_check(gpio_pin_enum key) {
  if (gpio_get_level(key) == 0) {
    system_delay_ms(100);
    if (gpio_get_level(key) == 0)
      return 1;
    else
      return 0;
  } else
    return 0;
}

// ������ʼ��
void key_init_io() {
  gpio_init(key_1, GPI, 1, GPI_PULL_DOWN);
  gpio_init(key_2, GPI, 1, GPI_PULL_DOWN);
  gpio_init(key_3, GPI, 1, GPI_PULL_DOWN);
  gpio_init(key_4, GPI, 1, GPI_PULL_DOWN);
}
