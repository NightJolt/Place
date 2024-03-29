#include "canvas.h"

space::chunk_t* space::canvas_t::get_chunk(chunk_pos_t p) {
    if (!m_chunks.contains(p)) {
        m_chunks.emplace(p, new chunk_t(p));
    }
    
    return m_chunks[p];
}

space::canvas_t::chunks_map_t& space::canvas_t::get_chunks() {
    return m_chunks;
}

bool space::canvas_t::has_chunk(chunk_pos_t chunk_pos) {
    return m_chunks.contains(chunk_pos);
}

fun::rgb_t space::canvas_t::get_color(grid_pos_t grid_pos) {
    return get_color(space::grid_to_chunk(grid_pos), space::grid_to_texel(grid_pos));
}

fun::rgb_t space::canvas_t::get_color(chunk_pos_t chunk_pos, texel_pos_t texel_pos) {
    return get_chunk(chunk_pos)->get_color(texel_pos);
}

void space::canvas_t::set_color(grid_pos_t p, fun::rgb_t color) {
    set_color(space::grid_to_chunk(p), space::grid_to_texel(p), color);
}

void space::canvas_t::set_color(chunk_pos_t chunk_pos, texel_pos_t texel_pos, fun::rgb_t color) {
    get_chunk(chunk_pos)->set_color(texel_pos, color);
}

void space::canvas_t::clear() {
    m_chunks.clear();
}

void space::canvas_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& [_, chunk] : m_chunks) {
        target.draw(&chunk->get_vertices()[0], chunk->get_vertices().size(), sf::Quads, states);
    }
}