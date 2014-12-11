## Overview
a scheduler base on libuv. Use c++11.

## Documentation
### API
all api in [scheduler.hpp](https://github.com/as-xjc/scheduler/blob/master/scheduler.hpp) and [util.hpp](https://github.com/as-xjc/scheduler/blob/master/util.hpp). Precision time in milliseconds.

### example

``` c++
#include <uv.h>
#include <ctime>

#include "scheduler.hpp"

int main(int argc, char* argv[])
{
        uv_loop_t* loop = new uv_loop_t;
        uv_loop_init(loop);

        schedule::scheduler s(loop);

	// every minutes 5 second to run
        s.run_every_minute(5*schedule::second, [](){
                std::time_t now = std::time(nullptr);
                struct tm* test = std::localtime(&now);
                std::printf("%s", std::asctime(test));
        });

	// run at after 5 seconds
        s.run_at(schedule::util::now_time() + 5*schedule::second, [](){
                std::time_t now = std::time(nullptr);
                struct tm* test = std::localtime(&now);
                std::printf("run at :%s", std::asctime(test));
        });

        uv_run(loop, UV_RUN_DEFAULT);

        uv_loop_close(loop);
        delete loop;

        return 0;
}
```
