//   Modificado: rotacao 180° + joystick invertido + brilho corrigido
#include "../src/modules/others/battery_information.h"
#include "core/powerSave.h"
#include "core/utils.h"
#include <Arduino.h>

const int center = 2048;
const int deadzone = 400;
const unsigned long readDelay = 20;
const unsigned long firstRepeatDelay = 400;
const unsigned long repeatDelay = 200;
const unsigned long debounceDelay = 50;

#ifndef SEL_BTN
#define SEL_BTN 14
#endif
#ifndef JOY_X
#define JOY_X 12
#endif
#ifndef JOY_Y
#define JOY_Y 13
#endif
#ifndef TFT_BL
#define TFT_BL 4
#endif

volatile bool upPress_flag = false;
volatile bool downPress_flag = false;
volatile bool leftPress_flag = false;
volatile bool rightPress_flag = false;
volatile bool slPress_flag = false;

enum JoyDirection { JOY_NONE, JOY_LEFT, JOY_RIGHT, JOY_UP, JOY_DOWN };
JoyDirection currentDirection = JOY_NONE;
unsigned long lastReadTime = 0;
unsigned long lastMoveTime = 0;
bool firstRepeat = true;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void _setup_gpio() {
  pinMode(SEL_BTN, INPUT_PULLUP);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  bruceConfig.colorInverted = 0;
  bruceConfigPins.rotation = 3;   // <-- 180° mantendo paisagem (voltar para 1 se quiser inverter)
}

void _post_setup_gpio() {}

int getBattery() { return Battery_information::getBatteryPercentage(); }

// BRILHO CORRIGIDO: mapeia 0-100% -> 8-255 (antes mandava 100 direto = tela escura)
void _setBrightness(uint8_t brightval) {
  uint32_t duty = brightval;
  if (brightval <= 100) {
    duty = map((long)brightval, 0L, 100L, (long)MINBRIGHT, 255L);
  }
  analogWrite(TFT_BL, duty);
}

int smoothAnalogRead(uint8_t pin) {
  long total = 0;
  for (int i = 0; i < 4; i++) { total += analogRead(pin); }
  return total / 4;
}

// EIXOS INVERTIDOS (compensa a tela rotacionada 180°)
JoyDirection readJoystickDirection() {
  int x = smoothAnalogRead(JOY_X);
  int y = smoothAnalogRead(JOY_Y);
  if (x < center - deadzone) { return JOY_LEFT; }   // antes era JOY_RIGHT
  if (x > center + deadzone) { return JOY_RIGHT; }  // antes era JOY_LEFT
  if (y < center - deadzone) { return JOY_UP; }     // antes era JOY_DOWN
  if (y > center + deadzone) { return JOY_DOWN; }   // antes era JOY_UP
  return JOY_NONE;
}

void triggerDirectionEvent(JoyDirection dir) {
  switch (dir) {
    case JOY_UP:    upPress_flag = true; break;
    case JOY_DOWN:  downPress_flag = true; break;
    case JOY_LEFT:  leftPress_flag = true; break;
    case JOY_RIGHT: rightPress_flag = true; break;
    default: break;
  }
}

void joystickMap() {
  if (menuOptionLabel == "Main Menu") {
    PrevPress = leftPress_flag;
    NextPress = rightPress_flag;
  } else {
    PrevPress = upPress_flag;
    NextPress = downPress_flag;
    EscPress = leftPress_flag;
    DownPress = rightPress_flag;
    OpenQuickAccess = false;
  }
}

void InputHandler(void) {
  unsigned long now = millis();
  if (now - lastReadTime < readDelay) { return; }
  lastReadTime = now;

  JoyDirection newDirection = readJoystickDirection();
  if (newDirection != currentDirection) {
    currentDirection = newDirection;
    lastMoveTime = now;
    firstRepeat = true;
    if (newDirection != JOY_NONE) { triggerDirectionEvent(newDirection); }
  } else if (newDirection != JOY_NONE) {
    unsigned long delayTime = firstRepeat ? firstRepeatDelay : repeatDelay;
    if (now - lastMoveTime >= delayTime) {
      triggerDirectionEvent(newDirection);
      lastMoveTime = now;
      firstRepeat = false;
    }
  }

  bool reading = digitalRead(SEL_BTN);
  if (reading != lastButtonReading) { lastDebounceTime = now; }
  if ((now - lastDebounceTime) > debounceDelay) {
    if (reading != stableButtonState) {
      stableButtonState = reading;
      if (stableButtonState == LOW) { slPress_flag = true; }
    }
  }
  lastButtonReading = reading;

  if (upPress_flag || downPress_flag || leftPress_flag || rightPress_flag || slPress_flag) {
    AnyKeyPress = true;
    joystickMap();
    SelPress = slPress_flag;
    upPress_flag = false;
    downPress_flag = false;
    leftPress_flag = false;
    rightPress_flag = false;
    slPress_flag = false;
  }
}

void powerOff() {}
void checkReboot() {}
