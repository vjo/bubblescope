// #define BUBBLESCOPE_DEBUG 1

#ifdef BUBBLESCOPE_DEBUG
#define DBGprint(x...) Serial.print(x)
#define DBGprintln(x...) Serial.println(x)
#else
#define DBGprint(x...)
#define DBGprintln(x...)
#endif

uint32_t MS_OF_BUBBLE = 1000;

uint32_t last_heart = 0;

int heartHandler(String not_used) {
  DBGprintln("heart");
  last_heart = millis();
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
