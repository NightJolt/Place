#include "texel_batch.h"

void space::texel_batch::add_texel(grid_pos_t grid_pos, fun::rgb_t color) {
    add_texel(grid_to_chunk(grid_pos), grid_to_texel(grid_pos), color);
}

void space::texel_batch::add_texel(chunk_pos_t chunk_pos, texel_pos_t texel_pos, fun::rgb_t color) {
    m_data[chunk_pos].emplace_back(texel_pos);

    m_total_texels++;
}

fun::str_t space::texel_batch::to_str() {
    fun::str_t cmd;

    // 1 + data.size() * sizeof chunk_pos_t + data.size() * sizeof uint16_t + total_texels * sizeof texel_t + 1
    cmd.resize(m_data.size() * (sizeof chunk_pos_t + sizeof uint16_t) + m_total_texels * sizeof texel_t + 2);
    char* buffer = &cmd[0];

    cmd[0] = server_cmd_t::send_batch;

    buffer += 1;

    for (auto& [chunk_pos, texels] : m_data) {
        *(chunk_pos_t*)buffer = chunk_pos;
        buffer += sizeof chunk_pos_t;

        *(uint16_t*)buffer = (uint16_t)texels.size();
        buffer += sizeof uint16_t;

        for (auto texel : texels) {
            *(texel_pos_t*)buffer = texel.pos;
            buffer += sizeof texel_pos_t;
            
            *(fun::rgb_t*)buffer = texel.color;
            buffer += sizeof fun::rgb_t;
        }
    }

    return cmd;
}

void space::texel_batch::from_str(const fun::str_t& cmd) {
    const char* buffer = &cmd[0];
    uint32_t ind = 1;

    while (ind < cmd.size()) {
        chunk_pos_t chunk_pos = *(chunk_pos_t*)buffer;
        buffer += sizeof chunk_pos_t;

        uint32_t texel_count = *(uint16_t*)buffer;
        buffer += sizeof uint16_t;

        for (uint32_t i = 0; i < texel_count; i++) {
            add_texel(chunk_pos, *(texel_pos_t*)buffer, *(fun::rgb_t*)(buffer + sizeof texel_pos_t));
            buffer += sizeof texel_pos_t + sizeof fun::rgb_t;
        }
    }
}

uint32_t space::texel_batch::get_total_texels() {
    return m_total_texels;
}

void space::texel_batch::clear() {
    m_data.clear();
}