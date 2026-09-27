//   Developed by ViniciusHNF & Leandro Vitor
//   Custom Build for ESP32-S3 (ST7789 2.8" + Touch XPT2046 + SD + JY050)

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

// =============================================
// Barramento SPI Principal (FSPI) - Compartilhado
// =============================================
#define SPI_SCK_PIN 12
#define SPI_MOSI_PIN 11
#define SPI_MISO_PIN 13
#define SPI_SS_PIN 10

#define USE_HSPI_PORT

// =============================================
// Sistema de Entrada: Módulo JY050 (5 Vias + 2 Botões)
// =============================================
#define HAS_5_BUTTONS
#define UP_BTN 4
#define DOWN_BTN 5
#define LEFT_BTN 6
#define RIGHT_BTN 7
#define SEL_BTN 1
#define BTN_SET 2
#define BTN_RST 42

// =============================================
// Display ST7789 2.8" (240x320)
// =============================================
#define TFT_BACKLIGHT_ON HIGH
#define TFT_BL 21
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 14
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_MISO 13

#define ROTATION 1
#define MINBRIGHT 1

// =============================================
// Touchscreen XPT2046
// =============================================
#define HAS_TOUCH 1
#define TOUCH_CS 15
#define TOUCH_IRQ 16

// =============================================
// Cartão MicroSD (Integrado na Tela)
// =============================================
#define SDCARD_CS 40
#define SDCARD_SCK 12
#define SDCARD_MISO 13
#define SDCARD_MOSI 11

// =============================================
// Configurações USB e LED RGB Interno (WS2812)
// =============================================
#define USB_VID 0x303a
#define USB_PID 0x1001

#define PIN_RGB_LED 48
static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;
#define BUILTIN_LED LED_BUILTIN
#define LED_BUILTIN LED_BUILTIN
#define RGB_BUILTIN LED_BUILTIN
#define RGB_BRIGHTNESS 64

// =============================================
// Barramento I2C Padrão (OLED 0.91" 128x32 secundário)
// =============================================
static const uint8_t SDA = 18;
static const uint8_t SCL = 8;

// =============================================
// Pinos Mapeados para o Sistema
// =============================================
static const uint8_t SS = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 13;
static const uint8_t SCK = 12;

static const uint8_t TX = 43;
static const uint8_t RX = 44;

// Mapeamento Analógico e Touch
static const uint8_t A0 = 1;
static const uint8_t A1 = 2;
static const uint8_t A2 = 3;
static const uint8_t A3 = 4;
static const uint8_t A4 = 5;
static const uint8_t A5 = 6;
static const uint8_t A6 = 7;
static const uint8_t A7 = 8;
static const uint8_t A8 = 9;
static const uint8_t A9 = 10;
static const uint8_t A10 = 11;
static const uint8_t A11 = 12;
static const uint8_t A12 = 13;
static const uint8_t A13 = 14;
static const uint8_t A14 = 15;
static const uint8_t A15 = 16;
static const uint8_t A16 = 17;
static const uint8_t A17 = 18;
static const uint8_t A18 = 19;
static const uint8_t A19 = 20;

static const uint8_t T1 = 1;
static const uint8_t T2 = 2;
static const uint8_t T3 = 3;
static const uint8_t T4 = 4;
static const uint8_t T5 = 5;
static const uint8_t T6 = 6;
static const uint8_t T7 = 7;
static const uint8_t T8 = 8;
static const uint8_t T9 = 9;
static const uint8_t T10 = 10;
static const uint8_t T11 = 11;
static const uint8_t T12 = 12;
static const uint8_t T13 = 13;
static const uint8_t T14 = 14;

#endif /* Pins_Arduino_h */
