#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "passive_timer.h"

PassiveTimer::PassiveTimer():
        started_      (false),
        initial_time_ (0),
        interval_     (0)
{
}

//---------------------------------------------------------------------------------------//
void PassiveTimer::setInterval(const time_t sec)
{
    if (started_) { fprintf(stderr, "\nERROR:Uncorrect usage of class PassiveTimer. Timer is started_. You should stop it before new start.\n"); exit(0); }
    interval_ = sec;
}

//---------------------------------------------------------------------------------------//
void PassiveTimer::start()
{
    if (started_) { fprintf(stderr, "\nERROR:Uncorrect usage of class PassiveTimer. Timer is started_. You should stop it before new start.\n"); exit(0); }
    started_ = true;
    initial_time_ = time(0);
}

//---------------------------------------------------------------------------------------//
bool PassiveTimer::isActive()
{
    bool active = true;
//    if ( !interval_ ) { fprintf(stderr, "\nERROR: Uncorrect usage of class PassiveTimer. Interval wasn't set\n"); exit(0); }
//    if ( !started_ ) { fprintf(stderr, "\nERROR:Uncorrect usage of class PassiveTimer. Timer wasn't started_\n"); exit(0); }
    if (started_){
        if ( interval_ <= difftime(time(0), initial_time_) ){
            active = false;
            started_ = false;
        }
        else active = true;
    }
    else active = false;

    return active;
}
