
#ifndef MOTOR_STRUCT
#define MOTOR_STRUCT

struct motor {
  
  byte speed = 0;

  struct {
    byte input1 = LOW;
    byte input2 = LOW;
  } direction;
};

#endif