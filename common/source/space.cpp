#include "space.h"

fun::vec2_t <space::texel_int_t> space::array_to_texel(int32_t i) {
    return fun::vec2_t <space::texel_int_t> (i / chunk_size, i % chunk_size);
}

int32_t space::texel_to_array(fun::vec2_t <space::texel_int_t> p) {
    return (p.x * chunk_size + p.y) * 4;
}

fun::vec2_t <space::grid_int_t> space::world_to_grid(fun::vec2f_t p) {
    return fun::vec2_t <grid_int_t> (p.x - (p.x < 0), p.y - (p.y < 0));
}

fun::vec2_t <space::chunk_int_t> space::grid_to_chunk(fun::vec2_t <space::grid_int_t> p) {
    chunk_int_t x = p.x + (p.x < 0);
    chunk_int_t y = p.y + (p.y < 0);
    
    x /= space::chunk_size;
    y /= space::chunk_size;

    x -= p.x < 0;
    y -= p.y < 0;

    return fun::vec2_t <space::chunk_int_t> (x, y);
}

fun::vec2_t <space::grid_int_t> space::chunk_to_grid(fun::vec2_t <space::chunk_int_t> p) {
    return p * (space::chunk_int_t)chunk_size;
}

fun::vec2_t <space::texel_int_t> space::grid_to_texel(fun::vec2_t <grid_int_t> p) {
    return fun::vec2_t <space::texel_int_t> (fun::math::mod(p.x, space::chunk_size), fun::math::mod(p.y, space::chunk_size));
}

std::string space::chunk::encode(chunk_pos_t chunk_pos, fun::rgb_t* data) {
    static constexpr size_t mem_size = sizeof chunk_pos_t + chunk_volume * sizeof fun::rgb_t;

    std::string encoded_data;
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

space::chunk_pos_t space::chunk::decode_position(const fun::str_t& encoded_data) {
    return *(chunk_pos_t*)&*encoded_data.begin();
}

void space::chunk::decode_colors(const fun::str_t& encoded_data, fun::rgb_t* data) {
    const char* ptr = &encoded_data[0];
    ptr += sizeof chunk_pos_t;
    
    for (uint32_t i = 0; i < chunk_volume; i++) {
        *(data + i) = *(fun::rgb_t*)ptr;
        ptr += sizeof fun::rgb_t;
    }
}