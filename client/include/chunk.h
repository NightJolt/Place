#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/color.h"

#include "../../common/include/space.h"

namespace space {
    class chunk_t {
    public:

        chunk_t(fun::vec2_t <space::chunk_pos_t>);

        void set_data(const std::vector <fun::rgb_t>&);
        const std::vector <sf::Vertex>& get_vertices();

        fun::rgb_t get_color(fun::vec2_t <space::texel_pos_t>);
        void set_color(fun::vec2_t <space::texel_pos_t>, fun::rgb_t);

    private:
    
        std::vector <sf::Vertex> vertices;
    };
}