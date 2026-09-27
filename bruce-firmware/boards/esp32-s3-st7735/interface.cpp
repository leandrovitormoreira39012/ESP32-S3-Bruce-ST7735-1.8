//   Developed by ViniciusHNF & Leandro Vitor
//   Custom Build for ESP32-S3 (ST7789 2.8" + Touch XPT2046 + SD + JY050)

#include <Arduino.h>

// Inclusões com fallback de diretórios para o Bruce
#if __has_include("core/powerSave.h")
  #include "core/powerSave.h"
#elif __has_include("../../include/core/powerSave.h")
  #include "../../include/core/powerSave.h"
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

// ======================================================
//          CONFIGURAÇÕES DE TEMPO E DEBOUNCE
// ======================================================
const unsigned long readDelay = 30;         // Intervalo de leitura
const unsigned long debounceDelay = 50;     // Tempo de debounce dos botões

// ======================================================
//          PINAGEM DO MÓDULO JY050 E DISPLAY
// ======================================================
#ifndef UP_BTN
#define UP_BTN 4
#endif
#ifndef DOWN_BTN
#define DOWN_BTN 5
#endif
#ifndef LEFT_BTN
#define LEFT_BTN 6
#endif
#ifndef RIGHT_BTN
#define RIGHT_BTN 7
#endif
#ifndef SEL_BTN
#define SEL_BTN 1
#endif
#ifndef BTN_SET
#define BTN_SET 2
#endif
#ifndef BTN_RST
#define BTN_RST 42
#endif

#ifndef TFT_BL
#define TFT_BL 21
#endif

// ======================================================
//          FLAGS DE EVENTOS
// ======================================================
volatile bool upPress_flag = false;
volatile bool downPress_flag = false;
volatile bool leftPress_flag = false;
volatile bool rightPress_flag = false;
volatile bool slPress_flag = false;
volatile bool setPress_flag = false;
volatile bool rstPress_flag = false;

// Controle de leitura
unsigned long lastReadTime = 0;

// Estado dos botões para debounce
bool lastUpState = HIGH, stableUpState = HIGH, upDebounce = 0;
bool lastDownState = HIGH, stableDownState = HIGH, downDebounce = 0;
bool lastLeftState = HIGH, stableLeftState = HIGH, leftDebounce = 0;
bool lastRightState = HIGH, stableRightState = HIGH, rightDebounce = 0;
bool lastSelState = HIGH, stableSelState = HIGH, selDebounce = 0;

// ======================================================
// SETUP GPIO
// ======================================================
void _setup_gpio() {
    // Configura os botões digitais do JY050 como PULLUP
    pinMode(UP_BTN, INPUT_PULLUP);
    pinMode(DOWN_BTN, INPUT_PULLUP);
    pinMode(LEFT_BTN, INPUT_PULLUP);
    pinMode(RIGHT_BTN, INPUT_PULLUP);
    pinMode(SEL_BTN, INPUT_PULLUP);
    pinMode(BTN_SET, INPUT_PULLUP);
    pinMode(BTN_RST, INPUT_PULLUP);

    // CORREÇÃO DO BRILHO: Força nível digital ALTO total no backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    bruceConfig.colorInverted = 0;
    bruceConfigPins.rotation = 1;
}

void _post_setup_gpio() {
    // Nada a executar pós-setup
}

// ======================================================
//          BATERIA
// ======================================================
int getBattery() { 
    return Battery_information::getBatteryPercentage(); 
}

// ======================================================
//          BRILHO DA TELA (CORRIGIDO PARA ST7789)
// ======================================================
void _setBrightness(uint8_t brightval) { 
    pinMode(TFT_BL, OUTPUT);
    // Para garantir 100% de brilho constante e sem piscar em PWM
    if (brightval > 0) {
        digitalWrite(TFT_BL, HIGH);
    } else {
        digitalWrite(TFT_BL, LOW);
    }
}

// ======================================================
//          MAPEAMENTO DE AÇÕES DOS BOTÕES
// ======================================================
void joystickMap() {
    if (menuOptionLabel == "Main Menu") {
        PrevPress = leftPress_flag;   // Esquerda
        NextPress = rightPress_flag;  // Direita
        EscPress = rstPress_flag;     // Botão RST funciona como voltar/ESC
    } else {
        PrevPress = upPress_flag;     // Cima
        NextPress = downPress_flag;   // Baixo
        EscPress = leftPress_flag;    // Esquerda vira ESC
        DownPress = rightPress_flag;  // Direita vira Avançar
        OpenQuickAccess = false;
    }
}

// ======================================================
//          INPUT HANDLER (LEITURA DIGITAL JY050)
// ======================================================
void InputHandler(void) {
    unsigned long now = millis();
    if (now - lastReadTime < readDelay) { return; }
    lastReadTime = now;

    // Leitura Digital dos Botões do JY050 (Ativos em nível LOW)
    bool currentUp = digitalRead(UP_BTN);
    bool currentDown = digitalRead(DOWN_BTN);
    bool currentLeft = digitalRead(LEFT_BTN);
    bool currentRight = digitalRead(RIGHT_BTN);
    bool currentSel = digitalRead(SEL_BTN);
    bool currentSet = digitalRead(BTN_SET);
    bool currentRst = digitalRead(BTN_RST);

    if (currentUp == LOW) { upPress_flag = true; }
    if (currentDown == LOW) { downPress_flag = true; }
    if (currentLeft == LOW) { leftPress_flag = true; }
    if (currentRight == LOW) { rightPress_flag = true; }
    if (currentSel == LOW) { slPress_flag = true; }
    if (currentSet == LOW) { setPress_flag = true; }
    if (currentRst == LOW) { rstPress_flag = true; }

    // Envio dos Eventos
    if (upPress_flag || downPress_flag || leftPress_flag || rightPress_flag || slPress_flag || setPress_flag || rstPress_flag) {
        AnyKeyPress = true;

        joystickMap();

        SelPress = slPress_flag || setPress_flag; // Clique do meio ou botão SET confirma seleção

        // Limpa as flags de evento
        upPress_flag = false;
        downPress_flag = false;
        leftPress_flag = false;
        rightPress_flag = false;
        slPress_flag = false;
        setPress_flag = false;
        rstPress_flag = false;
    }
}

// ======================================================
// POWER / REBOOT
// ======================================================
void powerOff() {}
void checkReboot() {}
