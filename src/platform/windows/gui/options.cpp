#include "gui.h"
#include <imgui.h>
#include <filesystem>
#include "platform/windows/config.h"

using namespace std::experimental::filesystem::v1;

extern bool showBiosWindow;
extern bool showControllerSetupWindow;

void biosSelectionWindow() {
    static bool biosesFound = false;
    static std::vector<std::string> bioses;
    static int selectedBios = 0;

    if (!biosesFound) {
        bioses.clear();
        auto dir = directory_iterator("data/bios");
        for (auto &e : dir) {
            if (!is_regular_file(e)) continue;

            auto path = e.path().string();
            auto ext = getExtension(path);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == "bin" || ext == "rom") {
                bioses.push_back(path);
            }
        }
        biosesFound = true;

        int i = 0;
        for (auto it = bioses.begin(); it != bioses.end(); ++it, ++i) {
            if (*it == config["bios"]) {
                selectedBios = i;
                break;
            }
        }
    }

    ImGui::Begin("BIOS", &showBiosWindow, ImGuiWindowFlags_AlwaysAutoResize);
    if (bioses.empty()) {
        ImGui::Text(
            "BIOS directory is empty.\n"
            "You need one of BIOS files (eg. SCPH1001.bin) placed in data/bios directory.\n"
            ".bin and .rom extensions are recognised.");
    } else {
        ImGui::PushItemWidth(300.f);
        ImGui::ListBox("", &selectedBios, [](void *data, int idx, const char **out_text) {
            const std::vector<std::string> *v = (std::vector<std::string> *)data;
            *out_text = v->at(idx).c_str();
            return true;
        }, (void *)&bioses, (int)bioses.size());
        ImGui::PopItemWidth();

        if (ImGui::Button("Select", ImVec2(-1, 0)) && selectedBios < bioses.size()) {
            config["bios"] = bioses[selectedBios];
            config["initialized"] = true;

            biosesFound = false;
            showBiosWindow = false;
            doHardReset = true;
        }
    }
    ImGui::End();
}

void button(std::string button) {
    static std::string currentButton = "";

    if (button == currentButton && lastPressedKey != 0) {
        config["controller"][button] = SDL_GetKeyName(lastPressedKey);
        lastPressedKey = 0;
    }

    std::string key = config["controller"][button];

    ImGui::Text(button.c_str());
    ImGui::NextColumn();
    if (ImGui::Button(key.c_str(), ImVec2(100.f, 0.f))) {
        currentButton = button;
        waitingForKeyPress = true;
        ImGui::OpenPopup("Waiting for key...");
    }
    ImGui::NextColumn();
}

void controllerSetupWindow() {
    ImGui::Begin("Controller", &showControllerSetupWindow, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginPopupModal("Waiting for key...", &waitingForKeyPress, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Press any key (ESC to cancel).");
        ImGui::EndPopup();
    }

    ImGui::Text("Controller 1 key mapping");

    ImGui::Columns(2, nullptr, false);
    button("up");
    button("down");
    button("left");
    button("right");

    button("triangle");
    button("cross");
    button("square");
    button("circle");

    button("l1");
    button("r1");
    button("l2");
    button("r2");

    button("select");
    button("start");

    ImGui::Columns(1);
    ImGui::End();
}