#pragma once

#include <iostream>
#include <chrono>

class Timer
{
public:
    // 在构造时记录当前时间点
    Timer() : start(std::chrono::high_resolution_clock::now()) {}

    // 返回自计时器构造以来经过的时间（毫秒）
    double elapsed() const
    {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    // 重新记录当前时间点
    void reset()
    {
        start = std::chrono::high_resolution_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};