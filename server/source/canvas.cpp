#include "canvas.h"

void space::canvas_t::init_chunk(chunk_pos_t chunk_pos) {
    if (!m_chunks.contains(chunk_pos)) m_chunks.emplace(chunk_pos, new chunk_t());
}

bool space::canvas_t::has_chunk(chunk_pos_t chunk_pos) {
    return m_chunks.contains(chunk_pos);
}

space::chunk_t* space::canvas_t::get_chunk(chunk_pos_t chunk_pos) {    
    return m_chunks[chunk_pos];
}

fun::rgb_t space::canvas_t::get_color(grid_pos_t grid_pos) {
    return get_color(space::grid_to_chunk(grid_pos), space::grid_to_texel(grid_pos));
}

fun::rgb_t space::canvas_t::get_color(chunk_pos_t chunk_pos, texel_pos_t texel_pos) {
    return get_chunk(chunk_pos)->get_color(texel_pos);
}

void space::canvas_t::set_color(grid_pos_t grid_pos, fun::rgb_t color) {
    set_color(space::grid_to_chunk(grid_pos), space::grid_to_texel(grid_pos), color);
}

void space::canvas_t::set_color(chunk_pos_t chunk_pos, texel_pos_t texel_pos, fun::rgb_t color) {
    get_chunk(chunk_pos)->set_color(texel_pos, color);
}

space::canvas_t::chunk_map_t& space::canvas_t::get_chunks() {
    return m_chunks;
}