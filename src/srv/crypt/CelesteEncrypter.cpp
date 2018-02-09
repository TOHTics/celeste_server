#include "picosha2.h"
#include "CelesteEncrypter.hpp"

namespace celeste
{
    void
    CelesteEncrypter
    ::encrypt(const std::string& src, std::string& encrypted) const
    {
        picosha2::hash256_hex_string(src, encrypted);
    }
}