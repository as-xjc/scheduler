#include "scheduler.hpp"
#include "util.hpp"

namespace schedule
{

namespace 
{

void schedule_callback(uv_timer_t* handle)
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
	if (repeat > 0) {
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

schedule_id scheduler::run_at(uint64_t target_time, schedule_cb cb)
{
        uint64_t now_time = schedule::util::now_time();

	if (target_time < now_time) return 0;
	return _schedule(target_time - now_time, 0, cb);
}

schedule_id scheduler::run_every_minute(uint64_t timeout, schedule_cb cb)
{
	timeout = timeout % schedule::minute;
	uint64_t now_second = schedule::util::now_time() % schedule::minute;
	uint64_t needTimeout = 0;
	if (timeout <= now_second) {
		needTimeout = schedule::minute - now_second + timeout;
	} else {
		needTimeout = timeout - now_second;
	}

	return _schedule(needTimeout, minute, cb);
}

schedule_id scheduler::run_every_hour(uint64_t timeout, schedule_cb cb)
{
	timeout = timeout % schedule::hour;
	uint64_t now_second = schedule::util::now_time() % schedule::hour;
	uint64_t needTimeout = 0;
	if (timeout <= now_second) {
		needTimeout = schedule::hour - now_second + timeout;
	} else {
		needTimeout = timeout - now_second;
	}

	return _schedule(needTimeout, minute, cb);
}

schedule_id scheduler::heartbeat(uint64_t repeat, schedule_cb cb, bool immediately)
{
	if (repeat < 1) return 0;

	uint64_t timeout = 0;
	if (!immediately) timeout = repeat;

	return _schedule(timeout, repeat, cb);
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
