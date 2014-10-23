#ifndef __SCHDULE_UTIL__
#define __SCHDULE_UTIL__

namespace schedule
{

namespace util
{

uint64_t now_time();

uint64_t make_time(int year, int month, int day, int hour, int minute, int second, int milliseconds = 0);

}

}

#endif
