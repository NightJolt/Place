#include "space.h"

space::texel_pos_t space::array_to_texel(int32_t i) {
    return fun::vec2_t <space::texel_int_t> (i / chunk_size, i % chunk_size);
}

int32_t space::texel_to_array(texel_pos_t p) {
    return (p.x * chunk_size + p.y) * 4;
}

space::grid_pos_t space::world_to_grid(fun::vec2f_t world_pos) {
    return grid_pos_t { (grid_int_t)(world_pos.x - (world_pos.x < 0)), (grid_int_t)(world_pos.y - (world_pos.y < 0)) };
}

space::chunk_pos_t space::grid_to_chunk(grid_pos_t grid_pos) {
    chunk_int_t x = grid_pos.x + (grid_pos.x < 0);
    chunk_int_t y = grid_pos.y + (grid_pos.y < 0);
    
    x /= space::chunk_size;
    y /= space::chunk_size;

    x -= grid_pos.x < 0;
    y -= grid_pos.y < 0;

    return chunk_pos_t { x, y };
}

space::grid_pos_t space::chunk_to_grid(chunk_pos_t chunk_pos) {
    return chunk_pos * (space::chunk_int_t)chunk_size;
}

space::texel_pos_t space::grid_to_texel(grid_pos_t p) {
    return texel_pos_t { (texel_int_t)fun::math::mod(p.x, space::chunk_size), (texel_int_t)fun::math::mod(p.y, space::chunk_size) };
}

fun::str_t space::chunk::encode(chunk_pos_t chunk_pos, fun::rgb_t* data) {
    static constexpr size_t mem_size = sizeof chunk_pos_t + chunk_volume * sizeof fun::rgb_t;

    fun::str_t encoded_data;
    encoded_data.resize(mem_size);

    char* ptr = &encoded_data[0];

    *(chunk_pos_t*)ptr = chunk_pos;
    ptr += sizeof chunk_pos_t;

    for (uint32_t i = 0; i < chunk_volume; i++) {
        *(fun::rgb_t*)ptr = *(data + i);
        ptr += sizeof fun::rgb_t;
    }

    return encoded_data;
}

space::chunk_pos_t space::chunk::decode_position(const fun::str_t& encoded_data, uint32_t offset) {
    return *(chunk_pos_t*)&encoded_data[offset];
}

void space::chunk::decode_colors(const fun::str_t& encoded_data, fun::rgb_t* data, uint32_t offset) {
    const char* ptr = &encoded_data[offset];
    ptr += sizeof chunk_pos_t;
    
    for (uint32_t i = 0; i < chunk_volume; i++) {
        *(data + i) = *(fun::rgb_t*)ptr;
        ptr += sizeof fun::rgb_t;
    }
}