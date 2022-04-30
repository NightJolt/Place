#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/color.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/tools/command.h"
#include "../FunEngine2D/core/include/tools/debugger.h"
#include "../FunEngine2D/core/include/_math.h"

#include "../common/include/chunk.h"

namespace std {
    template <> struct hash<fun::vec2i_t> {
        size_t operator()(const fun::vec2i_t& v) const {
            return (v.x + v.y) * (v.x + v.y + 1) >> 1 + v.x;
        }
    };
}

// namespace std
// {
//     template<> struct less<fun::vec2i_t>
//     {
//        bool operator() (const fun::vec2i_t& a, const fun::vec2i_t& b) const
//        {
//            return (a.x * a.y) < (b.x * b.y);
//        }
//     };
// }

template <uint8_t X, uint8_t Y>
class chunk_t {
public:

    inline fun::rgb_t get_color(uint8_t x, uint8_t y) { return data[x][y]; }
    inline void set_color(uint8_t x, uint8_t y, fun::rgb_t color) { data[x][y] = color; }

    inline fun::rgb_t* get_data() { return *data; }

private:

    fun::rgb_t data[X][Y];
};

class canvas_t {
public:

    canvas_t() {}

    chunk_t <space::chunk_size_x, space::chunk_size_y>* get_chunk(int32_t x, int32_t y) {
        auto coord = fun::vec2i_t { (x < 0 ? (x * -1 - 1) : x), (y < 0 ? (y * -1 - 1) : y) };

        if (!data.contains(coord)) {
            data.emplace(coord, new chunk_t <space::chunk_size_x, space::chunk_size_y> ());
        }
        
        return data[coord];
    }

    fun::rgb_t get_color(int32_t x, int32_t y) {
        return get_chunk(x, y)->get_color(fun::math::mod(x, space::chunk_size_x), fun::math::mod(y, space::chunk_size_y));
    }

    void set_color(int32_t x, int32_t y, fun::rgb_t color) {
        get_chunk(x, y)->set_color(fun::math::mod(x, space::chunk_size_x), fun::math::mod(y, space::chunk_size_y), color);
    }

private:

    std::unordered_map <fun::vec2i_t, chunk_t <space::chunk_size_x, space::chunk_size_y>*,
        decltype([](const fun::vec2i_t& v) -> size_t const { return std::hash <fun::vec2i_t> () (v); }),
        decltype([](const fun::vec2i_t& a, const fun::vec2i_t& b) -> bool const { return a == b; })> data;
};

// s - set pixel color (x, y, r, g, b)
// f - fetch chunk data (x, y)
// b - set chunk subscribtion range

int main () {
    auto* window = fun::winmgr::main_window;

    fun::server_t server;
    server.launch(8001);

    canvas_t canvas;
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();

        server.listen();
        auto& packet_storage = server.get_packets();

        if (!packet_storage.empty()) {
            fun::packet_storage_t::packet_t packet = packet_storage.read();
            fun::command_t command_parser = fun::command_t(packet.data);
            const std::string& command = command_parser.get_command();

            println(command);

            if (command == "f") {
                space::chunk_pos_t x = std::stoi(command_parser.get_arg(0));
                space::chunk_pos_t y = std::stoi(command_parser.get_arg(1));

                server.send_all(space::chunk::encode(x, y, space::chunk_size_x, space::chunk_size_y, canvas.get_chunk(x, y)->get_data()), packet.sender);
            } else if (command == "s") {
                fun::vec2_t pos = {
                    std::stoi(command_parser.get_arg(0)),
                    std::stoi(command_parser.get_arg(1))
                };

                fun::rgb_t color = {
                    (uint8_t)std::stoi(command_parser.get_arg(2)),
                    (uint8_t)std::stoi(command_parser.get_arg(3)),
                    (uint8_t)std::stoi(command_parser.get_arg(4))
                };

                canvas.set_color(pos.x, pos.y, color);

                fun::rgb_t col = canvas.get_color(pos.x, pos.y);

                printf("color: %d %d %d\n", col.r, col.g, col.b);
            }
        }

        fun::debugger::display();

        window->display(sf::Color::Black);
    }

    server.terminate();

    fun::winmgr::close();

    return 0;
};