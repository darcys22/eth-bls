// EthereumClient.hpp
#ifndef ETHEREUM_CLIENT_H
#define ETHEREUM_CLIENT_H

#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

class SenderTransactOpts {
public:
    std::string privateKey;
    std::string fromAddress;
    uint64_t chainID;
    uint64_t gasPrice;
    // Add any other transaction options you need

    // Constructor
    SenderTransactOpts(const std::string& _privateKey, const std::string& _fromAddress, uint64_t _chainID, uint64_t _gasPrice)
        : privateKey(_privateKey), fromAddress(_fromAddress), chainID(_chainID), gasPrice(_gasPrice) {}
};

class Transaction {
public:
    uint64_t chainId;
    uint64_t nonce;                     
    uint64_t maxPriorityFeePerGas;
    uint64_t maxFeePerGas;
    uint64_t gasLimit;                                                           
    std::string to;
    uint64_t value;     
    std::string data;
    uint64_t signatureYParity;
    std::vector<unsigned char> signatureR;
    std::vector<unsigned char> signatureS;

    // Constructor
    Transaction(uint64_t _chainId, uint64_t _nonce, uint64_t _maxPriorityFeePerGas, uint64_t _maxFeePerGas, uint64_t _gasLimit, const std::string& _to, uint64_t _value, const std::string& _data = "", uint64_t _signatureYParity = 0, const std::vector<unsigned char>& _signatureR = {}, const std::vector<unsigned char>& _signatureS = {})
        : chainId(_chainId), nonce(_nonce), maxPriorityFeePerGas(_maxPriorityFeePerGas), maxFeePerGas(_maxFeePerGas), gasLimit(_gasLimit), to(_to), value(_value), data(_data), signatureYParity(_signatureYParity), signatureR(_signatureR), signatureS(_signatureS) {}


    std::vector<unsigned char> raw() const;

};

class EthereumClient {
    std::string clientName;
    cpr::Url url;
    cpr::Session session;
public:
    EthereumClient(const std::string& name, const std::string& _url);
    ~EthereumClient();

    void connectToNetwork();
    void disconnectFromNetwork();
    void sendTransaction(const std::string& to, int amount);
    cpr::Response sendTransaction(const Transaction& signedTx);
    cpr::Response sendTransaction(const SenderTransactOpts& opts, const std::string& to, uint64_t value, const std::string& data = "");
    uint64_t getBalance(const std::string& address);

private:
    cpr::Response makeJsonRpcRequest(const std::string& method, const nlohmann::json& params);
};

#endif

