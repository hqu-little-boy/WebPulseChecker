//
// Created by zpf on 24-11-25.
//
#include "Entity/NetworkChecker.h"
#include "httplib.h"
#include "spdlog/spdlog.h"

NetworkChecker::NetworkChecker(std::string_view url)
    : m_url(url)
{
}

bool NetworkChecker::CheckConnection() const
{
    httplib::Client cli(m_url);
    auto            res = cli.Get("/");

    if (res && res->status == 200)
    {
        spdlog::info("网络正常");
        return true; // 连接成功
    }
    spdlog::error("网络异常，需要启用备份");
    return false; // 连接失败
}