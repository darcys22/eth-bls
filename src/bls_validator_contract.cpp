#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/utils.hpp"

BLSValidatorsContract::BLSValidatorsContract(const std::string& _contractAddress, std::shared_ptr<Provider> _provider)
        : contractAddress(_contractAddress), provider(_provider) {}

Transaction BLSValidatorsContract::addValidator(const std::string& publicKey) {
    const uint64_t amount = 15000;
    Transaction tx(contractAddress, 0, 150000);

    // Get the function selector
    std::string functionSelector = utils::getFunctionSignature("addValidator(uint256,uint256,uint256)");

    // Convert amount to hex string and pad it to 32 bytes
    std::string amount_padded = utils::padTo32Bytes(utils::decimalToHex(amount), utils::PaddingDirection::LEFT);

    // Concatenate the function selector and the encoded arguments
    tx.data = functionSelector + publicKey + amount_padded;

    return tx;
}
