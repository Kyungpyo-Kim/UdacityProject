#include <iomanip>
#include <string>

#include "format.h"

using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  long h = seconds / (60 * 60);
  long m = (seconds - h * 60 * 60) / 60;
  long s = seconds - h * 60 * 60 - m * 60;
  stringstream ss;
  ss << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << m
     << ":" << setw(2) << setfill('0') << s;
  return ss.str();
}