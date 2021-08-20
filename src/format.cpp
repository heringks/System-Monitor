#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string hrs_str = "00";
    string mins_str = "00";
    string secs_str = "00";

    long hrs = seconds / 3600;
    long mins = (seconds % 3600) / 60;
    long secs = seconds % 60;
    if (hrs < 10) {
        hrs_str = "0" + to_string(hrs);
    }
    else hrs_str = to_string(hrs);

    if (mins < 10) {
        mins_str = "0" + to_string(mins);
    }
    else mins_str = to_string(mins);

    if (secs < 10) {
        secs_str = "0" + to_string(secs);
    }
    else secs_str = to_string(secs);
    
    return string(hrs_str + ":" + mins_str + ":" + secs_str);
}