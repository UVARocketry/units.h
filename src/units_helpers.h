#ifndef UNITS_HELPERS_H
#define UNITS_HELPERS_H

#include "units_baseTypeTags.h"
#include "units_commonTypes.h"
#include "units_quantity.h"
template <typename T2, typename T1>
float unit_value_from(float v) {
    return Quantity<T1>(v).template as<T2>().value;
}

inline Quantity<AGL<Meter>> agl_from_asl(Length v, Length baseAltitude) {
    return Quantity<AGL<Meter>>(v.value - baseAltitude.value);
}
inline Quantity<Meter> asl_from_agl(Quantity<AGL<Meter>> v,
                                    Length baseAltitude) {
    return Quantity<Meter>(v.value + baseAltitude.value);
}
#endif
