#include "chunk.h"

space::chunk_t::chunk_t(fun::vec2_t <space::chunk_int_t> p) {
    vertices.resize(space::chunk_size * space::chunk_size * 4);

    fun::vec2f_t position(space::chunk_to_grid(p));

    uint32_t i = 0;
    auto initial_color = fun::rgb_t::black.to_sf();

    for (texel_int_t x = 0; x < space::chunk_size; x++) {
        for (texel_int_t y = 0; y < space::chunk_size; y++) {
            vertices[i].position = (position + fun::vec2f_t(x, y)).to_sf();
            vertices[i + 1].position = (position + fun::vec2f_t(x + 1, y)).to_sf();
            vertices[i + 2].position = (position + fun::vec2f_t(x + 1, y + 1)).to_sf();
            vertices[i + 3].position = (position + fun::vec2f_t(x, y + 1)).to_sf();

            vertices[i].color = initial_color;
            vertices[i + 1].color = initial_color;
            vertices[i + 2].color = initial_color;
            vertices[i + 3].color = initial_color;

            i += 4;
        }
    }
}

void space::chunk_t::set_data(const std::vector <fun::rgb_t>& data) {
    uint32_t i = 0;

    for (texel_int_t x = 0; x < space::chunk_size; x++) {
        for (texel_int_t y = 0; y < space::chunk_size; y++) {
            set_color({ x, y }, data[i++]);
        }
    }
}

const std::vector <sf::Vertex>& space::chunk_t::get_vertices() {
    return vertices;
}

fun::rgb_t space::chunk_t::get_color(fun::vec2_t <space::texel_int_t> p) {
    return vertices[space::texel_to_array(p)].color;
}

void space::chunk_t::set_color(fun::vec2_t <space::texel_int_t> p, fun::rgb_t color) {
    uint32_t i = space::texel_to_array(p);

    vertices[i++].color = color.to_sf();
    vertices[i++].color = color.to_sf();
    vertices[i++].color = color.to_sf();
    vertices[i].color = color.to_sf();
}