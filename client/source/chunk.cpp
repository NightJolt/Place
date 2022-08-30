#include "chunk.h"

space::chunk_t::chunk_t(chunk_pos_t chunk_pos) {
    m_vertices.resize(chunk_volume * 4);

    fun::vec2f_t position(chunk_to_grid(chunk_pos));

    uint32_t i = 0;
    auto initial_color = fun::rgb_t::black.to_sf();

    for (texel_int_t x = 0; x < chunk_size; x++) {
        for (texel_int_t y = 0; y < chunk_size; y++) {
            m_vertices[i].position = (position + fun::vec2f_t(x, y)).to_sf();
            m_vertices[i + 1].position = (position + fun::vec2f_t(x + 1, y)).to_sf();
            m_vertices[i + 2].position = (position + fun::vec2f_t(x + 1, y + 1)).to_sf();
            m_vertices[i + 3].position = (position + fun::vec2f_t(x, y + 1)).to_sf();

            m_vertices[i].color = initial_color;
            m_vertices[i + 1].color = initial_color;
            m_vertices[i + 2].color = initial_color;
            m_vertices[i + 3].color = initial_color;

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
    return m_vertices;
}

fun::rgb_t space::chunk_t::get_color(texel_pos_t texel_pos) {
    return m_vertices[texel_to_array(texel_pos)].color;
}

void space::chunk_t::set_color(texel_pos_t texel_pos, fun::rgb_t color) {
    uint32_t i = texel_to_array(texel_pos);

    m_vertices[i++].color = color.to_sf();
    m_vertices[i++].color = color.to_sf();
    m_vertices[i++].color = color.to_sf();
    m_vertices[i].color = color.to_sf();
}