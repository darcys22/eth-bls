#include "eth-bls/service_node_list.hpp"
#include "eth-bls/utils.hpp"
#include "eth-bls/ec_utils.hpp"

ServiceNode::ServiceNode() {
    // This init function generates a secret key calling blsSecretKeySetByCSPRNG
    secretKey.init();
}

ServiceNode::~ServiceNode() {
}

bls::Signature ServiceNode::signHash(const std::array<unsigned char, 32>& hash) {
    bls::Signature sig;
    secretKey.signHash(sig, hash.data(), hash.size());
    return sig;
}


std::string ServiceNode::getPublicKeyHex() {
    bls::PublicKey publicKey;
    secretKey.getPublicKey(publicKey);
    return utils::PublicKeyToHex(publicKey);
}

bls::PublicKey ServiceNode::getPublicKey() {
    bls::PublicKey publicKey;
    secretKey.getPublicKey(publicKey);
    return publicKey;
}

ServiceNodeList::ServiceNodeList(size_t numNodes) {
    bls::init(mclBn_CurveSNARK1);
    nodes.reserve(numNodes);
    for(size_t i = 0; i < numNodes; ++i) {
        nodes.emplace_back(); // construct new ServiceNode in-place
    }
}

ServiceNodeList::~ServiceNodeList() {
}

std::string ServiceNodeList::aggregatePubkeyHex() {
    bls::PublicKey aggregate_pubkey; 
    aggregate_pubkey.clear();
    for(auto& node : nodes) {
        aggregate_pubkey.add(node.getPublicKey());
    }
    return utils::PublicKeyToHex(aggregate_pubkey);
}

std::string ServiceNodeList::aggregateSignatures(const std::string& message) {
    bls::SignatureVec sigs;
    bls::IdVec ids(nodes.size());
    size_t count = 0;

    const std::array<unsigned char, 32> hash = utils::hash(message); // Get the hash of the input

    for(auto& node : nodes) {
        sigs.push_back(node.signHash(hash));
        ids[count] = static_cast<unsigned int>(count + 1);
        count++;
    }

    bls::Signature aggSig;
    aggSig.recover(sigs, ids);
    return utils::SignatureToHex(aggSig);
}

