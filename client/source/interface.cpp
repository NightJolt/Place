#include "interface.h"

namespace {
    bool console_window_enabled = false;
    char console_input[512];
    void draw_console_window(space::state_t& state) {
        ImGui::Begin("Console", &console_window_enabled);
            ImGui::BeginGroup();
                ImGui::BeginChild("RoomView");

                    const float text_area_height = ImGui::GetFrameHeightWithSpacing();
                    const float text_area_reserve = ImGui::GetStyle().ItemSpacing.y + text_area_height;

                    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -text_area_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

                        // for (int i = 0; i < chat_history.size(); i++) {
                        //     const char* item = chat_history[i].c_str();
                        //     //ImVec4 color = color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                        //     //ImGui::PushStyleColor(ImGuiCol_Text, color);
                        //     ImGui::TextUnformatted(item);
                        //     //ImGui::PopStyleColor();
                        // }

                        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                            ImGui::SetScrollHereY(1.0f);

                        ImGui::PopStyleVar();
                    ImGui::EndChild();

                    bool reclaim_focus = false;
                    ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine;// | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
                    ImGui::PushItemWidth(-1);
                    if (ImGui::InputText("##Input", console_input, sizeof console_input, input_flags)) {
                        std::string input = console_input;

                        if (!input.empty()) {
                            memset(console_input, 0, sizeof *console_input);

                            // if (input[0] == '/')
                            //     space::slave::send_message(state, input);
                            // else
                            //     space::slave::process_command(state, input);
                        }

                        reclaim_focus = true;
                    }

                    ImGui::SetItemDefaultFocus();
                    if (reclaim_focus)
                        ImGui::SetKeyboardFocusHere(-1);

                ImGui::EndChild();

            ImGui::EndGroup();
        ImGui::End();
    }

    bool brush_window_enabled = false;
    std::array <float, 3> selected_color = { 1.f, 1.f, 1.f };
    void draw_brush_window(space::state_t& state) {
        selected_color = state.tool.color.to_float();

        ImGui::Begin("Brush", &brush_window_enabled);

            if (ImGui::ColorPicker3("color picker", (float*)&selected_color)) {
                state.tool.color = fun::rgb_t::from_float(::selected_color[0], ::selected_color[1], ::selected_color[2]);
            }

            ImGui::NewLine();

            if (ImGui::Button("Eyedrop")) {
                state.tool.mode = space::tool_mode_t::eyedrop;
            }

        ImGui::End();
    }

    bool connect_to_server_window_enabled = false;
    char ip_input[32] = "localhost";
    char port_input[8] = "8001";
    void draw_connect_to_server_window(space::state_t& state) {
        ImGui::Begin("Connect", &connect_to_server_window_enabled);

            ImGui::InputText(" Ip Address", ip_input, sizeof ip_input);
            ImGui::InputText(" Port", port_input, sizeof port_input);
            ImGui::NewLine();

            if (ImGui::Button("Join")) {
                if (state.client.connect(ip_input, std::stoi(port_input))) {
                    connect_to_server_window_enabled = false;
                    
                    state.canvas.clear();

                    space::slave::request_all_chunks(state);
                }
            }

        ImGui::End();
    }

    void draw_nav_bar(space::state_t& state) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("World")) {
                if (!state.client.check_connection() && ImGui::MenuItem("Connect")) {
                    connect_to_server_window_enabled = true;
                }
                
                if (state.client.check_connection() && ImGui::MenuItem("Disconnect")) {
                    state.client.disconnect();

                    state.canvas.clear();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window")) {
                if (ImGui::MenuItem("Brush")) {
                    brush_window_enabled = true;
                }

                if (ImGui::MenuItem("Console")) {
                    console_window_enabled = true;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}

void space::interf::draw(state_t& state) {
    ::draw_nav_bar(state);

    if (::connect_to_server_window_enabled) ::draw_connect_to_server_window(state);
    if (::brush_window_enabled) ::draw_brush_window(state);
    if (::console_window_enabled) ::draw_console_window(state);
}