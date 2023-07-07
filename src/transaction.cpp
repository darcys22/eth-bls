#include "eth-bls/transaction.hpp"
#include "eth-bls/utils.hpp"

#include <rlpvalue.h>
#include <iostream>

/**
* Returns the raw Bytes of the EIP-1559 transaction, in order.
*
* Format: `[chainId, nonce, maxPriorityFeePerGas, maxFeePerGas, gasLimit, to, value, data,
* accessList, signatureYParity, signatureR, signatureS]`
*
* For an unsigned tx this method uses the empty Bytes values for the
* signature parameters `v`, `r` and `s` for encoding.
*/
//std::vector<unsigned char> Transaction::raw() const {
    //std::vector<unsigned char> rawBytes;
    
    //auto appendBytes = [&rawBytes](const std::vector<unsigned char>& bytes) {
        //rawBytes.insert(rawBytes.end(), bytes.begin(), bytes.end());
    //};

    //auto appendString = [&rawBytes](const std::string& str) {
        //rawBytes.insert(rawBytes.end(), str.begin(), str.end());
    //};
    
    //appendBytes(utils::intToBytes(chainId));
    //appendBytes(utils::intToBytes(nonce));
    //appendBytes(utils::intToBytes(maxPriorityFeePerGas));
    //appendBytes(utils::intToBytes(maxFeePerGas));
    //appendBytes(utils::intToBytes(gasLimit));
    //appendString(to);
    //appendBytes(utils::intToBytes(value));
    //appendString(data);
    // AccessList -> not going to use but would be here
    //appendBytes(signatureYParity != 0 ? utils::intToBytes(signatureYParity) : std::vector<unsigned char>());
    //appendBytes(signatureR.empty() ? std::vector<unsigned char>() : signatureR);
    //appendBytes(signatureS.empty() ? std::vector<unsigned char>() : signatureS);

    //return rawBytes;
//}

std::string Transaction::serialized() const {
    RLPValue arr(RLPValue::VARR);
    arr.setArray();
    RLPValue temp_val;
    temp_val.clear();
    temp_val.assign(utils::intToBytes(chainId));
    arr.push_back(temp_val);
    temp_val.assign(utils::intToBytes(nonce));
    arr.push_back(temp_val);
    temp_val.assign(utils::intToBytes(maxPriorityFeePerGas));
    arr.push_back(temp_val);
    temp_val.assign(utils::intToBytes(maxFeePerGas));
    arr.push_back(temp_val);
    temp_val.assign(utils::intToBytes(gasLimit));
    arr.push_back(temp_val);
    temp_val.assign(utils::fromHexString(to));
    arr.push_back(temp_val);
    temp_val.assign(utils::intToBytes(value));
    arr.push_back(temp_val);
    temp_val.assign(utils::fromHexString(data));
    arr.push_back(temp_val);

    // Access list not going to use
    RLPValue access_list(RLPValue::VARR);
    access_list.setArray();
    arr.push_back(access_list);

    if (transaction_signed) {
        temp_val.assign(utils::intToBytes(signatureYParity));
        arr.push_back(temp_val);
        temp_val.assign(signatureR);
        arr.push_back(temp_val);
        temp_val.assign(signatureS);
        arr.push_back(temp_val);
    }
    return "0x02" + utils::toHexString(arr.write());
}

std::string Transaction::hash() const {
    return "0x" + utils::toHexString(utils::hash(serialized()));
}
