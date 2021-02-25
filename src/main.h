#ifndef _WAVEFORM_MAIN_H_
#define _WAVEFORM_MAIN_H_

#include <Arduino.h>
#include "Free_Fonts.h"
#include "TFT_eSPI.h"
#include "Adafruit_ZeroTimer.h"

typedef struct {
  tc_clock_prescaler prescaler;
  uint16_t compare; 
} Timer4Params;

Timer4Params configureTimerParams(long freq);
void StartSquareWaveform(long freq, uint8_t duty);
void RunningScreen();

#endif