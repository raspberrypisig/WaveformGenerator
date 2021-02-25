#ifndef _WAVEFORM_MAIN_H_
#define _WAVEFORM_MAIN_H_

#include <Arduino.h>
#include "Free_Fonts.h"
#include "TFT_eSPI.h"
#include "Adafruit_ZeroTimer.h"

#define CONFIGURE_FREQUENCY_WIDTH 30
#define CONFIGURE_FREQUENCY_SPACING 25
#define NUMBEROFPOINTS 100

enum class Waveform {SQUARE=0, SINE=1, RAMP=2, TRIANGLE=3};
enum class FunctionGeneratorProgramState {HOMESCREEN, CONFIGURE_FREQUENCY, CONFIGURE_DUTY, RUNNING, RUNFOREVER};


typedef struct {
  tc_clock_prescaler prescaler;
  uint16_t compare; 
} Timer4Params;

Timer4Params configureTimerParams(long freq);
void StartSquareWaveform(long freq, uint8_t duty);
void HomeScreen_drawSquare(bool fill);
void HomeScreen_drawSine(bool fill);
void HomeScreen_drawRamp(bool fill);
void HomeScreen_drawTriangle(bool fill);

void HomeScreen_draw();
Waveform HomeScreen_nextWaveform();
void HomeScreen_redraw(Waveform current, Waveform next);
void ConfigureFrequencyScreen();
void FrequencyScreen_redraw();
void ConfigureDutyScreen();
void DutyScreen_redraw();
void RunningScreen();

#endif