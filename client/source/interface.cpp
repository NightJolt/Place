#include "interface.h"

namespace {
    bool color_picker_window_enabled = false;
    float selected_color[3] = { 1.f, 1.f, 1.f };
    void draw_color_picker(space::state_t& state) {
        ImGui::Begin("Color Picker", &color_picker_window_enabled);

            if (ImGui::ColorEdit3("color picker tool", (float*)&selected_color)) {
                state.brush.color = { (uint8_t)(::selected_color[0] * 255), (uint8_t)(::selected_color[1] * 255), (uint8_t)(::selected_color[2] * 255) };
            }

        ImGui::End();
    }

    bool connect_to_server_window_enabled = false;
    char ip_input[32];
    char port_input[8];
    void draw_connect_to_server(space::state_t& state) {
        ImGui::Begin("Connect", &connect_to_server_window_enabled);

            ImGui::InputText(" Ip Address", ip_input, sizeof ip_input);
            ImGui::InputText(" Port", port_input, sizeof port_input);
            ImGui::NewLine();

            if (ImGui::Button("Join")) {
                if (state.client.connect(ip_input, std::stoi(port_input))) {
                    connect_to_server_window_enabled = false;
                    
                    state.canvas.clear();
                }
            }

        ImGui::End();
    }

    void draw_nav_bar(space::state_t& state) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("World")) {
                if (ImGui::MenuItem("Connect")) {
                    connect_to_server_window_enabled = true;
                }
                
                if (state.client.check_connection() && ImGui::MenuItem("Disconnect")) {
                    state.client.disconnect();

                    state.canvas.clear();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window")) {
                if (ImGui::MenuItem("Color Picker")) {
                    color_picker_window_enabled = true;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}

void space::interf::draw(state_t& state) {
    ::draw_nav_bar(state);

    if (::color_picker_window_enabled) ::draw_color_picker(state);
    if (::connect_to_server_window_enabled) ::draw_connect_to_server(state);
}