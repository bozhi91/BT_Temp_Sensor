

#include <Arduino.h>
#include <stdio.h>
#include "bt.h"

  static int bt_status = 0;

  int bt_init(void){

    return 0;
  }
  
  int getBtStatus(void){
    
    bt_status=!bt_status;
    return bt_status;
  }
