#pragma once
#define MCLBN_FP_UNIT_SIZE 4

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
};

class ServiceNodeList {
public:
    std::vector<ServiceNode> nodes;

    ServiceNodeList(size_t numNodes);
    ~ServiceNodeList();
    std::string aggregateSignatures(const std::string& message);
};
