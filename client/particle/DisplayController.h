#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H
#define DISPLAY_SIZE 4
#include <stdint.h>


class DisplayController{
  public:
    // Instanciate a displayController for num_displays
    DisplayController(int data_in, int data_clock, int store_clock, bool charlieplexing = false);

    // Draws one segment, This function shall be called in the execution loop.
    void draw();

    // Set the digits to be displayed
    void set_digits(int digits[]);

    // If charlieplexing is activated, sets the amount of simultaneous segments
    // to display
    int charlieplexing_segs = 2;

  private:
    int digits_[DISPLAY_SIZE];
    int store_clock_;
    int data_clock_;
    int data_in_;
    int current_segment_ = 0;
    bool charlieplexing_;
    int first_seg_ = 0;

    /* Definitions of numbers as displayed by a seven segment display
       The index of the segment in the following table is as follow:

            7
         -------
        |       |             The number in position 0 corresponds to
      2 |       | 6           the comma dot, present on some 7 segment
        |   1   |             display. As we never use the comma dot,
         -------              kNumberDefinition[i][0] always equals 0
        |       |
      3 |       | 5           A special digit is kNumberDefinition[10]
        |   4   |             It correspond to no number displayed.
         -------
    */
    const uint8_t kNumberDefinition_[11] = {
      0b00111111, //0
      0b00000110, //1
      0b01011011, //2
      0b01001111, //3
      0b01100110, //4
      0b01101101, //5
      0b01111101, //6
      0b00000111, //7
      0b01111111, //8
      0b01101111, //9
      0b00000000  //10 = no light.
    };
    void shift_segment(int segment);
    void tick(int clock);
    void full_draw();
    void draw_n_segs(int nb_seg, int first_seg);
    void shift_command(uint8_t cmd);
};

#endif
