#include <ctime>
#include "scheduler.hpp"

namespace schedule
{

namespace 
{

void schedule_callback(uv_timer_t* handle, int status)
{
	schedule_t* tmp = reinterpret_cast<schedule_t*>(handle->data);
	tmp->callback();

	if (tmp->type == schedule_type::run_one) {
		tmp->base->unschedule(tmp->id);
	}
}

}

scheduler::scheduler()
{
	_id = 1;
}

scheduler::~scheduler()
{
	clear();
}

void scheduler::init(uv_loop_t* loop)
{
	if (loop != nullptr) {
		_loop = loop;
	}
}

void scheduler::clear()
{
	for (auto& it : _schedules) {
		auto& tmp = it.second;
		uv_timer_stop(&(tmp->timer));
	}

	_schedules.clear();
}

schedule_id scheduler::_schedule(uint64_t timeout, uint64_t repeat, schedule_cb& cb)
{
	auto tmp = std::make_shared<schedule_t>();
	tmp->callback = cb;
	tmp->base = this;
	if (repeat) {
		tmp->type = schedule_type::repeat;
	} else {
		tmp->type = schedule_type::run_one;
	}
	
	uv_timer_init(_loop, &tmp->timer);

	schedule_id newid = 0;
	if (uv_timer_start(&tmp->timer, schedule_callback, timeout, repeat) == 0) {
		tmp->timer.data = reinterpret_cast<void*>(tmp.get());
		newid = _id++;
		tmp->id = newid;
		_schedules[newid] = tmp;
	} 

	return newid;
}

schedule_id scheduler::run_at(uint64_t target_time, schedule_cb cb, bool use_millisecond)
{
	std::time_t now = std::time(nullptr);
	uint64_t timeout = 0;
	if (use_millisecond) {
		uint64_t _now = now*one_second;
		if (target_time <= _now) return 0;

		timeout = target_time - _now;
	} else {
		if (target_time <= static_cast<uint64_t>(now)) return 0;
		timeout = (target_time-now)*one_second;
	}

	return _schedule(timeout, 0, cb);
}

schedule_id scheduler::run_every_minute(uint64_t timeout, schedule_cb cb, bool use_millisecond)
{
	std::time_t now = std::time(nullptr);
	struct tm now_mt = *std::localtime(&now);
	uint64_t now_second = now_mt.tm_sec;

	uint64_t needTimeout = 0;
	if (use_millisecond) {
		now_second *= one_second;
		if (now_second <= timeout) {
			needTimeout = timeout - now_second;
		} else {
			needTimeout = one_minute - now_second - timeout;
		}
	} else {
		if (now_second <= timeout) {
			needTimeout = (timeout-now_second)*one_second;
		} else {
			needTimeout = one_minute - (now_second-timeout)*one_second;
		}
	}

	return _schedule(needTimeout, one_minute, cb);
}

schedule_id scheduler::heartbeat(uint64_t repeat, schedule_cb cb, bool start_now, bool use_millisecond)
{
	if (repeat <= 0) return 0;

	uint64_t heartbeat = 0;
	if (use_millisecond) {
		heartbeat = repeat;
	} else {
		heartbeat = repeat*one_second;
	}

	uint64_t timeout = 0;
	if (!start_now) timeout = heartbeat;

	return _schedule(timeout, heartbeat, cb);
}

void scheduler::unschedule(schedule_id id)
{
	auto it = _schedules.find(id);
	if (it == _schedules.end()) return;

	auto& tmp = it->second;
	uv_timer_stop(&(tmp->timer));
	_schedules.erase(it);
}

}
