#include "chunk.h"

space::chunk_t::chunk_t() {
    std::fill(*data, *data + space::chunk_size * space::chunk_size, fun::rgb_t { 0, 0, 0 });
}

fun::rgb_t space::chunk_t::get_color(uint8_t x, uint8_t y) {
    return data[x][y];
}

void space::chunk_t::set_color(uint8_t x, uint8_t y, fun::rgb_t color) {
    data[x][y] = color;
}

fun::rgb_t* space::chunk_t::get_data() {
    return *data;
}