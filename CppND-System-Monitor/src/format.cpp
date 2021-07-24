#include <string>
#include "format.h"
#include <iomanip>

using std::string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  std::ostringstream stream;
  int hours = seconds / 3600; //vai truncar valor
  int minutes = int((seconds % 3600) / 60);
  int seconds_ = seconds % 60;
  stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) <<  std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds_;
  return stream.str(); 
}