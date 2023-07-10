#include <string>
#include <vector>
#include <memory>

#include "transaction.hpp"
#include "provider.hpp"

class BLSValidatorsContract {
public:
    // Constructor
    BLSValidatorsContract(const std::string& contractAddress, std::shared_ptr<Provider> provider);

    // Method for creating a transaction to add a public key
    Transaction addValidator(const std::string& publicKey);

private:
    std::string contractAddress;
    std::shared_ptr<Provider> provider;

    // This function is not defined in this example
    std::vector<unsigned char> encodeFunctionCall(const std::string& functionName, const std::vector<unsigned char>& publicKey);
};
