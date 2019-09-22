/**********************************
*                                 *
*    Tumbler Motor Controller     *
*                                 *
*        By Alon Fliess           *
*                                 *
*            2019                 *
*                                 *
/*********************************/

#ifndef _TUMBLER_CONTROLLER_H
#define _TUMBLER_CONTROLLER_H
// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3
#define SPD 6
#define BUTTON 5
#define RELAY 16

// The amount of time (in milliseconds) that a button action is conssidered
const unsigned int minimumButtonPressingTime = 75;
const unsigned int LongPressTime = 200;

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };

  const uint8_t SEG_START[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_B | SEG_G | SEG_C ,                          // t
  SEG_E | SEG_G,                                   // r
  SEG_B | SEG_G | SEG_C                            // t
  };

  const uint8_t SEG_STOP[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_B | SEG_G | SEG_C ,                          // t
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_A | SEG_F | SEG_G | SEG_B | SEG_E            // p
  };

  const uint8_t SEG_SPD[] = {
  0,                                               // _
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_A | SEG_F | SEG_G | SEG_B | SEG_E,           // p
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G            // d
  };

  const uint8_t SEG_SPD_DOWN[] = {
  SEG_G,                                           // -
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_A | SEG_F | SEG_G | SEG_B | SEG_E,           // p
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G            // d
  };

  const uint8_t SEG_DUR[] = {
  0,                                               // _
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // U
  SEG_E | SEG_G                                    // r
  };

  const uint8_t SEG_DUR_DOWN[] = {
  SEG_G,                                           // -
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // U
  SEG_E | SEG_G                                    // r
  };

  const uint8_t SEG_RESET[] = {
  SEG_E | SEG_G,                                   // r
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_B | SEG_G | SEG_C                            // t
  };

const uint8_t PERIMETER[] = {4, 5, 0, 10, 20, 30, 31, 32, 33, 23, 13, 3};

typedef void (*func_t)();
class Action
{
  private:
  const uint8_t* _display;
  func_t _func;
  
  public:
  Action(const uint8_t* display, func_t func)
    : _display(display), _func(func) {}
  void Display();
  void Act();
};

enum ButtonAction {ButtonActionNone, ButtonActionShortPress, ButtonActionLongPress};

ButtonAction DetectButtonAction();

#endif //_TUMBLER_CONTROLLER_H
