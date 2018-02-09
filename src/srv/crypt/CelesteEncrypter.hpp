#ifndef CELESTE_ENCRYPTER_HPP
#define CELESTE_ENCRYPTER_HPP

#include <string>

namespace celeste
{
    struct CelesteEncrypter
    {
        void encrypt(const std::string& src, std::string& encrypted) const;
    };
}

#endif