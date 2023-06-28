#ifndef SIGNER_H
#define SIGNER_H

#include <secp256k1.h>
#include <vector>

class Signer {
private:
    secp256k1_context* ctx;

public:
    Signer();
    ~Signer();
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> generate_key_pair();
    std::vector<unsigned char> sign(const std::vector<unsigned char>& msg_hash, const std::vector<unsigned char>& seckey);
};

#endif // SIGNER_H

