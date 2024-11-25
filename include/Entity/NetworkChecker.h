//
// Created by zpf on 24-11-25.
//

#ifndef NETWORKCHECKER_H
#define NETWORKCHECKER_H
#include <string_view>

#include <string>

class NetworkChecker {
public:
    explicit NetworkChecker(std::string_view url);

    [[nodiscard]] bool CheckConnection() const;

private:
    std::string m_url;
};

#endif //NETWORKCHECKER_H
