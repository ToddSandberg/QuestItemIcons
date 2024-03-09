#include "include/LookupConfigs.h"

// string[] function FetchConfigs() global native
std::vector<std::string> FetchConfigs(RE::StaticFunctionTag *) {
    std::vector<std::string> files = clib_util::distribution::get_configs(R"(Data\)", "_ITM");
    std::vector<std::string> configs;

    if (files.empty()) {
        RE::ConsoleLog::GetSingleton()->Print(
            "\nUnable to find _ITM files in the current directory. Make sure you're running this from the Skyrim Data "
            "folder!\n");
    }

    for (std::string path : files) {
        RE::ConsoleLog::GetSingleton()->Print(path.c_str());

        CSimpleIniA ini;
        ini.SetUnicode();
        ini.SetMultiKey();

        if (const auto rc = ini.LoadFile(path.c_str()); rc < 0) {
            RE::ConsoleLog::GetSingleton()->Print("Couldnt read ini");
            continue;
        }

        if (auto values = ini.GetSection(""); values && !values->empty()) {
            std::multimap<CSimpleIniA::Entry, std::pair<std::string, std::string>, CSimpleIniA::Entry::LoadOrder>
                oldFormatMap;

            auto truncatedPath = path.substr(5);  // strip "Data\\"

            for (auto &[key, entry] : *values) {
                try {
                    RE::ConsoleLog::GetSingleton()->Print(entry);
                    RE::ConsoleLog::GetSingleton()->Print(key.pItem);
                    configs.push_back(std::format("{}={}", key.pItem, entry));
                } catch (...) {
                    RE::ConsoleLog::GetSingleton()->Print("Failed to parse entry");
                }
            }
        }
    }

    return configs;
}

bool PapyrusFunctions(RE::BSScript::IVirtualMachine *vm) {
    vm->RegisterFunction("FetchConfigs", "mkyQuestIcons_SKSEFunctions", FetchConfigs);
    return true;
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    // Once all plugins and mods are loaded, then the ~ console is ready and can
    // be printed to
    SKSE::GetPapyrusInterface()->Register(PapyrusFunctions);

    return true;
}