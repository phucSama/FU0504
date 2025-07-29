#ifndef farc_h
#define farc_h

#include <Arduino.h>
#include "Log.h"
#include "Config.h"
#include "ViaBoard.h"

//---------------------------------------------
#define ONBOARD_LED_PIN 13 // Mạch VIA
#define LED_INIT() pinMode(ONBOARD_LED_PIN, OUTPUT)
#define LED_ON()  digitalWrite(ONBOARD_LED_PIN, HIGH)
#define LED_OFF() digitalWrite(ONBOARD_LED_PIN, LOW)

//---------------------------------------------
// Các biến toàn cục
ViaBoard viaBoard = ViaBoard();
uint8_t lastMoveState;

#endif
