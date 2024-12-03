//
// Created by zpf on 24-12-2.
//

#ifndef FILESIZE_H
#define FILESIZE_H
#include <cstdint>
#include <string>

class FileSize {
public:
    constexpr explicit FileSize(uint64_t bytes = 0) noexcept;

    [[nodiscard]] uint64_t bytes() const;

    [[nodiscard]] std::string ToString() const;

    [[nodiscard]] std::strong_ordering operator<=>(const FileSize &rhs) const;

private:
    uint64_t bytes_;
};

constexpr FileSize::FileSize(uint64_t bytes) noexcept
    : bytes_(bytes) {
}

// 用户定义字面量，用于表示不同单位的文件大小
constexpr FileSize operator"" _B(const unsigned long long bytes) {
    return FileSize(bytes);
}

constexpr FileSize operator"" _KB(const unsigned long long kilobytes) {
    return FileSize(kilobytes * 1024ull);
}

constexpr FileSize operator"" _MB(const unsigned long long megabytes) {
    return FileSize(megabytes * 1024ull * 1024ull);
}

constexpr FileSize operator"" _GB(const unsigned long long gigabytes) {
    return FileSize(gigabytes * 1024ull * 1024ull * 1024ull);
}

constexpr FileSize operator"" _TB(const unsigned long long terabytes) {
    return FileSize(terabytes * 1024ull * 1024ull * 1024ull * 1024ull);
}
#endif //FILESIZE_H
