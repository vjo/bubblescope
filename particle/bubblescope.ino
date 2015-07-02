// #define BUBBLESCOPE_DEBUG 1
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
DisplayController display(D3, D4, D5);

int heartHandler(String heart_count_str) {
  DBGprintln("heart");
  last_heart = millis();
  int hearts_count = heart_count_str.toInt();
  // Optimisation of this next call is left as an exercise for the reader
  int digits[] = {hearts_count/1000, hearts_count/100, hearts_count/10, hearts_count};
  display.set_digits(digits);
  return 0;
}

void setup(){
  pinMode(D0, OUTPUT);
  Spark.function("heart", heartHandler);
  #ifdef BUBBLESCOPE_DEBUG
  Serial.begin(115200);
  while(!Serial.available()) SPARK_WLAN_Loop();
  #endif
  return;
}

void loop(){
  if(millis() - last_heart < MS_OF_BUBBLE){
    digitalWrite(D0, LOW);
  } else {
    digitalWrite(D0, HIGH);
  }
  return;
}
