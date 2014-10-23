#ifndef __SCHDULE_DEFINE__
#define __SCHDULE_DEFINE__

#include <functional>

namespace schedule
{

const static uint64_t second = 1000;
const static uint64_t minute = second*60;
const static uint64_t hour = minute*60;
const static uint64_t day = hour*24;

typedef uint64_t schedule_id;
typedef std::function<void()> schedule_cb;

}

#endif
