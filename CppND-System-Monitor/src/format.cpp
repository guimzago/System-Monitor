#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hours = seconds / 3600; //vai truncar valor
  int minutes = int((seconds % 3600) / 60);
  int seconds_ = seconds % 60;
  string clock_format = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds_);
  return clock_format; 
}