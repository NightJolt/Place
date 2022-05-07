#include "canvas.h"

space::chunk_t* space::canvas_t::get_chunk(space::chunk_pos_t x, space::chunk_pos_t y) {
    fun::vec2_t <space::chunk_pos_t> coord = { x, y };

    if (!data.contains(coord)) {
        data.emplace(coord, new chunk_t(x, y));
    }
    
    return data[coord];
}

fun::rgb_t space::canvas_t::get_color(space::grid_pos_t x, space::grid_pos_t y) {
    return get_chunk(x, y)->get_color(fun::math::mod(x, space::chunk_size), fun::math::mod(y, space::chunk_size));
}

void space::canvas_t::set_color(space::grid_pos_t x, space::grid_pos_t y, fun::rgb_t color) {
    get_chunk(x, y)->set_color(fun::math::mod(x, space::chunk_size), fun::math::mod(y, space::chunk_size), color);
}