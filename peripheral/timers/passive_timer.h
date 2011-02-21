#ifndef PASSIVE_TIMER_H
#define PASSIVE_TIMER_H

// this class not sends signals,
// it's just give possibiliy to know when time interval was ended
class PassiveTimer
{
    bool   started_;
    time_t initial_time_;
    time_t interval_;
public:
    PassiveTimer();
    void start();
    void stop()  {  started_ = false;  }

    time_t getInterval()  {  return interval_;  }
    void   setInterval(const time_t sec);

    bool   isActive();
};

// example of usage:
//    timer = new c_timer;
//    timer->setInterval(descreteness);
//    timer->start();
//    while (timer->isActive()) printf("Interval yet hasn't expired");
//    printf("Interval has been expired!");

#endif // PASSIVE_TIMER_H
