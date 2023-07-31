#pragma once

#define BLS_ETH
#define MCLBN_FP_UNIT_SIZE 4
#define MCLBN_FR_UNIT_SIZE 4

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <bls/bls.hpp>
#include <mcl/bn.hpp>
#undef MCLBN_NO_AUTOLINK
#pragma GCC diagnostic pop

#include <string>
#include <vector>

class ServiceNode {
private:
    bls::SecretKey secretKey;
public:
    ServiceNode();
    ~ServiceNode();
    bls::Signature signHash(const std::array<unsigned char, 32>& hash);
    std::string getPublicKeyHex();
    bls::PublicKey getPublicKey();
};

class ServiceNodeList {
public:
    std::vector<ServiceNode> nodes;

    ServiceNodeList(size_t numNodes);
    ~ServiceNodeList();

    std::string aggregatePubkeyHex();
    std::string aggregateSignatures(const std::string& message);
};
