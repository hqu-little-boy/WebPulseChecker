//
// Created by zpf on 24-12-2.
//
#include "Entity/FileSize.h"

#include <format>

uint64_t FileSize::bytes() const
{
    return bytes_;
}

std::string FileSize::ToString() const
{
    std::string result;
    // 余数
    auto remainder{bytes_ % 1024};
    // 除数
    auto divisor{bytes_ / 1024};
    while (divisor > 0)
    {
        result    = std::format("{}::{}", remainder, result);
        remainder = divisor % 1024;
        divisor /= 1024;
    }
    return std::format("{}::{}", remainder, result);
}

std::strong_ordering FileSize::operator<=>(const FileSize& rhs) const
{
    return bytes_ <=> rhs.bytes_;
}