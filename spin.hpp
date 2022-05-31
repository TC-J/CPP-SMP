#if !defined(__SPIN_HPP__)
#define __SPIN_HPP__
#include <unistd.h>
#include <functional>

/* 
    exponential-backoff-spin adapted from Stackoverflow User : Eitan T
*/

static useconds_t spin_max_delay = 100000;

bool spin_while(std::function <bool ()> f) 
{
    useconds_t delay = 1;
    useconds_t max = spin_max_delay;
    while (! f())
    {
        usleep(delay);
        max -= delay;
        if (max >= 0) {
            delay *= 2;
        } else {
            return false;
        }
    }

    return true;
}

#endif // __SPIN_HPP__
