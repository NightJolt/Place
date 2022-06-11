#include "interface.h"

namespace {
    float selected_color[3];

    bool color_picker_enabled = false;
    void draw_color_picker() {
        ImGui::Begin("Color Picker", &color_picker_enabled);

            ImGui::ColorPicker3("color picker tool", (float*)&selected_color);

        ImGui::End();
    }

    void draw_nav_bar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("World")) {
                ImGui::MenuItem("Connect");
                ImGui::MenuItem("Disconnect");

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window")) {
                if (ImGui::MenuItem("Color Picker")) {
                    color_picker_enabled = true;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}

void space::interf::draw() {
    draw_nav_bar();

    draw_color_picker();
}

fun::rgb_t space::interf::get_selected_color() {
    return { (uint8_t)(::selected_color[0] * 255), (uint8_t)(::selected_color[1] * 255), (uint8_t)(::selected_color[2] * 255) };
}