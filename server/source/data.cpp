#include "data.h"

space::statistics_t::statistics_t() : texels_placed(0) {}

space::client_data_t::client_data_t() : texels_placed(0) {
    static const fun::str_t prefixes[] = { "sussy", "horny", "chad", "sexy" };
    static const fun::str_t names[] = { "amogus", "sugoma", "impostus", "sherk", "moth", "cheems", "ogre" };

    fun::str_t rand_prefix = prefixes[fun::math::random_32(0, sizeof(prefixes) / sizeof(fun::str_t) - 1)];
    fun::str_t rand_name = names[fun::math::random_32(0, sizeof(names) / sizeof(fun::str_t) - 1)];

    name = rand_prefix + "_" + rand_name + "_" + std::to_string(fun::math::random_32(0, 999));
}
