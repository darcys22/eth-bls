#include <secp256k1.h>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <stdexcept>

#include "eth-bls/ecdsa_util.h"

class Signer {
    secp256k1_context* ctx;

public:
    Signer() {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
        unsigned char randomize[32];
        if (!fill_random(randomize, sizeof(randomize))) {
            throw std::runtime_error("Failed to generate randomness");
        }
        int return_val = secp256k1_context_randomize(ctx, randomize);
        assert(return_val);
    }

    ~Signer() {
        secp256k1_context_destroy(ctx);
    }

    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> generate_key_pair() {
        unsigned char seckey[32];
        unsigned char compressed_pubkey[33];
        size_t len;
        int return_val;
        secp256k1_pubkey pubkey;

        while (1) {
            if (!fill_random(seckey, sizeof(seckey))) {
                throw std::runtime_error("Failed to generate randomness");
            }
            if (secp256k1_ec_seckey_verify(ctx, seckey)) {
                break;
            }
        }

        return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, seckey);
        assert(return_val);

        len = sizeof(compressed_pubkey);
        return_val = secp256k1_ec_pubkey_serialize(ctx, compressed_pubkey, &len, &pubkey, SECP256K1_EC_COMPRESSED);
        assert(return_val);
        assert(len == sizeof(compressed_pubkey));

        return {std::vector<unsigned char>(seckey, seckey + sizeof(seckey)), 
                std::vector<unsigned char>(compressed_pubkey, compressed_pubkey + sizeof(compressed_pubkey))};
    }

    std::vector<unsigned char> sign(const std::vector<unsigned char>& msg_hash, const std::vector<unsigned char>& seckey) {
        secp256k1_ecdsa_signature sig;
        unsigned char serialized_signature[64];
        int return_val;

        return_val = secp256k1_ecdsa_sign(ctx, &sig, msg_hash.data(), seckey.data(), NULL, NULL);
        assert(return_val);

        return_val = secp256k1_ecdsa_signature_serialize_compact(ctx, serialized_signature, &sig);
        assert(return_val);

        return std::vector<unsigned char>(serialized_signature, serialized_signature + sizeof(serialized_signature));
    }
};

