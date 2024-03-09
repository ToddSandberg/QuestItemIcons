#pragma once

#include <filesystem>
#include <variant>

// SHOULD USE CLIBUTIL INSTEAD IF IT WORKS
namespace filefetcher {
    inline std::vector<std::string> get_configs(std::string_view a_folder, std::string_view a_suffix = ""sv,
                                                std::string_view a_extension = ".ini"sv) {
        std::vector<std::string> configs{};

        for (const auto iterator = std::filesystem::directory_iterator(a_folder); const auto& entry : iterator) {
            if (entry.exists()) {
                if (const auto& path = entry.path(); !path.empty() && path.extension() == a_extension) {
                    if (const auto& fileName = entry.path().string();
                        a_suffix.empty() || fileName.rfind(a_suffix) != std::string::npos) {
                        configs.push_back(fileName);
                    }
                }
            }
        }

        std::ranges::sort(configs);

        return configs;
    }
}