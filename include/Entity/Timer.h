//
// Created by zpf on 24-11-25.
//

#ifndef TIMER_H
#define TIMER_H
#include <thread>
#include <functional>
#include <list>

class Timer {
public:
    Timer();

    void Start();

    void AddTask(int interval, std::function<void()> callback);

    void Stop();

    void Join();

private:
    std::list<std::function<void()> > m_callbacks;
    bool m_running;
    std::jthread m_thread;
};

#endif //TIMER_H
