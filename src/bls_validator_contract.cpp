#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/utils.hpp"
#include "eth-bls/ec_utils.hpp"


#include <iostream>

BLSValidatorsContract::BLSValidatorsContract(const std::string& _contractAddress, std::shared_ptr<Provider> _provider)
        : contractAddress(_contractAddress), provider(_provider) {}

Transaction BLSValidatorsContract::addValidator(const std::string& publicKey) {
    const uint64_t amount = 15000;
    Transaction tx(contractAddress, 0, 150000);

    std::string functionSelector = utils::getFunctionSignature("addValidator(uint256,uint256,uint256)");

    // Convert amount to hex string and pad it to 32 bytes
    std::string amount_padded = utils::padTo32Bytes(utils::decimalToHex(amount), utils::PaddingDirection::LEFT);

    // Concatenate the function selector and the encoded arguments
    tx.data = functionSelector + publicKey + amount_padded;

    return tx;
}

Transaction BLSValidatorsContract::clear() {
    Transaction tx(contractAddress, 0, 10000000);
    tx.data = utils::getFunctionSignature("clearValidators()");
    return tx;
}

uint64_t BLSValidatorsContract::getValidatorsLength() {
    ReadCallData callData;
    callData.contractAddress = contractAddress;
    callData.data = utils::getFunctionSignature("getValidatorsLength()");
    std::string result = provider->callReadFunction(callData);
    return utils::fromHexStringToUint64(result);
}

Transaction BLSValidatorsContract::checkSigAGG(const std::string& sig, const std::string& message) {
    Transaction tx(contractAddress, 0, 30000000);
    std::string functionSelector = utils::getFunctionSignature("checkSigAGG(uint256,uint256,uint256,uint256,uint256)");
    std::string message_padded = utils::padTo32Bytes(utils::toHexString(utils::HashModulus(message)), utils::PaddingDirection::LEFT);
    tx.data = functionSelector + sig + message_padded;
    return tx;
}

Transaction BLSValidatorsContract::checkAggPubkey(const std::string& aggPubkey) {
    Transaction tx(contractAddress, 0, 800000);
    std::string functionSelector = utils::getFunctionSignature("checkAggPubkey(uint256,uint256)");
    tx.data = functionSelector + aggPubkey;
    return tx;
}
