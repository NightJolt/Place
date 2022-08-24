#include "chunk.h"

space::chunk_t::chunk_t() {
    std::fill(*data, *data + space::chunk_size * space::chunk_size, fun::rgb_t { 0, 0, 0 });
}

fun::rgb_t space::chunk_t::get_color(fun::vec2_t <space::texel_int_t> p) {
    return data[p.x][p.y];
}

void space::chunk_t::set_color(fun::vec2_t <space::texel_int_t> p, fun::rgb_t color) {
    data[p.x][p.y] = color;
}

fun::rgb_t* space::chunk_t::get_data() {
    return *data;
}