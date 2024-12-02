#include "spdlog/spdlog.h"

#include <iostream>
#include <Entity/AutoBackupManager.h>
#include <Entity/NetworkChecker.h>
#include <Entity/Timer.h>

using namespace std::string_literals;

int main()
{
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] [pid %P] %v");
    Timer timer;

    AutoBackupManager backupManager{"/home/zpf/Share/wjj-origin-2024-07-02-15-08",
                                    "/run/media/zpf/1a85a04d-8f24-422a-9774-735d3163f5fb",
                                    // ".h5",
                                    10_GB,
                                    std::chrono::seconds(100)};
    timer.AddTask(0,
                  [&backupManager]() {
                      backupManager.run();
                  });
    timer.Start();
    timer.Join();

    return 0;
}