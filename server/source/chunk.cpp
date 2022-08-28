#include "chunk.h"

space::chunk_t::chunk_t() {
    std::fill(*m_colors, *m_colors + space::chunk_size * space::chunk_size, fun::rgb_t { 0, 0, 0 });

    updated = true;
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

fun::str_t space::chunk_t::to_str() {
    fun::str_t res;
    res.resize(space::chunk_size * space::chunk_size * sizeof fun::rgb_t + 1);

    char* ptr = &res[0];

    for (uint32_t y = 0; y < space::chunk_size; ++y) {
        for (uint32_t x = 0; x < space::chunk_size; ++x) {
            *(fun::rgb_t*)ptr = m_colors[y][x];

            ptr += sizeof fun::rgb_t;
        }
    }

    return res;
}