//#define BUBBLESCOPE_DEBUG 1
#include "DisplayController.h"

#ifdef BUBBLESCOPE_DEBUG
#define DBGprint(x...) Serial.print(x)
#define DBGprintln(x...) Serial.println(x)
#else
#define DBGprint(x...)
#define DBGprintln(x...)
#endif

uint32_t MS_OF_BUBBLE = 1000;

uint32_t last_heart = 0;
uint32_t dly = 0;
DisplayController *display;

int heartHandler(String heart_count_str) {
  DBGprintln("heart");
  last_heart = millis();
  int hearts_count = heart_count_str.toInt() %10000;
  // Optimisation of this next call is left as an exercise for the reader
  int digits[] = {(hearts_count/10) % 10, (hearts_count) % 10, (hearts_count/1000) % 10, (hearts_count/100) % 10};
  DBGprint(digits[0]);
  DBGprint(digits[1]);
  DBGprint(digits[2]);
  DBGprintln(digits[3]);
  display->set_digits(digits);
  return hearts_count;
}

int segHandler(String segsstr) {
  display->charlieplexing_segs = segsstr.toInt();
  return 0;
}

int speedHandler(String dlystr) {
  dly =  dlystr.toInt();
  return 0;
}

void setup(){
  display = new DisplayController(A0, A1, A2, false); // set true for charlieplexing
  pinMode(D2, OUTPUT);
  Spark.function("heart", heartHandler);
  Spark.function("speed", speedHandler);
  Spark.function("segs", segHandler);
  #ifdef BUBBLESCOPE_DEBUG
  Serial.begin(115200);
  while(!Serial.available()) SPARK_WLAN_Loop();
  #endif
  int digits[] = {0, 0, 0, 0};
  display->set_digits(digits);
  return;
}

void loop(){
  delay(dly);
  if(millis() - last_heart < MS_OF_BUBBLE){
    digitalWrite(D0, LOW);
  } else {
    digitalWrite(D0, HIGH);
  }
  display->draw();
  return;
}
