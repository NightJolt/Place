#include "chunk.h"

std::string space::chunk::encode(chunk_pos_t px, chunk_pos_t py, chunk_size_t sx, chunk_size_t sy, fun::rgb_t* data) {
    size_t mem_size = sizeof chunk_pos_t * 2 + sx * sy * sizeof fun::rgb_t + 1;

    char* ptr = (char*)malloc(mem_size);
    uint32_t pos = 0;

    *(chunk_pos_t*)ptr = px;
    *(chunk_pos_t*)(ptr + sizeof chunk_pos_t) = py;

    pos += sizeof chunk_pos_t * 2;

    for (texel_local_pos_t x = 0; x < sx; x++) {
        for (texel_local_pos_t y = 0; y < sy; y++) {
            fun::rgb_t color = *(data + x * sx + y);

            *(uint8_t*)(ptr + pos) = color.r;
            pos += sizeof uint8_t;

            *(uint8_t*)(ptr + pos) = color.g;
            pos += sizeof uint8_t;

            *(uint8_t*)(ptr + pos) = color.b;
            pos += sizeof uint8_t;
        }
    }

    *(uint8_t*)(ptr + pos) = '\n';

    std::string encoded_data(ptr);

    free(ptr);

    return encoded_data;
}