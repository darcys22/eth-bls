#include <mcl/bn.hpp>

#include "eth-bls/service_node_list.hpp"
#include "eth-bls/utils.hpp"

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
    return ServiceNodeList::PublicKeyToHex(publicKey);
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

std::string ServiceNodeList::PublicKeyToHex(bls::PublicKey publicKey) {
    mclSize serializedPublicKeySize = 32;
    std::vector<unsigned char> serialized_pubkey(serializedPublicKeySize*2);
    uint8_t *dst = serialized_pubkey.data();
    const blsPublicKey* pub = publicKey.getPtr();  
    const mcl::bn::G1* g1Point = reinterpret_cast<const mcl::bn::G1*>(&pub->v);
    mcl::bn::G1 g1Point2 = *g1Point;
    g1Point2.normalize();
    if (g1Point2.x.serialize(dst, serializedPublicKeySize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x is zero");
    if (g1Point2.y.serialize(dst + serializedPublicKeySize, serializedPublicKeySize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y is zero");

    return utils::toHexString(serialized_pubkey);
}

std::string ServiceNodeList::aggregatePubkeyHex() {
    bls::PublicKey aggregate_pubkey; 
    aggregate_pubkey.clear();
    for(auto& node : nodes) {
        aggregate_pubkey.add(node.getPublicKey());
    }
    return PublicKeyToHex(aggregate_pubkey);
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

