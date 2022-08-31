#include "texel_batch.h"

space::texel_batch_t::texel_batch_t() : m_total_texels(0) {}

void space::texel_batch_t::add_texel(grid_pos_t grid_pos, fun::rgb_t color) {
    add_texel(grid_to_chunk(grid_pos), grid_to_texel(grid_pos), color);
}

void space::texel_batch_t::add_texel(chunk_pos_t chunk_pos, texel_pos_t texel_pos, fun::rgb_t color) {
    if (m_data.count(chunk_pos) == 0) m_data[chunk_pos] = new std::vector <texel_t> ();

    m_data[chunk_pos]->emplace_back(texel_pos, color);

    m_total_texels++;
}

fun::str_t space::texel_batch_t::to_cmd() {
    fun::str_t cmd;

    // 1 + data.size() * sizeof chunk_pos_t + data.size() * sizeof chunk_volume_t + total_texels * sizeof texel_t + 1
    cmd.resize(1 + m_data.size() * (sizeof chunk_pos_t + sizeof chunk_volume_t) + m_total_texels * sizeof texel_t);
    char* buffer = &cmd[0];

    cmd[0] = server_cmd_t::receive_batch;

    buffer += 1;

    for (auto& [chunk_pos, texels] : m_data) {
        *(chunk_pos_t*)buffer = chunk_pos;
        buffer += sizeof chunk_pos_t;

        *(chunk_volume_t*)buffer = (chunk_volume_t)texels->size();
        buffer += sizeof chunk_volume_t;

        for (auto texel : *texels) {
            *(texel_pos_t*)buffer = texel.pos;
            buffer += sizeof texel_pos_t;
            
            *(fun::rgb_t*)buffer = texel.color;
            buffer += sizeof fun::rgb_t;
        }
    }

    return cmd;
}

void space::texel_batch_t::from_cmd(const fun::str_t& cmd) {
    const char* buffer = &cmd[0];
    
    buffer += 1;

    while (buffer < &*(cmd.end() - 1)) {
        chunk_pos_t chunk_pos = *(chunk_pos_t*)buffer;
        buffer += sizeof chunk_pos_t;

        uint32_t texel_count = *(chunk_volume_t*)buffer;
        buffer += sizeof chunk_volume_t;

        for (uint32_t i = 0; i < texel_count; i++) {
            add_texel(chunk_pos, *(texel_pos_t*)buffer, *(fun::rgb_t*)(buffer + sizeof texel_pos_t));
            buffer += sizeof texel_pos_t + sizeof fun::rgb_t;
        }
    }
}

uint32_t space::texel_batch_t::get_total_texels() {
    return m_total_texels;
}

space::texel_batch_t::chunk_texel_map_t& space::texel_batch_t::get_data() {
    return m_data;
}

void space::texel_batch_t::clear() {
    m_data.clear();

    m_total_texels = 0;
}