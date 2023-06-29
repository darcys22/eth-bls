#pragma once

#include <assert.h>
#include <secp256k1.h>
#include <vector>
#include <string>

class Signer {
private:
    secp256k1_context* ctx;

public:
    Signer();
    ~Signer();

    // Returns <Pubkey, Seckey>
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> generate_key_pair();

    std::vector<unsigned char> sign(const std::string& message, const std::vector<unsigned char>& seckey);
// END
};

