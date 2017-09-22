
#pragma once

#include "engine/db/metadata.hpp"

namespace eng {

  // prefix Np is for Niepce Property

#define DEFINE_PROPERTY(a,b,c,d,e)               \
    a = b,

enum NiepceProperties {

    #include "engine/db/properties-def.hpp"

    _NpLastProp
};

#undef DEFINE_PROPERTY

}
