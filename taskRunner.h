/*
    *  Flybrix Flight Controller -- Copyright 2018 Flying Selfie Inc. d/b/a Flybrix
    *
    *  http://www.flybrix.com
*/

#ifndef task_runner_h
#define task_runner_h

#include <Arduino.h>

using TaskPtr = bool (*)();

constexpr uint32_t hzToMicros(float hz) {
    return (uint32_t) (1000000.0f / hz);
}

struct StatTrack {
    void log(uint32_t value) {
        value_last = value;
        value_min = std::min(value_min, value);
        value_max = std::max(value_max, value);
        value_sum += value;
    }

    void reset() {
        value_last = 0;
        value_min = 0xFFFFFFFF;
        value_max = 0;
        value_sum = 0;
    }
    
    uint32_t value_last{0};
    uint32_t value_min{0xFFFFFFFF};
    uint32_t value_max{0};
    uint32_t value_sum{0};
};

class TaskRunner {
   public:
    TaskRunner(TaskPtr task, uint32_t desired_interval_us);
    TaskRunner(TaskPtr task, uint32_t desired_interval_us, bool enabled);

    void setDesiredInterval(uint32_t value) {
        desired_interval_us = value;
    }

    bool process();

    void reset(uint32_t reset_time_us) {
        last_update_us = reset_time_us;
    }

    void logExecution(uint32_t delay, uint32_t duration) {
        delay_track.log(delay);
        duration_track.log(duration);
        ++call_count;
    }

    void resetStats(){
        delay_track.reset();
        duration_track.reset();
        call_count = 0;
    }

    TaskPtr task;
    uint32_t desired_interval_us;
    bool enabled;
    uint32_t last_update_us;
    StatTrack delay_track{0};
    StatTrack duration_track{0};
    uint32_t call_count{0};
    uint32_t work_count{0};
};

#endif  // task_runner_h
