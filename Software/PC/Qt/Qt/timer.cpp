#include "timer.h"
#include <QDebug>

Timer::Timer()
{
    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
            this, SLOT(TimerSlot()));

    // msec
    timer->start(1000);
}

void Timer::TimerSlot()
{
    qDebug() << "Timer...";
}
