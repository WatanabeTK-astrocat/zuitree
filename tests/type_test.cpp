#include "type.hpp"
#include <cassert>

static_assert(sizeof(double3) == 3 * sizeof(double), "double3 should be three doubles");

int main() {
    double3 v{1.0, 2.0, 3.0};
    assert(v.x == 1.0);
    assert(v.y == 2.0);
    assert(v.z == 3.0);
    return 0;
}
