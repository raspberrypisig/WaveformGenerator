#include "main.h"

Adafruit_ZeroTimer zt4 = Adafruit_ZeroTimer(4);
TFT_eSPI tft = TFT_eSPI();
long freq = 300L;
uint8_t duty = 50;



void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);

  delay(5000);
  
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

  pinMode(D1, OUTPUT);


  RunningScreen();


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
  //Serial.print("Final freq:"); Serial.println((int)(48000000/compare));

  Timer4Params params;
  params.compare = (compare>0) ? compare - 1: 0;
  params.prescaler = prescaler;

  return params;
}

void RunningScreen() {
        tft.setTextDatum(MC_DATUM);
        tft.fillScreen(TFT_RED);
        tft.setFreeFont(FF17);
        tft.setTextColor(tft.color565(224,225,232));
        tft.drawString("Function Generator", 160, 20);
        tft.fillRoundRect(10, 45, 300, 55, 5, tft.color565(255, 194, 179));
        tft.fillRoundRect(10, 105, 300, 55, 5, tft.color565(255, 194, 179));
        //if (homescreen_waveform == Waveform::SQUARE) {
          tft.fillRoundRect(10, 165, 300, 55, 5, tft.color565(255, 194, 179));
        //}
        
        
        tft.setFreeFont(FM9);
        tft.setTextColor(TFT_BLACK);
        tft.drawString("Waveform", 160, 50);
        tft.drawString("Frequency(Hz)", 160, 110);
        //if (homescreen_waveform == Waveform::SQUARE) {
        tft.drawString("Duty Cycle(%)", 160, 170);
        //}


        tft.setFreeFont(FMB12);
        tft.setTextColor(TFT_BLUE);
        //tft.drawString(waveform_names[(int)homescreen_waveform], 160, 72);
        //tft.setTextColor(tft.color565(224,225,232));
        tft.setCursor(35, 140);
        //displayMacAddress(mac);

        //long freq = 0L;

        //if (homescreen_waveform == Waveform::SQUARE) {
        //  for (int i=0; i<8; i++) {
        //   freq += powl(10, i) * squarewave_frequency[i] ;
        //  }
        //}

        //else {
        //   freq = 1000L;
        //}
         
        tft.drawNumber(freq, 160, 132);
        //if (homescreen_waveform == Waveform::SQUARE) {
          //tft.drawNumber(squarewave_dutycycle, 160, 192); 
          tft.drawNumber(duty, 160, 192);
          //Serial.println(freq);
          //Serial.println(squarewave_dutycycle);
          Serial.println("Enter running screen: square wave");
          StartSquareWaveform(freq, duty);
        //}

        //if (homescreen_waveform == Waveform::RAMP) {
        //  Serial.println("Start ramp  waveform");
        //  StartRampWaveform();
        //}
}

void StartSquareWaveform(long freq, uint8_t duty) {
  zt4.enable(false);
  Timer4Params params = configureTimerParams(freq);
  uint32_t compare = (uint32_t) params.compare;
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

  uint32_t match = compare * duty / 100;

  Serial.printf("Compare:%d\n", compare);
  Serial.printf("Match:%d\n", match);
  // 10kHz 50% duty cycle square wave
  //zt4.setCompare(1, 4799);

  zt4.setPeriodMatch(compare, match, 1);
  zt4.enable(true);
}
