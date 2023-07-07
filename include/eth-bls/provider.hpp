// Provider.hpp
#pragma once

#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "transaction.hpp"

class SenderTransactOpts {
public:
    std::string privateKey;
    std::string fromAddress;
    uint64_t chainID;
    uint64_t gasPrice;

    // Constructor
    SenderTransactOpts(const std::string& _privateKey, const std::string& _fromAddress, uint64_t _chainID, uint64_t _gasPrice)
        : privateKey(_privateKey), fromAddress(_fromAddress), chainID(_chainID), gasPrice(_gasPrice) {}
};

struct FeeData {
    uint64_t gasPrice;
    uint64_t maxFeePerGas;
    uint64_t maxPriorityFeePerGas;

    FeeData(uint64_t _gasPrice, uint64_t _maxFeePerGas, uint64_t _maxPriorityFeePerGas)
        : gasPrice(_gasPrice), maxFeePerGas(_maxFeePerGas), maxPriorityFeePerGas(_maxPriorityFeePerGas) {}
};

class Provider {
    std::string clientName;
    cpr::Url url;
    cpr::Session session;
public:
    Provider(const std::string& name, const std::string& _url);
    ~Provider();

    void connectToNetwork();
    void disconnectFromNetwork();
    void sendTransaction(const std::string& to, int amount);
    cpr::Response sendTransaction(const Transaction& signedTx);
    cpr::Response sendTransaction(const SenderTransactOpts& opts, const std::string& to, uint64_t value, const std::string& data = "");
    uint64_t getBalance(const std::string& address);

    FeeData getFeeData();

private:
    cpr::Response makeJsonRpcRequest(const std::string& method, const nlohmann::json& params);
};

