//
// Created by zpf on 24-11-28.
//
#include "Entity/AutoBackupManager.h"

#include "../../cmake-build-release/3rd/cryptopp-cmake/cryptopp/sha.h"
#include "spdlog/spdlog.h"

// SHA256
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <files.h>
#include <cryptopp/filters.h>
#include <hex.h>
#include <regex>
using namespace std::string_literals;

AutoBackupManager::AutoBackupManager(std::filesystem::path&& source,
                                     std::filesystem::path&& backup,
                                     // std::string&&           extension,
                                     FileSize&&             maxSize,
                                     std::chrono::seconds&& interval)
    : m_sourceFolder{source},
      m_backupFolder{backup},
      // m_fileExtension{extension},
      m_maxBackupSize{maxSize},
      m_checkInterval{interval},
      m_networkChecker{"https://one.one.one.one"s}

{
}

void AutoBackupManager::run() const
{
    // if (this->m_networkChecker.CheckConnection())
    if (false)
    {
        std::this_thread::sleep_for(this->m_checkInterval);
    }
    else
    {
        spdlog::info("网络异常，启用备份");
        // 备份操作
        const auto startTime{std::chrono::system_clock::now()};
        const auto result{this->backupFiles()};
        const auto endTime{std::chrono::system_clock::now()};
        const auto duration{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
        if (!result)
        {
            spdlog::error("备份失败，耗时{}秒", duration.count());
        }
        else
        {
            spdlog::info("备份完成，耗时{}秒", duration.count());
        }

        if (duration < this->m_checkInterval)
        {
            std::this_thread::sleep_for(this->m_checkInterval - duration);
        }
        else
        {
            spdlog::error("备份耗时超过检查间隔");
        }
    }
}

std::optional<std::deque<std::filesystem::path> > AutoBackupManager::getFilesByExtension(
    const std::filesystem::path& fileFolder) const
{
    if (!std::filesystem::exists(fileFolder))
    {
        spdlog::error("源文件夹不存在");
        return std::nullopt;
    }
    const std::regex fileNamePattern(
        R"(^tdas-[^-]+-[^-]+-\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2}-\d{3}\.h5$)");
    std::deque<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(fileFolder))
    {
        const auto fileName{entry.path().filename().string()};
        if (std::regex_match(fileName, fileNamePattern))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path());
            }
            else
            {
                spdlog::error("{}文件不是普通文件", entry.path().string());
            }
        }

    }

    std::sort(files.begin(),
              files.end(),
              [](const std::filesystem::path& a, const std::filesystem::path& b) {
                  return AutoBackupManager::calculateFileTime(a) <
                         AutoBackupManager::calculateFileTime(b);
              });

    std::string strFiles;
    for (const auto& file : files)
    {
        strFiles += file.string() + ",";
    }

    spdlog::info("获取到以下文件[{}]", strFiles);
    return files;
}

uint64_t AutoBackupManager::calculateTotalSize(
    const std::deque<std::filesystem::path>& files)
{
    auto totalSize{0ull};

    for (const auto& file : files)
    {
        if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file))
        {
            spdlog::error("{}文件不存在或不是普通文件", file.string());
            continue;
        }
        totalSize += std::filesystem::file_size(file);
    }
    return totalSize;
}

bool AutoBackupManager::backupFiles() const
{
    if (!std::filesystem::exists(this->m_backupFolder))
    {
        spdlog::error("备份文件夹不存在");
        return false;
    }

    auto sourceFiles{this->getFilesByExtension(this->m_sourceFolder)};
    auto backedFiles{this->getFilesByExtension(this->m_backupFolder)};
    if (sourceFiles->empty())
    {
        spdlog::error("获取文件列表失败");
        return false;
    }
    auto       backedFilesTotalSize{AutoBackupManager::calculateTotalSize(backedFiles.value())};
    const auto needBackupFile{sourceFiles.value().front()};
    int        iCount = 0;
    while (FileSize(backedFilesTotalSize + std::filesystem::file_size(needBackupFile)) >
           this->m_maxBackupSize)
    {
        // 删除最早的文件，并重新计算大小，直到小于最大限制
        iCount++;
        std::filesystem::remove(backedFiles.value().front());
        backedFiles.value().pop_front();
        backedFilesTotalSize = AutoBackupManager::calculateTotalSize(backedFiles.value());

    }

    if (iCount > 0)
    {
        spdlog::info("超过最大备份大小，删除{}个文件", iCount);
    }

    // 开始备份
    auto backupFile{this->m_backupFolder / needBackupFile.filename()};
    spdlog::info("备份文件{}到{}", needBackupFile.string(), backupFile.string());

    if (std::filesystem::exists(backupFile))
    {
        spdlog::error("{}文件存在，备份文件失败", backupFile.string());
        return false;
    }

    try
    {
        std::filesystem::copy_file(needBackupFile,
                                   backupFile);
    }
    catch (const std::exception& e)
    {
        spdlog::error("备份文件失败:{}", e.what());
        return false;
    }

    // 校验备份文件
    auto sourceFileHash{AutoBackupManager::calculateFileHash(needBackupFile)};
    auto backupFileHash{AutoBackupManager::calculateFileHash(backupFile)};
    if (!sourceFileHash.has_value() || !backupFileHash.has_value())
    {
        spdlog::error("计算文件哈希值失败");
        return false;
    }

    if (sourceFileHash.value() != backupFileHash.value())
    {
        spdlog::error("文件哈希值不一致");
        return false;
    }
    return true;
}

std::optional<std::string> AutoBackupManager::calculateFileHash(
    const std::filesystem::path& file)
{
    if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file))
    {
        spdlog::error("{}文件不存在或不是普通文件", file.string());
        return std::nullopt;
    }

    // 创建 MD5 哈希对象
    CryptoPP::SHA256 hash;
    // 用于存储哈希值的变量
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    // 将文件内容传递给哈希对象
    CryptoPP::FileSource fs(file.c_str(),
                            true,
                            new CryptoPP::HashFilter(hash,
                                                     new CryptoPP::ArraySink(digest, sizeof(digest))
                                                     // 将结果存入 digest
                                ) // HashFilter

        ); // FileSource
    // 将二进制的哈希值转换为十六进制字符串
    std::string          strHash;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(strHash));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    return strHash;
}

std::optional<std::time_t> AutoBackupManager::calculateFileTime(const std::filesystem::path& file)
{
    if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file))
    {
        spdlog::error("{}文件不存在或不是普通文件", file.string());
        return std::nullopt;
    }
    const auto fileName{file.filename().string()};
    // 从文件名中提取时间
    std::tm          tm{};
    const std::regex timePattern(R"(\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2})");
    std::smatch      match;
    if (!std::regex_search(fileName, match, timePattern))
    {
        spdlog::error("{}文件名中没有找到时间", file.string());
        return std::nullopt;
    }
    std::istringstream ss(match.str());
    ss >> std::get_time(&tm, "%Y-%m-%d-%H-%M-%S");
    if (ss.fail())
    {
        spdlog::error("解析时间失败");
        return std::nullopt;
    }
    return std::mktime(&tm);
}