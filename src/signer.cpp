#include "eth-bls/signer.hpp"

#include <stdio.h>
#include <array>
#include <stdexcept>

#include "eth-bls/ecdsa_util.h"
#include "eth-bls/utils.hpp"

#include <secp256k1_recovery.h>

Signer::Signer() {
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    unsigned char randomize[32];
    if (!fill_random(randomize, sizeof(randomize))) {
        throw std::runtime_error("Failed to generate randomness");
    }
    int return_val = secp256k1_context_randomize(ctx, randomize);
    assert(return_val);
}

Signer::~Signer() {
    secp256k1_context_destroy(ctx);
}

std::pair<std::vector<unsigned char>, std::vector<unsigned char>> Signer::generate_key_pair() {
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

std::vector<unsigned char> Signer::sign(const std::string& message, const std::vector<unsigned char>& seckey) {
    secp256k1_ecdsa_recoverable_signature sig;
    unsigned char serialized_signature[64];
    int recid;
    int return_val;

    // Hash the message
    std::array<unsigned char, 32> msg_hash = utils::hash(message);

    return_val = secp256k1_ecdsa_sign_recoverable(ctx, &sig, msg_hash.data(), seckey.data(), NULL, NULL);
    assert(return_val);

    return_val = secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, serialized_signature, &recid, &sig);
    assert(return_val);

    // Create a vector and fill it with the serialized signature
    std::vector<unsigned char> signature(serialized_signature, serialized_signature + sizeof(serialized_signature));

    // Append the recid +27 to the end of the vector, this appears to be an ethereum thing. Pulled possibly from bitcoin originally
    // https://github.com/ethereum/EIPs/blob/master/EIPS/eip-155.md
    // TODO sean it looks like the EIP modifys how this is done in new ones from that EIP
    //
    // If block.number >= FORK_BLKNUM and v = CHAIN_ID * 2 + 35 or v = CHAIN_ID * 2 + 36,
    // then when computing the hash of a transaction for purposes of recovering,
    // instead of hashing six rlp encoded elements (nonce, gasprice, startgas, to, value, data),
    // hash nine rlp encoded elements (nonce, gasprice, startgas, to, value, data, chainid, 0, 0).
    // The currently existing signature scheme using v = 27 and v = 28 remains valid and continues to operate under the same rules as it did previously.
    signature.push_back(static_cast<unsigned char>(recid + 27));

    return signature;
}
