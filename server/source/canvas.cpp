#include "canvas.h"

space::chunk_t* space::canvas_t::get_chunk(int32_t x, int32_t y) {
    auto coord = fun::vec2i_t { (x < 0 ? (x * -1 - 1) : x), (y < 0 ? (y * -1 - 1) : y) };

    if (!data.contains(coord)) {
        data.emplace(coord, new chunk_t());
    }
    
    return data[coord];
}

fun::rgb_t space::canvas_t::get_color(int32_t x, int32_t y) {
    return get_chunk(x, y)->get_color(fun::math::mod(x, chunk_size_x), fun::math::mod(y, chunk_size_y));
}

void space::canvas_t::set_color(int32_t x, int32_t y, fun::rgb_t color) {
    get_chunk(x, y)->set_color(fun::math::mod(x, chunk_size_x), fun::math::mod(y, chunk_size_y), color);
}