#pragma once
#include <string>
#include <vector>

class ServiceNode {
private:
    std::string secretKey;
public:
    ServiceNode();
    ~ServiceNode();
    std::string sign(const std::string& message);
    std::string getPublicKeyHex();
    std::string getPublicKey();
};

class ServiceNodeList {
public:
    std::vector<ServiceNode> nodes;

    ServiceNodeList(size_t numNodes);
    ~ServiceNodeList();
    std::string aggregateSignatures(const std::string& message);
};
