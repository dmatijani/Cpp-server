#include "time.h"
#include <chrono>
#include <string>

void Time::get_time(char out[30])
{
    time_t sad = time(0);
    tm *ltm = localtime(&sad);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", ltm);
    std::string time_str = std::string(buffer);
    for (int i = 0; i < 30; i++) {
        out[i] = time_str[i];
    }
}