#pragma once

struct Message {
  enum Type {
    kInterruptXHCI,
    kTimerTimeout,
    kKeyPush,
    kCellfinish,
    kCellEfinish,
    kInterruptE1000,
    kNetInput,
  } type;

  union {
    struct {
      unsigned long timeout;
      int value;
    } timer;

    struct {
      uint8_t modifier;
      uint8_t keycode;
      char ascii;
    } keyboard;
  } arg;
};
