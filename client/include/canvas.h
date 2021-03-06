#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

#include "../../common/include/space.h"

#include "chunk.h"

namespace space {
    class canvas_t : public sf::Drawable {
    public:

        chunk_t* get_chunk(fun::vec2_t <space::chunk_pos_t>);

        fun::rgb_t get_color(fun::vec2_t <space::grid_pos_t>);
        void set_color(fun::vec2_t <space::grid_pos_t>, fun::rgb_t);

        void clear();

    private:

        void draw(sf::RenderTarget&, sf::RenderStates) const override;

        std::unordered_map <
            fun::vec2_t <space::chunk_pos_t>,
            chunk_t*,
            decltype([](const fun::vec2_t <space::chunk_pos_t>& v) -> size_t const { return fun::hash(v); }),
            decltype([](const fun::vec2_t <space::chunk_pos_t>& a, const fun::vec2_t <space::chunk_pos_t>& b) -> bool const { return a == b; })
        > chunks;
    };
}