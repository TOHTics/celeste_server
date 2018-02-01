#include "picosha2.h"
#include "CelesteEncrypter.hpp"

namespace celeste
{
    void
    CelesteEncrypter
    ::encrypt(const std::string& src, std::string& encrypted)
    {
        picosha2::hash256_hex_string(src, encrypted);
    }
}