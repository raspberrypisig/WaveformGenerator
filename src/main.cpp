#include <Arduino.h>

#include "Adafruit_ZeroTimer.h"

Adafruit_ZeroTimer zt4 = Adafruit_ZeroTimer(4);

typedef struct {
  tc_clock_prescaler prescaler;
  uint16_t compare; 
} Timer4Params;

Timer4Params configureTimerParams(long freq);

void setup() {
  Serial.begin(115200);

  delay(10000);
  pinMode(D1, OUTPUT);
  zt4.enable(false);

  long freq = 30000;

  Timer4Params params = configureTimerParams(freq);

  zt4.configure(params.prescaler, // prescaler
                TC_COUNTER_SIZE_16BIT,   // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM // frequency or PWM mode ( TC_WAVE_GENERATION_NORMAL_PWM, TC_WAVE_GENERATION_MATCH_PWM)
                );

                
  if (! zt4.PWMout(true, 1, D1)) {
    Serial.println("Failed to configure PWM output");
  }

  else {
    Serial.println("PWM output configured");
  }

  uint32_t duty = params.compare * 50 / 100;

  Serial.printf("Compare:%d\n", params.compare);
  Serial.printf("Match:%d\n", duty);
  // 10kHz 50% duty cycle square wave
  //zt4.setCompare(1, 4799);

  zt4.setPeriodMatch(params.compare, duty, 1);
  zt4.enable(true);
}

void loop() {

}



Timer4Params configureTimerParams(long freq) {
  uint16_t divider  = 1;
  uint16_t compare = 0;
  tc_clock_prescaler prescaler = TC_CLOCK_PRESCALER_DIV1;

  if ((freq < 24000000) && (freq > 800)) {
    divider = 1;
    prescaler = TC_CLOCK_PRESCALER_DIV1;
    compare = 48000000/freq;
  } else if (freq > 400) {
    divider = 2;
    prescaler = TC_CLOCK_PRESCALER_DIV2;
    compare = (48000000/2)/freq;
  } else if (freq > 200) {
    divider = 4;
    prescaler = TC_CLOCK_PRESCALER_DIV4;
    compare = (48000000/4)/freq;
  } else if (freq > 100) {
    divider = 8;
    prescaler = TC_CLOCK_PRESCALER_DIV8;
    compare = (48000000/8)/freq;
  } else if (freq > 50) {
    divider = 16;
    prescaler = TC_CLOCK_PRESCALER_DIV16;
    compare = (48000000/16)/freq;
  } else if (freq > 12) {
    divider = 64;
    prescaler = TC_CLOCK_PRESCALER_DIV64;
    compare = (48000000/64)/freq;
  } else if (freq > 3) {
    divider = 256;
    prescaler = TC_CLOCK_PRESCALER_DIV256;
    compare = (48000000/256)/freq;
  } else if (freq >= 0.75) {
    divider = 1024;
    prescaler = TC_CLOCK_PRESCALER_DIV1024;
    compare = (48000000/1024)/freq;
  } else {
    Serial.println("Invalid frequency");
    while (1) delay(10);
  }
  Serial.print("Divider:"); Serial.println(divider);
  Serial.print("Compare:"); Serial.println(compare);
  Serial.print("Final freq:"); Serial.println((int)(48000000/compare));

  Timer4Params params;
  params.compare = (compare>0) ? compare - 1: 0;
  params.prescaler = prescaler;

  return params;
}