#include "canvas.h"

space::chunk_t* space::canvas_t::get_chunk(fun::vec2_t <space::chunk_pos_t> p) {
    if (!chunks.contains(p)) {
        chunks.emplace(p, new chunk_t());
    }
    
    return chunks[p];
}

fun::rgb_t space::canvas_t::get_color(fun::vec2_t <space::grid_pos_t> p) {
    return get_chunk(space::grid_to_chunk(p))->get_color(space::grid_to_texel(p));
}

void space::canvas_t::set_color(fun::vec2_t <space::grid_pos_t> p, fun::rgb_t color) {
    get_chunk(space::grid_to_chunk(p))->set_color(space::grid_to_texel(p), color);
}