#pragma once

#include <vector>
#include <cstddef>
#include <string>
#include <cassert>

using ui32 = unsigned int;
using ui64 = unsigned long long int;

// Почти с семинара
class StringHashes {
    std::vector<ui64> baseDegrees, prefixHashes;
    ui32 base, modulo;
public:
    StringHashes(ui32 base, ui32 modulo, const std::string& s)
            : base(base), modulo(modulo), prefixHashes(s.length(), 0), baseDegrees(s.length(), 0) {
        ui64 hash = 0;
        ui64 baseDegree = 1;
        for (size_t i = 0; i < s.length(); ++i) {
            hash = (hash * base + s[i])/* % modulo*/;
            prefixHashes[i] = hash;

            baseDegrees[i] = baseDegree;
            baseDegree = (baseDegree * base) /*% modulo*/;
        }
    }

    size_t size() const {
        return prefixHashes.size();
    }

    ui64 calcHash(size_t l, size_t r) const {
        assert(l <= r && r < size());
        ui64 ansr = prefixHashes[r];
        if (l == 0) {
            return ansr;
        }
        ui64 ansl = (prefixHashes[l - 1] * 1ll * baseDegrees[r - l + 1]) /*% modulo*/;
        /*if (ansr < ansl)
            ansr += modulo;*/
        return ansr - ansl;
    }
};
