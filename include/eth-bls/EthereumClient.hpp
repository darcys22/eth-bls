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
    uint64_t nonce;
    std::string to;
    uint64_t value;
    uint64_t gasLimit;
    uint64_t gasPrice;
    std::string data;

    // Constructor
    Transaction(uint64_t _nonce, const std::string& _to, uint64_t _value, uint64_t _gasLimit, uint64_t _gasPrice, const std::string& _data = "")
        : nonce(_nonce), to(_to), value(_value), gasLimit(_gasLimit), gasPrice(_gasPrice), data(_data) {}

    std::string raw() const;
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

