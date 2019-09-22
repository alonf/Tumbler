/**********************************
*                                 *
*    Tumbler Motor Controller     *
*                                 *
*        By Alon Fliess           *
*                                 *
*            2019                 *
*                                 *
/*********************************/

#include <Arduino.h>
#include <TM1637Display.h>
#include "TumblerController.h"

TM1637Display display(CLK, DIO);
unsigned int speed = 3;
unsigned int time = 24 * 60; //in minutes
bool isRunning = false;
unsigned long lastAccountingTime;
unsigned long pressTime;
unsigned long buttonPressStartTime;
bool buttonPreviousState = false;
unsigned long lastActionTime = 0;
unsigned char currentAction = 0;

Action actions[] = {Action(SEG_START, &Start), 
                    Action(SEG_SPD, &SpeedUp), Action(SEG_SPD_DOWN, &SpeedDown),
                    Action(SEG_DUR, &TimeUp), Action(SEG_DUR_DOWN, &TimeDown),
                    Action(SEG_RESET, &Reset)
};

void DisplayRunningLoop(int times, int stepDelay)
{
  int length = sizeof(PERIMETER)/sizeof(PERIMETER[0]);
  for (int t = 0; t < times; ++t) //times
  {
    for (int loop = 0; loop < length; ++loop)
    {
      uint8_t LEDS[] = {0,0,0,0};
      for (int digit = 0; digit < length/2; ++digit)
      {
        int shiftedDigit = (digit + loop)%length;
        int index = PERIMETER[shiftedDigit] / 10;
        int led = 1 << (PERIMETER[shiftedDigit] % 10);
        LEDS[index] |= led;
      }
      display.setSegments(LEDS);
      delay(stepDelay);
    }
  }
}

void Start()
{
  isRunning = true;
  analogWrite(SPD, 75 + (speed - 1) * 30);
  delay(250);
  digitalWrite(RELAY, LOW);
  lastAccountingTime = millis();
}

void Stop()
{
  digitalWrite(RELAY, HIGH);
  digitalWrite(SPD, 0);
  isRunning = false;
  currentAction = 0; 
  DisplayRunningLoop(2, 4);
  display.clear();
  display.setSegments(SEG_STOP);
  delay(400);
  actions[currentAction].Display();
  lastActionTime = millis();  
}

void DisplaySpeed(bool shouldErase = true)
{
  if (shouldErase)
    display.clear();
  display.showNumberDec(speed, false); 
}

void SpeedUp()
{
  ++speed;
  if (speed > 5)
    speed = 5;
  DisplaySpeed();
}

void SpeedDown()
{
  --speed;
  if (speed < 1)
    speed = 1;
  DisplaySpeed();
}

void DisplayTime(bool shouldErase = true, uint8_t colon = 0b01000000)
{
  auto hours = time / 60;
  auto minutes = time % 60;
  if (shouldErase)
    display.clear();
  display.showNumberDecEx(minutes + hours * 100, colon, true);
}
void TimeUp()
{
  time += 30;
  DisplayTime();
}

void TimeDown()
{
  if (time == 0)
  return;
  
  if (time < 30)
  {
    --time;
    DisplayTime();
    return;
  }
  
  time -= 30;
  DisplayTime();
}

void Reset()
{
  speed = 3;
  isRunning = false;
  time = 24 * 60;
  DisplayRunningLoop(4, 2);
  currentAction = 0; 
  actions[currentAction].Display();
  lastActionTime = millis();
}

void Action::Display()
{
  display.clear();
  display.setSegments(_display);
}

void Action::Act()
{
  _func();
}


ButtonAction DetectButtonAction()
{
  bool current = digitalRead(BUTTON) == 0; //pull up
  if (!current && buttonPreviousState && (millis() - pressTime) > LongPressTime)
  {
    buttonPreviousState = false;
    Serial.println("ButtonActionLongPress");
    return ButtonActionLongPress;
  }
  //else
  if (!current && buttonPreviousState && (millis() - pressTime) > minimumButtonPressingTime)
  {
    buttonPreviousState = false;
    Serial.println("ButtonActionShortPress");
    return ButtonActionShortPress;
  }

  if (current && !buttonPreviousState)
  {
    pressTime = millis();
  }
  buttonPreviousState = current;
  return ButtonActionNone;
}

void setup()
{
  pinMode(SPD, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP); 
  digitalWrite(RELAY, HIGH);
  pinMode(RELAY, OUTPUT); 
  
 //blink 5 times
  display.clear();
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  for (int i = 0; i < 5; ++i)
  {
    display.setBrightness(0x0f - 4 + i);
    display.setSegments(data);
    delay(25);
    display.setSegments(blank);
    delay(25);
  }
  display.clear();
  DisplayRunningLoop(4, 7);
  display.setSegments(SEG_START);
  Serial.begin(9600);
}

auto clockColon = 0b01000000;
byte lastSecondDigit = 0;
void HandleRunning(ButtonAction buttonAction)
{
  if (buttonAction != ButtonActionNone)
  {
    Stop();
    return;
  }
  bool shouldErase = false;
  if (millis() - lastAccountingTime >= 60000) //a minute
  {
    lastAccountingTime = millis();
    time -= 1;
    shouldErase = true;
  }

  byte secondDigit = (millis() / 1000)%10;
  if (secondDigit != lastSecondDigit) //a second has passed
  {
    lastSecondDigit = secondDigit;
    clockColon = (clockColon == 0b01000000) ? 0 : 0b01000000;
  }
  DisplayTime(shouldErase, clockColon);
  if (time == 0)
      Stop();
}

void ShowStatus()
{
    if ((millis() / 1000) % 2 == 1)
    {
      DisplayTime(false);
    }
    else
    {
      DisplaySpeed(false);
    }
}

void loop()
{
  if (millis() - lastActionTime > 5 * 60 * 1000)
    display.setBrightness(0);
    
  auto buttonAction = DetectButtonAction();
  if (isRunning)
  {
    HandleRunning(buttonAction);
    return;
  }
  //else
  display.setBrightness(0xf);
  //not running, handle menue
  if (buttonAction == ButtonActionLongPress)
  {
    currentAction = (currentAction + 1) % (sizeof(actions)/sizeof(actions[0]));
    actions[currentAction].Display();
    lastActionTime = millis();
    return;
  }
  //else
  if (buttonAction == ButtonActionShortPress)
  {
    actions[currentAction].Act();
    lastActionTime = millis();
  }

  if (millis() - lastActionTime > 10 * 1000)
  {
     ShowStatus();
  }
}
