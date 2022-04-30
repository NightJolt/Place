#include "chunk.h"

std::string space::chunk::encode(chunk_pos_t px, chunk_pos_t py, chunk_size_t sx, chunk_size_t sy, fun::rgb_t* data) {
    size_t mem_size = sizeof chunk_pos_t * 2 + sx * sy * sizeof fun::rgb_t + 1;

    std::string encoded_data;
    encoded_data.resize(mem_size);

    char* ptr = &*encoded_data.begin();

    *(chunk_pos_t*)ptr = px;
    ptr += sizeof chunk_pos_t;

    *(chunk_pos_t*)ptr = py;
    ptr += sizeof chunk_pos_t;

    for (texel_local_pos_t i = 0; i < sx * sy; i++) {
        *(fun::rgb_t*)ptr = *(data + i);

        ptr += sizeof fun::rgb_t;

        // fun::rgb_t color = *(data + i);

        // *(uint8_t*)ptr = color.r;
        // ptr += sizeof uint8_t;

        // *(uint8_t*)ptr = color.g;
        // ptr += sizeof uint8_t;

        // *(uint8_t*)ptr = color.b;
        // ptr += sizeof uint8_t;
    }

    *(uint8_t*)ptr = '\n';

    return encoded_data;
}

std::vector <uint8_t> space::chunk::decode(std::string data, chunk_size_t sx, chunk_size_t sy, chunk_pos_t* px, chunk_pos_t* py) {
    std::vector <uint8_t> texels;
    texels.resize(sx * sy);

    char* ptr = &*data.begin();

    *px = *(chunk_pos_t*)ptr;
    ptr += sizeof chunk_pos_t;

    *py = *(chunk_pos_t*)ptr;
    ptr += sizeof chunk_pos_t;

    for (texel_local_pos_t i = 0; i < sx * sy; i++) {
        texels[i] = *ptr;

        ptr += sizeof fun::rgb_t;
    }

    return texels;
}