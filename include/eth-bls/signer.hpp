#pragma once

#include <assert.h>
#include <secp256k1.h>
#include <vector>
#include <string>
#include <memory>

#include "eth-bls/provider.hpp"

class Signer {
private:
    secp256k1_context* ctx;
    std::shared_ptr<Provider> provider;

    uint64_t maxPriorityFeePerGas = 0;
    uint64_t maxFeePerGas = 0;
    uint64_t gasPrice = 0;

public:
    Signer();
    Signer(const std::shared_ptr<Provider>& client);
    ~Signer();

    // Returns <Pubkey, Seckey>
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> generate_key_pair();

    std::vector<unsigned char> sign(const std::array<unsigned char, 32>& hash, const std::vector<unsigned char>& seckey);
    std::vector<unsigned char> sign(const std::string& hash, const std::vector<unsigned char>& seckey);

    // Client usage methods
    bool hasProvider() const { return static_cast<bool>(provider); }
    std::shared_ptr<Provider> getProvider() { return provider; }

    void populateTransaction(Transaction& tx, const SenderTransactOpts& opts);

    std::vector<unsigned char> signMessage(const std::string& message, const std::vector<unsigned char>& seckey);
    std::string signTransaction(Transaction& tx, const std::vector<unsigned char>& seckey);


private:
    void initContext();

// END
};

