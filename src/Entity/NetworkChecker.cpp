//
// Created by zpf on 24-11-25.
//
#include "Entity/NetworkChecker.h"
#include <cpp-httplib/httplib.h>

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
        return true; // 连接成功
    }
    return false; // 连接失败
}