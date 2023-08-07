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
    mclBn_setMapToMode(MCL_MAP_TO_MODE_TRY_AND_INC);
    mcl::bn::G1 gen;
    bool b;
    mcl::bn::mapToG1(&b, gen, 1);
    blsPublicKey publicKey;
    publicKey.v = *reinterpret_cast<const mclBnG1*>(&gen); // Cast gen to mclBnG1 and assign it to publicKey.v

    blsSetGeneratorOfPublicKey(&publicKey);
    nodes.reserve(numNodes);
    for(size_t i = 0; i < numNodes; ++i) {
        nodes.emplace_back(); // construct new ServiceNode in-place
    }
}

ServiceNodeList::~ServiceNodeList() {
}

void ServiceNodeList::addNode() {
    nodes.emplace_back(); // construct new ServiceNode in-place
}
std::string ServiceNodeList::getLatestNodePubkey() {
    return nodes.back().getPublicKeyHex();
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
    const std::array<unsigned char, 32> hash = utils::hash(message); // Get the hash of the input
    bls::Signature aggSig;
    aggSig.clear();
    for(auto& node : nodes) {
        aggSig.add(node.signHash(hash));
    }
    return utils::SignatureToHex(aggSig);
}

