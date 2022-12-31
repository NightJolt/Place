#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window/window_manager.h"
#include "../FunEngine2D/core/include/render/window/window_data.h"
#include "../FunEngine2D/core/include/render/window/window.h"
#include "../FunEngine2D/core/include/render/sprite.h"
#include "../FunEngine2D/core/include/tools/debugger.h"
#include "../FunEngine2D/core/include/input.h"
#include "../FunEngine2D/core/include/resources.h"

#include "../common/include/space.h"

#include "slave.h"
#include "canvas.h"
#include "interface.h"
#include "state.h"

int main () {
    fun::render::winmgr::init(fun::render::window_data_t("Place Client"));
    auto& window = fun::render::winmgr::get_main_window();

    window.set_vsync(false);
    window.target_framerate(60);
    window.set_world_view({ 0, 0 }, 64);

    fun::resources::load_texture("focus", "focus.png");

    window.register_event_handler(sf::Event::MouseWheelMoved, [](fun::render::window_t& window, const sf::Event& event) {
        float zoom_value = event.mouseWheel.delta > 0 ? .9f : 1.1f;
        float zoom = window.get_zoom() * zoom_value;
        
        if (zoom < .1f || zoom > 5.f) return;
        
        window.zoom_into(window.get_mouse_screen_position(), zoom_value);
    });
    
    window.set_cursor_visible(false);

    space::state_t state;
    state.tool.mode = space::tool_mode_t::brush;
    state.tool.color = fun::rgb_t::white;
    state.tool.fill_line_gaps = true;
    state.batch_max_texels = 256;
    state.batch_send_interval = 1.f;
    state.batch_cooldown = state.batch_send_interval;

    fun::render::sprite_t focus;
    focus.bind_texture(fun::resources::get_texture("focus"));
    
    while (window.is_open()) {
        fun::time::recalculate();
        fun::render::winmgr::update();
        fun::input::listen(window.is_focused());

        if (fun::input::pressed(sf::Keyboard::B)) state.tool.mode = space::tool_mode_t::brush;
        if (fun::input::pressed(sf::Keyboard::I)) state.tool.mode = space::tool_mode_t::eyedrop;
        if (fun::input::pressed(sf::Keyboard::E)) state.tool.mode = space::tool_mode_t::erase;
        if (fun::input::pressed(sf::Keyboard::G)) state.tool.mode = space::tool_mode_t::fill;

        bool mouse_was_active = false;
        fun::vec2f_t mouse_pos = window.get_mouse_world_position();

        if (fun::input::hold(sf::Mouse::Left)) {
            switch(state.tool.mode) {
                case space::tool_mode_t::brush:
                case space::tool_mode_t::erase: {
                    fun::rgb_t color = (state.tool.mode == space::tool_mode_t::brush) ? state.tool.color : fun::rgb_t::black;

                    if (!state.tool.last_frame_mouse_active || !state.tool.fill_line_gaps) {
                        space::slave::send_texel(state, space::world_to_grid(mouse_pos), state.tool.color);
                    } else {
                        fun::vec2f_t start = space::world_to_grid(state.tool.last_frame_mouse_position);
                        fun::vec2f_t end = space::world_to_grid(mouse_pos);
                        
                        const bool steep = (std::fabs(end.y - start.y) > std::fabs(end.x - start.x));

                        if (steep) {
                            std::swap(start.x, start.y);
                            std::swap(end.x, end.y);
                        }

                        if (start.x > end.x) {
                            std::swap(start, end);
                        }

                        const float dx = end.x - start.x;
                        const float dy = std::fabs(end.y - start.y);
                        
                        float error = dx * .5f;
                        const int32_t ystep = (start.y < end.y) ? 1 : -1;

                        int32_t x = start.x;
                        int32_t y = start.y;

                        const int32_t end_x = end.x;

                        for (; x <= end_x; x++) {
                            if (steep) {
                                space::slave::send_texel(state, fun::vec2i_t(y, x), color);
                            }  else {
                                space::slave::send_texel(state, fun::vec2i_t(x, y), color);
                            }
                                    
                            error -= dy;

                            if (error < 0) {
                                y += ystep;
                                error += dx;
                            }
                        }

                        state.tool.last_grid_position = { x, y };
                    }

                    state.tool.last_frame_mouse_position = mouse_pos;
                    mouse_was_active = true;

                    break;
                }

                case space::tool_mode_t::eyedrop: {
                    state.tool.color = state.canvas.get_color(space::world_to_grid(window.get_mouse_world_position()));
                    state.tool.mode = space::tool_mode_t::brush;

                    break;
                }

                case space::tool_mode_t::fill: {
                    std::queue <fun::vec2i_t> queue;
                    fun::unordered_map_vec2_t <uint32_t, bool> visited;

                    fun::vec2i_t start = space::world_to_grid(mouse_pos);
                    fun::rgb_t start_color = state.canvas.get_color(start);

                    fun::rgb_t color = state.tool.color;

                    queue.push(start);

                    uint32_t max_iterations = 10000;

                    while (!queue.empty() && max_iterations--) {
                        fun::vec2i_t pos = queue.front();
                        queue.pop();

                        if (visited[pos]) continue;
                        visited[pos] = true;

                        if (state.canvas.get_color(pos) != start_color) continue;

                        space::slave::send_texel(state, pos, color);

                        queue.push({ pos.x + 1, pos.y });
                        queue.push({ pos.x - 1, pos.y });
                        queue.push({ pos.x, pos.y + 1 });
                        queue.push({ pos.x, pos.y - 1 });
                    }
                }
            }
        }

        state.tool.last_frame_mouse_active = mouse_was_active;

        // {
        //     uint32_t margin = 1;

        //     fun::vec2f_t tl_view = window->world_view.getCenter() - window->world_view.getSize() * .5f;
        //     fun::vec2f_t br_view = window->world_view.getCenter() + window->world_view.getSize() * .5f;

        //     fun::vec2i_t tl_chunk = space::grid_to_chunk(space::world_to_grid(tl_view));
        //     fun::vec2i_t br_chunk = space::grid_to_chunk(space::world_to_grid(br_view));

        //     tl_chunk.x -= margin;
        //     tl_chunk.y -= margin;

        //     br_chunk.x += margin;
        //     br_chunk.y += margin;

        //     for (auto& [chunk_pos, chunk] : state.canvas.get_chunks()) {
        //         if (chunk_pos.x < tl_chunk.x || chunk_pos.x > br_chunk.x || chunk_pos.y < tl_chunk.y || chunk_pos.y > br_chunk.y) {
        //             chunk->synced = false;

        //             continue;
        //         }

        //         if (!chunk->synced) {
        //             space::slave::request_chunk(state, chunk_pos);

        //             chunk->synced = true;
        //         }
        //     }

        //     for (int32_t x = tl_chunk.x; x <= br_chunk.x; x++) {
        //         for (int32_t y = tl_chunk.y; y <= br_chunk.y; y++) {
        //             space::chunk_t* chunk = state.canvas.get_chunk({ x, y });

        //             if (!chunk->synced) {
        //                 space::slave::request_chunk(state, fun::vec2f_t(x, y));

        //                 chunk->synced = true;
        //             }
        //         }
        //     }
        // }

        auto grid_pos = space::world_to_grid(window.get_mouse_world_position());

        focus.set_position(grid_pos);
        focus.set_color(state.canvas.get_color(grid_pos).invert());
        
        space::slave::step(state, fun::time::delta_time());

        space::interf::draw(state);
        
        // fun::debugger::display();

        window.draw_world(state.canvas, 0);
        window.draw_world(focus, 1);

        window.display(fun::rgb_t::black);
    }

    state.client.disconnect();

    fun::render::winmgr::close();

    return 0;
};