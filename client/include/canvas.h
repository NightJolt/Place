#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

#include "../../common/include/space.h"

#include "chunk.h"

namespace space {
    class canvas_t : public sf::Drawable {
    private:
        typedef fun::unordered_map_vec2_t <chunk_int_t, chunk_t*> chunks_map_t;

    public:
        chunk_t* get_chunk(chunk_pos_t);
        chunks_map_t& get_chunks();
        
        bool has_chunk(chunk_pos_t);

        fun::rgb_t get_color(grid_pos_t);
        fun::rgb_t get_color(chunk_pos_t, texel_pos_t);

        void set_color(grid_pos_t, fun::rgb_t);
        void set_color(chunk_pos_t, texel_pos_t, fun::rgb_t);



        void clear();

    private:
        void draw(sf::RenderTarget&, sf::RenderStates) const override;

        chunks_map_t m_chunks;
    };
}