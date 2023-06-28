#ifndef TMP_TMP_H_
#define TMP_TMP_H_

#include <cpr/cpr.h>

namespace tmp
{
    int add(int, int);

    int64_t request();

    int ecdsa();

    std::string toHexString(const std::array<unsigned char, 32>& bytes);

    std::array<unsigned char, 32> hash(const std::string& in);

// END
}

#endif  // TMP_TMP_H_
