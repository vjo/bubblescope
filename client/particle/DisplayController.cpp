#include "DisplayController.h"
#include "application.h"

// #define BUBBLESCOPE_DEBUG 1

#ifdef BUBBLESCOPE_DEBUG
#define DBGprint(x...) Serial.print(x)
#define DBGprintln(x...) Serial.println(x)
#else
#define DBGprint(x...)
#define DBGprintln(x...)
#endif

DisplayController::DisplayController(int data_in, int data_clock, int store_clock, bool charlieplexing){
  pinMode(data_in, OUTPUT);
  pinMode(data_clock, OUTPUT);
  pinMode(store_clock, OUTPUT);
  data_in_ = data_in;
  data_clock_ = data_clock;
  store_clock_ = store_clock;
  charlieplexing_ = charlieplexing;
}

/* TODO, when charlieplexing is again a thing, this function will light the next set of segments */
void DisplayController::draw(){
  if(charlieplexing_){
    draw_n_segs(first_seg_, charlieplexing_segs);
    first_seg_ = (first_seg_ + 1) % 8;
  }
}

void DisplayController::set_digits(int *digits){
  memcpy(digits_, digits, sizeof(digits_));
  // refresh the entire segment display if we do not charlieplex
  if(!charlieplexing_){
    full_draw();
  }
}

// PRIVATE FUNCTIONS
void DisplayController::tick(int clock){
  digitalWrite(clock, HIGH);
  digitalWrite(clock, LOW);
}

// Draws nb_seg segments, starting with segment first_seg
void DisplayController::draw_n_segs(int first_seg = 0,int nb_seg = 8) {
  for(int i= 0; i < DISPLAY_SIZE; ++i) {
      uint8_t mask = 0;
    for(int j = 0; j < nb_seg; j++) {
      mask += 1 << (((j*3) + first_seg) % 8);
    }
      uint8_t cmd = kNumberDefinition_[digits_[i]] & mask;
      shift_command(cmd);
    }
  tick(store_clock_);
}

// Draws all segments of the display
void DisplayController::full_draw() {
  draw_n_segs();
}


void DisplayController::shift_command(uint8_t cmd) {
  for(int i = 7; i >= 0;--i){
    int state = ((1 << i) & cmd)?HIGH:LOW;
    digitalWrite(data_in_, state);
    tick(data_clock_);
  }
}

void DisplayController::shift_segment(int segment){
  for(int i=7; i>=0; i--) {
    int segment_value = (i == segment)? HIGH:LOW;
    digitalWrite(data_in_, segment_value);
    tick(data_clock_);
  }
}
