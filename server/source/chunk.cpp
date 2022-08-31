#include "chunk.h"

space::chunk_t::chunk_t() {
    std::fill(*m_colors, *m_colors + space::chunk_size * space::chunk_size, fun::rgb_t { 0, 0, 0 });
}

fun::rgb_t space::chunk_t::get_color(fun::vec2_t <space::texel_int_t> p) {
    return m_colors[p.y][p.x];
}

void space::chunk_t::set_color(fun::vec2_t <space::texel_int_t> p, fun::rgb_t color) {
    m_colors[p.y][p.x] = color;

    updated = true;
}

fun::rgb_t* space::chunk_t::get_colors() {
    return *m_colors;
}