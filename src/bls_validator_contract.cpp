#include "eth-bls/bls_validator_contract.hpp"

BLSValidatorsContract::BLSValidatorsContract(const std::string& contractAddress, std::shared_ptr<Provider> provider)
        : contractAddress(contractAddress), provider(provider) {}

Transaction BLSValidatorsContract::addValidator(const std::string& publicKey) {
    Transaction tx(contractAddress, 0, 21000);
    //tx.data = encodeFunctionCall("addPublicKey", publicKey);
    return tx;
}
