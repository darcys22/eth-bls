#include <mcl/bn.hpp>

#include "eth-bls/service_node_list.hpp"

ServiceNode::ServiceNode() {
    // This init function generates a secret key calling blsSecretKeySetByCSPRNG
    secretKey.init();
}

ServiceNode::~ServiceNode() {
}

bls::Signature ServiceNode::sign(const std::string& message) {
    bls::Signature sig;
    secretKey.sign(sig, message);
    return sig;
}

std::string ServiceNode::getPublicKeyHex() {
    bls::PublicKey publicKey;
    secretKey.getPublicKey(publicKey);
    return publicKey.serializeToHexStr();
}

ServiceNodeList::ServiceNodeList(size_t numNodes) {
    bls::init();
    nodes.reserve(numNodes);
    for(size_t i = 0; i < numNodes; ++i) {
        nodes.emplace_back(); // construct new ServiceNode in-place
    }
}

ServiceNodeList::~ServiceNodeList() {
}

std::string ServiceNodeList::aggregateSignatures(const std::string& message) {
    bls::SignatureVec sigs;
	bls::IdVec ids(nodes.size());
    size_t count = 0;
    for(auto& node : nodes) {
        sigs.push_back(node.sign(message));
        ids[count] = count + 1;
        count++;
    }

    bls::Signature aggSig;
    aggSig.recover(sigs, ids);
    return aggSig.serializeToHexStr();
}

