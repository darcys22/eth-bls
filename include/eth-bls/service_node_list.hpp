#pragma once
#define BLS_ETH
#define MCLBN_FP_UNIT_SIZE 4
#define MCLBN_FR_UNIT_SIZE 4
#include <bls/bls.h>
#include <bls/bls.hpp>
#include <string>
#include <vector>

class ServiceNode {
private:
    bls::SecretKey secretKey;
public:
    ServiceNode();
    ~ServiceNode();
    bls::Signature sign(const std::string& message);
    std::string getPublicKeyHex();
    bls::PublicKey getPublicKey();
};

class ServiceNodeList {
public:
    std::vector<ServiceNode> nodes;

    static std::string PublicKeyToHex(bls::PublicKey publicKey);

    ServiceNodeList(size_t numNodes);
    ~ServiceNodeList();

    std::string aggregatePubkeyHex();
    std::string aggregateSignatures(const std::string& message);
};
