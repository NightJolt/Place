#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/_math.h"

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

struct color_t {
    color_t() : r(0), g(0), b(0) {}
    color_t(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

template <int8_t X, int8_t Y>
class chunk_t {
public:
    inline color_t get_color(uint8_t x, uint8_t y) { return data[x][y]; }
    inline void set_color(uint8_t x, uint8_t y, color_t color) { data[x][y] = color; }

private:
    color_t data[X][Y];
};

class canvas_t {
public:
    static constexpr uint8_t size_x = 16;
    static constexpr uint8_t size_y = 16;

    chunk_t <size_x, size_y>* get_chunk(int32_t x, int32_t y) {
        auto coord = fun::vec2i_t { (x < 0 ? (x * -1 - 1) : x), (y < 0 ? (y * -1 - 1) : y) };

        if (!data.contains(coord)) {
            data.emplace(coord, new chunk_t <size_x, size_y> ());
        }
        
        return data[coord];
    }

    color_t get_color(int32_t x, int32_t y) {
        return get_chunk(x, y)->get_color(fun::math::mod(x, size_x), fun::math::mod(y, size_y));
    }

    void set_color(int32_t x, int32_t y, color_t color) {
        get_chunk(x, y)->set_color(fun::math::mod(x, size_x), fun::math::mod(y, size_y), color);
    }

private:
    std::unordered_map <fun::vec2i_t, chunk_t <size_x, size_y>*,
        decltype([](const fun::vec2i_t& v) -> size_t const { return std::hash <fun::vec2i_t> () (v); }),
        decltype([](const fun::vec2i_t& a, const fun::vec2i_t& b) -> bool const { return a == b; })> data;
};

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Server"));
    auto* window = fun::winmgr::main_window;

    // fun::server_t server;
    // server.launch(8001);

    canvas_t canvas;

    canvas.set_color(3, 4, color_t(2, 2, 2));
    println(+canvas.get_color(3, 4).r);
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();

        // server.listen();

        window->display(sf::Color::Black);
    }

    // server.terminate();

    fun::winmgr::close();

    return 0;
};