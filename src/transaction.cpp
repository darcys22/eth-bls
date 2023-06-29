#include "eth-bls/transaction.hpp"
#include "eth-bls/utils.hpp"

/**
* Returns the raw Bytes of the EIP-1559 transaction, in order.
*
* Format: `[chainId, nonce, maxPriorityFeePerGas, maxFeePerGas, gasLimit, to, value, data,
* accessList, signatureYParity, signatureR, signatureS]`
*
* For an unsigned tx this method uses the empty Bytes values for the
* signature parameters `v`, `r` and `s` for encoding.
*/
std::vector<unsigned char> Transaction::raw() const {
    std::vector<unsigned char> rawBytes;
    
    auto appendBytes = [&rawBytes](const std::vector<unsigned char>& bytes) {
        rawBytes.insert(rawBytes.end(), bytes.begin(), bytes.end());
    };

    auto appendString = [&rawBytes](const std::string& str) {
        rawBytes.insert(rawBytes.end(), str.begin(), str.end());
    };
    
    appendBytes(utils::intToBytes(chainId));
    appendBytes(utils::intToBytes(nonce));
    appendBytes(utils::intToBytes(maxPriorityFeePerGas));
    appendBytes(utils::intToBytes(maxFeePerGas));
    appendBytes(utils::intToBytes(gasLimit));
    appendString(to);
    appendBytes(utils::intToBytes(value));
    appendString(data);
    // AccessList -> not going to use but is this right way of handling?
    appendBytes(std::vector<unsigned char>());
    appendBytes(signatureYParity != 0 ? utils::intToBytes(signatureYParity) : std::vector<unsigned char>());
    appendBytes(signatureR.empty() ? std::vector<unsigned char>() : signatureR);
    appendBytes(signatureS.empty() ? std::vector<unsigned char>() : signatureS);

    return rawBytes;
}

