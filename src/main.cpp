#include <iostream>
#include <Entity/NetworkChecker.h>
#include <Entity/Timer.h>

using namespace std::string_literals;

int main()
{
    Timer          timer;
    NetworkChecker checker{"http://www.baidu.com"};

    timer.AddTask(5,
                  [&checker]() {
                      if (checker.CheckConnection())
                      {
                          std::println(std::cout, "网络正常");
                      }
                      else
                      {
                          std::println(std::cout, "网络异常");
                      }
                  });
    timer.Start();
    timer.Join();

    return 0;
}