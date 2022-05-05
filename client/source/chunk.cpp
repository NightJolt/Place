#include "chunk.h"

space::chunk_t::chunk_t(space::chunk_pos_t x, space::chunk_pos_t y) : position(sf::Vector2f(x, y)) {
    vertices.resize(space::chunk_size * space::chunk_size * 4);

    uint32_t i = 0;

    for (uint32_t x = 0; x < space::chunk_size; x++) {
        for (uint32_t y = 0; y < space::chunk_size; y++) {
            vertices[i++].position = (position + fun::vec2f_t(x, y)).to_sf();
            vertices[i++].position = (position + fun::vec2f_t(x + 1, y)).to_sf();
            vertices[i++].position = (position + fun::vec2f_t(x + 1, y + 1)).to_sf();
            vertices[i++].position = (position + fun::vec2f_t(x, y + 1)).to_sf();
        }
    }
}

fun::rgb_t space::chunk_t::get_color(space::texel_pos_t x, space::texel_pos_t y) {
    return vertices[space::texel_to_array({ x, y })].color;
}

void space::chunk_t::set_color(space::texel_pos_t x, space::texel_pos_t y, fun::rgb_t color) {
    uint32_t i = space::texel_to_array({ x, y });

    vertices[i++].color = color.to_sf();
    vertices[i++].color = color.to_sf();
    vertices[i++].color = color.to_sf();
    vertices[i].color = color.to_sf();
}