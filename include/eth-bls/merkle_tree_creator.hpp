#pragma once

#include <array>
#include <vector>
#include <string>
#include "merklecpp.h"

#include "transaction.hpp"
#include "provider.hpp"

class MerkleTreeCreator {
public:
    MerkleTreeCreator(const std::string& _contractAddress, std::shared_ptr<Provider> _provider);

    void addLeaf(const std::string& input);
    void addLeaves(const std::map<std::string, uint64_t>& data);

    merkle::Tree::Hash createMerkleKeccakHash(const std::string& input);

    std::string getRoot();
    std::string getPath(size_t index);
    size_t getPathSize(size_t index);
    size_t findIndex(const std::string& input);

    Transaction updateRewardsMerkleRoot();
    Transaction validateProof(size_t index, int64_t amount);

    std::string abiEncode(const std::string& address, uint64_t balance);

    static inline void cncryptoCompressKeccak256(
        const merkle::HashT<32>& l,
        const merkle::HashT<32>& r,
        merkle::HashT<32>& out
    );

    std::string contractAddress;
    std::shared_ptr<Provider> provider;

    merkle::TreeT<32, cncryptoCompressKeccak256> tree;
};

