#include <chrono>
#include <ctime>
#include "util.hpp"
#include "define.hpp"

namespace schedule
{

namespace util
{

uint64_t now_time()
{
        std::chrono::system_clock::duration time_since = std::chrono::system_clock::now().time_since_epoch();
        return  std::chrono::duration_cast<std::chrono::milliseconds>(time_since).count();
}

uint64_t make_time(int year, int month, int day, int hour, int minute, int second, int milliseconds)
{
	std::tm tm;
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;

	std::time_t t = std::mktime(&tm);
	if (t == -1) return 0;

	uint64_t result = t;
	result = result*schedule::second + milliseconds;
	return result;
}

}

}
