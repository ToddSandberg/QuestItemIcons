#include "include/LookupConfigs.h"
#include "LookupForms.hpp"

// string[] function FetchConfigs() global native
std::vector<std::string> FetchConfigs(RE::StaticFunctionTag *) {
    // Fetches all files ending in _ITM from Data/
    std::vector<std::string> files = clib_util::distribution::get_configs(R"(Data\)", "_ITM");
    // Store strings to be returned on SKSE call
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
            auto truncatedPath = path.substr(5);  // strip "Data\\"

            for (auto &[key, entry] : *values) {
                try {
                    RE::ConsoleLog::GetSingleton()->Print(entry);
                    RE::ConsoleLog::GetSingleton()->Print(key.pItem);

                    // TODO see FormData.h in SPID for example of handling FormOrEditorID
                    // TODO move this to a function
                    // TODO test this works
                    // TODO sanitize string
                    // Right now only filters is quest
                    const auto sections = clib_util::string::split(entry, "|");
                    FormOrEditorID record = clib_util::distribution::get_record(sections[0]);
                    std::string recordString = LookupForms::fetchFormId(record);
                    std::string convertedLine = ""; 
                    if (sections.size() > 1) {
                        FormOrEditorID quest = clib_util::distribution::get_record(sections[1]);
                        std::string questString = LookupForms::fetchFormId(record);
                        convertedLine = std::format("{}|{}", recordString, questString);
                    } else {
                        convertedLine = recordString;
                    }

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