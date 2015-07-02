#include "DisplayController.h"
#include "application.h"

DisplayController::DisplayController(int data_in, int data_clock, int store_clock, bool charlieplexing){
  pinMode(data_in_, OUTPUT);
  pinMode(data_clock, OUTPUT);
  pinMode(store_clock, OUTPUT);
  data_in_ = data_in;
  data_clock_ = data_clock;
  store_clock_ = store_clock;
  charlieplexing_ = charlieplexing;
}

/* TODO, when charlieplexing is again a thing, this function will light the next set of segments */
void DisplayController::draw(){
}

void DisplayController::set_digits(int *digits){
  memcpy(digits_, digits, sizeof(digits_));
  // refresh the entire segment display if we do not charlieplex
  if(!charlieplexing_){
    full_draw();
  }
}

void DisplayController::set_simultaneous_segments(int segments){
  simultaneous_segments_ = segments;
}


// PRIVATE FUNCTIONS
void DisplayController::tick(int clock){
  digitalWrite(clock, HIGH);
  digitalWrite(clock, LOW);
}

void DisplayController::full_draw() {
  for(int i= 0; i< DISPLAY_SIZE; ++i) {
    uint8_t cmd = kNumberDefinition_[digits_[i]];
    shift_command(cmd);
  }
  tick(store_clock_);
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
