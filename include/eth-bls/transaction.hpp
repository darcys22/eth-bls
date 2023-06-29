#pragma once

#include <string>
#include <vector>

class Transaction {
public:
    uint64_t chainId;
    uint64_t nonce;                     
    uint64_t maxPriorityFeePerGas;
    uint64_t maxFeePerGas;
    std::string to;
    uint64_t value;     
    uint64_t gasLimit;
    uint64_t gasPrice;
    std::string data;
    uint64_t signatureYParity;
    std::vector<unsigned char> signatureR;
    std::vector<unsigned char> signatureS;

    // Constructor                                                                                                        
    // (nonce, toAddress, value, gasLimit, gasPrice, nil)
    Transaction(uint64_t _nonce , const std::string& _to , uint64_t _value , uint64_t _gasLimit = 21000, uint64_t _gasPrice = 1000000000, const std::string& _data = "") 
        : chainId(1), nonce(_nonce), maxPriorityFeePerGas(0), maxFeePerGas(0), to(_to), value(_value), gasLimit(_gasLimit), gasPrice(_gasPrice), data(_data) {}


    std::vector<unsigned char> raw() const;

};
