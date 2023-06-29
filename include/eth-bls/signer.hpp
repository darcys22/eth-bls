#pragma once

#include <assert.h>
#include <secp256k1.h>
#include <vector>
#include <string>
#include <memory>

#include "eth-bls/EthereumClient.hpp"

class Signer {
private:
    secp256k1_context* ctx;
    std::shared_ptr<EthereumClient> ethClient;

    uint64_t maxPriorityFeePerGas = 0;
    uint64_t maxFeePerGas = 0;
    uint64_t gasPrice = 0;

public:
    Signer();
    Signer(const std::shared_ptr<EthereumClient>& client);
    ~Signer();

    // Returns <Pubkey, Seckey>
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> generate_key_pair();

    std::vector<unsigned char> sign(const std::string& message, const std::vector<unsigned char>& seckey);

    // Client usage methods
    bool hasClient() const { return static_cast<bool>(ethClient); }
    std::shared_ptr<EthereumClient> getClient() { return ethClient; }

    void populateTransaction(Transaction& tx, const SenderTransactOpts& opts);


private:
    void initContext();

// END
};

