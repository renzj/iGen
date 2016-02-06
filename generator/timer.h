#ifndef PORCUPINE_TIMER_H
#define PORCUPINE_TIMER_H

#include <ctime>
#include <stdint.h>

namespace porcupine
{

typedef int64_t TimeInSec;
typedef int64_t TimeInMsec;
typedef int64_t TimeInUsec;
typedef int64_t TimeInNsec;
          
const TimeInUsec USEC_PER_SEC = 1000000;
const TimeInNsec NSEC_PER_USEC = 1000;
const TimeInNsec NSEC_PER_MSEC = 1000000;
const TimeInNsec NSEC_PER_SEC = USEC_PER_SEC * NSEC_PER_USEC;

inline TimeInNsec GetCurrentTimeInNanoSeconds()
{
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_sec * NSEC_PER_SEC + tm.tv_nsec;
}

inline TimeInSec GetCurrentTimeInSeconds()
{
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_sec;
}

inline TimeInUsec GetCurrentTimeInMicroSeconds()
{
    return GetCurrentTimeInNanoSeconds() / NSEC_PER_USEC;
}

inline TimeInMsec GetCurrentTimeInMilliSeconds()
{
    return GetCurrentTimeInNanoSeconds() / NSEC_PER_MSEC;
}

} // namespace porcupine
#endif 
