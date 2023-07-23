#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/utils.hpp"

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

    // Make the request and get the result
    std::string result = provider->callReadFunction(callData);

    return utils::fromHexStringToUint64(result);
}

//function checkSigAGG(uint256 sigs0, uint256 sigs1, uint256 sigs2, uint256 sigs3, uint256 message) public {
Transaction BLSValidatorsContract::checkSigAGG(const std::string& sig0, const std::string& sig1, const std::string& sig2, const std::string& sig3, const std::string& message) {
    Transaction tx(contractAddress, 0, 800000);

    std::string functionSelector = utils::getFunctionSignature("checkSigAgg(uint256,uint256,uint256,uint256,uint256)");

    std::string message_padded = utils::padTo32Bytes(utils::toHexString(message), utils::PaddingDirection::LEFT);

    tx.data = functionSelector + sig0 + sig1 + sig2 + sig3 + message_padded;

    return tx;
}

Transaction BLSValidatorsContract::checkAggPubkey(const std::string& aggPubkey) {
    Transaction tx(contractAddress, 0, 800000);
    std::string functionSelector = utils::getFunctionSignature("checkAggPubkey(uint256,uint256)");
    tx.data = functionSelector + aggPubkey;
    return tx;
}
