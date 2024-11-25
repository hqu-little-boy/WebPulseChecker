//
// Created by zpf on 24-11-25.
//
#include "Entity/Timer.h"

Timer::Timer()
    : m_running(false)
{
}

void Timer::Start()
{
    m_running = true;
    m_thread  = std::jthread([this](const std::stop_token& stoken) {
        while (!stoken.stop_requested())
        {
            if (m_running)
            {
                for (const auto& callback : m_callbacks)
                {
                    callback();
                }
            }
        }
    });
}

void Timer::AddTask(int interval, std::function<void()> callback)
{
    m_callbacks.emplace_back([interval, callback]() {
        std::this_thread::sleep_for(std::chrono::seconds(interval));
        callback();
    });
}

void Timer::Stop()
{
    m_running = false;
    if (m_thread.joinable())
    {
        m_thread.request_stop();
        m_thread.join();
    }
}

void Timer::Join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}