#include <Arduino.h>

#if __has_include("core/powersave.h")
  #include "core/powersave.h"
#elif __has_include("../../include/core/powersave.h")
  #include "../../include/core/powersave.h"
#endif

#if __has_include("core/utils.h")
  #include "core/utils.h"
#elif __has_include("../../include/core/utils.h")
  #include "../../include/core/utils.h"
#endif

#if __has_include("modules/others/battery_information.h")
  #include "modules/others/battery_information.h"
#elif __has_include("../../src/modules/others/battery_information.h")
  #include "../../src/modules/others/battery_information.h"
#endif

// =========================================================================
// CONFIGURAÇÕES E TEMPOS
// =========================================================================
const unsigned long readDelay = 30;          
const unsigned long firstRepeatDelay = 400;  
const unsigned long repeatDelay = 200;       
const unsigned long debounceDelay = 50;      

// =========================================================================
// MAPEAMENTO DOS PINOS DO JOYSTICK
// =========================================================================
#ifndef UP_BTN
#define UP_BTN 11    
#endif

#ifndef DOWN_BTN
#define DOWN_BTN 12  
#endif

#ifndef LEFT_BTN
#define LEFT_BTN 13  
#endif

#ifndef RIGHT_BTN
#define RIGHT_BTN 10 
#endif

#ifndef SEL_BTN
#define SEL_BTN 14   
#endif

#ifndef TFT_BL
#define TFT_BL 4     
#endif

// =========================================================================
// FLAGS DE EVENTO
// =========================================================================
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

// =========================================================================
// INICIALIZAÇÃO DE GPIOS E ROTAÇÃO 180°
// =========================================================================
void _setup_gpio() {
    pinMode(UP_BTN, INPUT_PULLUP);
    pinMode(DOWN_BTN, INPUT_PULLUP);
    pinMode(LEFT_BTN, INPUT_PULLUP);
    pinMode(RIGHT_BTN, INPUT_PULLUP);
    pinMode(SEL_BTN, INPUT_PULLUP);

    // Força o pino de luz de fundo no máximo absoluto
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    bruceConfig.colorInverted = 0;
    bruceConfigPins.rotation = 3; // Rotação 180° (para SD card virado para cima)
}

void _post_setup_gpio() {}

int getBattery() {
    return Battery_Information::getBatteryPercentage();
}

void _setBrightness(uint8_t brightval) {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
}

// =========================================================================
// LEITURA DO JOYSTICK COM EIXOS INVERTIDOS (CORRESPONDENDO À TELA 180°)
// =========================================================================
JoyDirection readJoystickDirection() {
    // Invertidos para acompanhar a tela virada de ponta-cabeça:
    if (digitalRead(UP_BTN) == LOW)    return JOY_DOWN;  // Cima vira Baixo
    if (digitalRead(DOWN_BTN) == LOW)  return JOY_UP;    // Baixo vira Cima
    if (digitalRead(LEFT_BTN) == LOW)  return JOY_RIGHT; // Esquerda vira Direita
    if (digitalRead(RIGHT_BTN) == LOW) return JOY_LEFT;  // Direita vira Esquerda

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
        EscPress = downPress_flag;
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

void inputHandler(void) {
    unsigned long now = millis();
    if (now - lastReadTime < readDelay) return;
    lastReadTime = now;

    JoyDirection newDirection = readJoystickDirection();
    if (newDirection != currentDirection) {
        currentDirection = newDirection;
        lastMoveTime = now;
        firstRepeat = true;
        if (newDirection != JOY_NONE) {
            triggerDirectionEvent(newDirection);
        }
    } else if (newDirection != JOY_NONE) {
        unsigned long delayTime = firstRepeat ? firstRepeatDelay : repeatDelay;
        if (now - lastMoveTime >= delayTime) {
            triggerDirectionEvent(newDirection);
            lastMoveTime = now;
            firstRepeat = false;
        }
    }

    bool reading = digitalRead(SEL_BTN);
    if (reading != lastButtonReading) {
        lastDebounceTime = now;
    }
    if ((now - lastDebounceTime) > debounceDelay) {
        if (reading != stableButtonState) {
            stableButtonState = reading;
            if (stableButtonState == LOW) {
                slPress_flag = true;
            }
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
