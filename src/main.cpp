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
                          std::cout << "网络正常" << std::endl;
                      }
                      else
                      {
                          std::cout << "网络异常" << std::endl;
                      }
                  });
    timer.Start();
    timer.Join();

    return 0;
}