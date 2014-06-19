#ifndef __SCHDULE_SCHDULER__
#define __SCHDULE_SCHDULER__

#include <map>
#include <functional>
#include <memory>
#include "uv.h"

namespace schedule
{

const static uint64_t one_second = 1000;
const static uint64_t one_minute = one_second*60;
const static uint64_t one_hour = one_minute*60;
const static uint64_t one_day = one_hour*24;

typedef uint64_t schedule_id;
typedef std::function<void()> schedule_cb;

class scheduler;

enum class schedule_type {repeat, run_one};
typedef struct 
{
	schedule_id id;
	schedule_type type;
	uv_timer_t timer;
	schedule_cb callback;
	scheduler* base;
}schedule_t;

class scheduler 
{
public:
	scheduler();
	~scheduler();
	void init(uv_loop_t* loop);

	void clear();
	void unschedule(schedule_id id);

	schedule_id run_at(uint64_t target_time, schedule_cb cb, bool use_millisecond = false);
	schedule_id run_every_minute(uint64_t timeout, schedule_cb cb, bool use_millisecond = false);
	schedule_id heartbeat(uint64_t repeat, schedule_cb cb, bool start_now = false, bool use_millisecond  = false);

private:
	schedule_id _schedule(uint64_t timeout, uint64_t repeat, schedule_cb& cb);
	uv_loop_t* _loop;
	schedule_id _id;
	std::map<schedule_id, std::shared_ptr<schedule_t>> _schedules;
};

}

#endif
