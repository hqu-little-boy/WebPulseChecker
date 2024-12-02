//
// Created by zpf on 24-11-28.
//

#ifndef AUTOBACKUPMANAGER_H
#define AUTOBACKUPMANAGER_H
#include <deque>
#include <filesystem>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "FileSize.h"
#include "NetworkChecker.h"

// 自动备份管理类
class AutoBackupManager {
public:
    AutoBackupManager(std::filesystem::path &&source,
                      std::filesystem::path &&backup,
                      // std::string &&extension,
                      FileSize &&maxSize,
                      std::chrono::seconds &&interval);

    // 启动备份管理
    void run() const;

private:
    [[nodiscard]] std::optional<std::deque<std::filesystem::path> > getFilesByExtension(
        const std::filesystem::path &fileFolder) const;

    [[nodiscard]] static uint64_t calculateTotalSize(const std::deque<std::filesystem::path> &files);

    [[nodiscard]] bool backupFiles() const;

    [[nodiscard]] static std::optional<std::string> calculateFileHash(const std::filesystem::path &file) ;

    [[nodiscard]] static std::optional<std::time_t> calculateFileTime(const std::filesystem::path &file);
private:
    std::filesystem::path m_sourceFolder; // 源文件夹
    std::filesystem::path m_backupFolder; // 备份文件夹
    // std::string m_fileExtension; // 文件扩展名
    // std::string m_regexPattern; // 文件名正则表达式
    FileSize m_maxBackupSize; // 最大备份文件大小
    std::chrono::seconds m_checkInterval; // 检查间隔
    NetworkChecker m_networkChecker; // 网络检查器
};
#endif //AUTOBACKUPMANAGER_H
