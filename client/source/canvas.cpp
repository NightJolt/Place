#include "canvas.h"

space::chunk_t* space::canvas_t::get_chunk(fun::vec2_t <space::chunk_int_t> p) {
    if (!chunks.contains(p)) {
        chunks.emplace(p, new chunk_t(p));
    }
    
    return chunks[p];
}

fun::rgb_t space::canvas_t::get_color(fun::vec2_t <space::grid_int_t> p) {
    return get_chunk(space::grid_to_chunk(p))->get_color(space::grid_to_texel(p));
}

void space::canvas_t::set_color(fun::vec2_t <space::grid_int_t> p, fun::rgb_t color) {
    get_chunk(space::grid_to_chunk(p))->set_color(space::grid_to_texel(p), color);
}

void space::canvas_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& [_, chunk] : chunks) {
        target.draw(&chunk->get_vertices()[0], chunk->get_vertices().size(), sf::Quads, states);
    }
}

void space::canvas_t::clear() {
    chunks.clear();
}