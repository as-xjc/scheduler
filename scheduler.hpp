// The MIT License (MIT)
//
// Copyright (c) 2014 xiao jian cheng
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once

#include <map>
#include <memory>
#include <uv.h>
#include "define.hpp"
#include "util.hpp"

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
	scheduler(uv_loop_t* loop);
	~scheduler();

	void clear();
	void unschedule(schedule_id id);

	schedule_id run_at(uint64_t target_time, schedule_cb cb);
	schedule_id run_every_minute(uint64_t timeout, schedule_cb cb);
	schedule_id run_every_hour(uint64_t timeout, schedule_cb cb);
	schedule_id run_every_day(uint64_t timeout, schedule_cb cb);
	schedule_id heartbeat(uint64_t repeat, schedule_cb cb, bool immediately = false);

private:
	schedule_id _schedule(uint64_t timeout, uint64_t repeat, schedule_cb& cb);
	const uv_loop_t* _loop;
	schedule_id _id = 1;
	std::map<schedule_id, std::shared_ptr<schedule_t>> _schedules;
};

}
