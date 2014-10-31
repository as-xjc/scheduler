#ifndef __SCHDULE_SCHDULER__
#define __SCHDULE_SCHDULER__

#include <map>
#include <memory>
#include <uv.h>
#include "define.hpp"

namespace schedule
{

class scheduler;

enum class schedule_type {repeat, run_one};
typedef struct 
{
	schedule_id id = 0;
	schedule_type type;
	uv_timer_t timer;
	schedule_cb callback;
	scheduler* base = nullptr;
}schedule_t;

class scheduler 
{
public:
	scheduler();
	~scheduler();
	void init(uv_loop_t* loop);

	void clear();
	void unschedule(schedule_id id);

	schedule_id run_at(uint64_t target_time, schedule_cb cb);
	schedule_id run_every_minute(uint64_t timeout, schedule_cb cb);
	schedule_id run_every_hour(uint64_t timeout, schedule_cb cb);
	schedule_id run_every_day(uint64_t timeout, schedule_cb cb);
	schedule_id heartbeat(uint64_t repeat, schedule_cb cb, bool immediately = false);

private:
	schedule_id _schedule(uint64_t timeout, uint64_t repeat, schedule_cb& cb);
	uv_loop_t* _loop = nullptr;
	schedule_id _id = 1;
	std::map<schedule_id, std::shared_ptr<schedule_t>> _schedules;
};

}

#endif
