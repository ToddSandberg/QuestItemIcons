#pragma once

#include <string>

// This was copied from SPID FormData.h :)
// Just trying to understand it and take what I need
// Right now just making sure formID + modName works correctly
namespace LookupForms {

    std::string fetchFormattedString(const std::string& a_editorID) {
        if (!a_editorID.empty()) {
            RE::ConsoleLog::GetSingleton()->Print("Editor ID in config, not yet implemented");
            // No keyword right now
            /* if constexpr (std::is_same_v<Form, RE::BGSKeyword>) {
                auto& keywordArray = a_dataHandler->GetFormArray<RE::BGSKeyword>();

                auto result =
                    std::find_if(keywordArray.begin(), keywordArray.end(), [&](const auto& keyword) {
                        return keyword && keyword->formEditorID == a_editorID.c_str();
                    });

                if (result != keywordArray.end()) {
                    if (const auto keyword = *result; keyword) {
                        form = keyword;
                    } else {
                        buffered_logger::critical("\t[{}] {} FAIL - couldn't get existing keyword", path,
                                                    a_editorID);
                    }
                } else {
                    const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSKeyword>();
                    if (auto keyword = factory ? factory->Create() : nullptr; keyword) {
                        keyword->formEditorID = a_editorID;
                        keywordArray.push_back(keyword);

                        form = keyword;
                    } else {
                        buffered_logger::critical("\t[{}] {} FAIL - couldn't create keyword", path,
                                                    a_editorID);
                    }
                }
            } else {
                form = RE::TESForm::LookupByEditorID(a_editorID);
                if (!form) {
                    buffered_logger::error("\t[{}] {} FAIL - editorID doesn't exist", path, a_editorID);
                } else {
                    return a_form->GetFormID() & 0x00FFFFFF;
                }
            }*/
        }
        return "";
    }

    std::string fetchFormattedString(const FormModPair& a_formMod) {
        if (auto [formID, modName] = a_formMod; formID) {
            // TODO right now theoretically I just need the formID + modName
            // Eventually might want to retrieve the form itself
            /* if (g_mergeMapperInterface) {
                detail::get_merged_IDs(formID, modName);
            }
            if (modName) {
                form = a_dataHandler->LookupForm(*formID, *modName);
            } else {
                form = RE::TESForm::LookupByID(*formID);
            }
            if (!form) {
                buffered_logger::error("\t[{}] [0x{:X}] ({}) FAIL - formID doesn't exist", path, *formID,
                                       modName.value_or(""));
            } else {
                if constexpr (std::is_same_v<Form, RE::BGSKeyword>) {
                    if (string::is_empty(form->GetFormEditorID())) {
                        form = nullptr;
                        buffered_logger::error(
                            "\t[{}] [0x{:X}] ({}) FAIL - keyword does not have a valid editorID", path,
                            *formID, modName.value_or(""));
                    }
                }
            }*/
            std::string formIdAsString = std::to_string(*formID);
            return std::format("{}~{}", formIdAsString, *modName);
        }
        return "";
    }

    inline std::string fetchFormId(FormOrEditorID formOrEditorID) {

        // Apply function depending on type
        return std::visit([](auto&& arg) { return fetchFormattedString(arg); }, formOrEditorID);
    }
}