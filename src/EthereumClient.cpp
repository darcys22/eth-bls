// EthereumClient.cpp
#include <iostream>

#include "eth-bls/EthereumClient.hpp"
#include "eth-bls/utils.hpp"


EthereumClient::EthereumClient(const std::string& name, const std::string& _url) 
    : clientName(name), url(_url) {
    // Initialize client
}

EthereumClient::~EthereumClient() {
    // Cleanup client
}

void EthereumClient::connectToNetwork() {
    // Here we can verify connection by calling some simple JSON RPC method like `net_version`
    auto response = makeJsonRpcRequest("net_version", cpr::Body("{}"));
    if (response.status_code == 200) {
        std::cout << "Connected to the Ethereum network.\n";
    } else {
        std::cout << "Failed to connect to the Ethereum network.\n";
    }
}

void EthereumClient::disconnectFromNetwork() {
    // Code to disconnect from Ethereum network
    std::cout << "Disconnected from the Ethereum network.\n";
}

cpr::Response EthereumClient::makeJsonRpcRequest(const std::string& method, const nlohmann::json& params) {
    nlohmann::json bodyJson;
    bodyJson["jsonrpc"] = "2.0";
    bodyJson["method"] = method;
    bodyJson["params"] = params;
    bodyJson["id"] = 1;

    cpr::Body body(bodyJson.dump());

    session.SetUrl(url);
    session.SetBody(body);
    session.SetHeader({{"Content-Type", "application/json"}});

    return session.Post();
}

// Sending transaction with SenderTransactOpts
cpr::Response EthereumClient::sendTransaction(const SenderTransactOpts& opts, const std::string& to, uint64_t value, const std::string& data) {
    // Prepare the transaction parameters
    nlohmann::json params = nlohmann::json::array();
    nlohmann::json transaction;
    transaction["from"] = opts.fromAddress;
    transaction["to"] = to;
    transaction["value"] = "0x" + std::to_string(value);  // Ethereum requires values to be passed as hex
    if (!data.empty()) {
        transaction["data"] = data;
    }
    params.push_back(transaction);

    // Send the transaction
    return makeJsonRpcRequest("eth_sendTransaction", params);
}

// Sending signed transaction
cpr::Response EthereumClient::sendTransaction(const Transaction& signedTx) {
    // Create and send a raw transaction
    nlohmann::json params = nlohmann::json::array();
    params.push_back(signedTx.raw());  // Assuming Transaction::raw() returns the raw, signed transaction as hex
    return makeJsonRpcRequest("eth_sendRawTransaction", params);
}

uint64_t EthereumClient::getBalance(const std::string& address) {
    nlohmann::json params = nlohmann::json::array();
    params.push_back(address);
    params.push_back("latest");

    auto response = makeJsonRpcRequest("eth_getBalance", params);
if (response.status_code == 200) {
        nlohmann::json responseJson = nlohmann::json::parse(response.text);

        if (responseJson.find("error") != responseJson.end())
            throw std::runtime_error("Error getting balance: " + responseJson["error"]["message"].get<std::string>());

        std::string balanceHex = responseJson["result"].get<std::string>();

        // Convert balance from hex to decimal
        uint64_t balance = std::stoull(balanceHex, nullptr, 16);

        return balance;
    } else {
        throw std::runtime_error("Failed to get balance for address " + address);
    }
}

FeeData EthereumClient::getFeeData() {
    // Get latest block
    nlohmann::json params = nlohmann::json::array();
    auto blockResponse = makeJsonRpcRequest("eth_getBlockByNumber", params);
    if (blockResponse.status_code != 200) {
        throw std::runtime_error("Failed to get the latest block");
    }
    nlohmann::json blockJson = nlohmann::json::parse(blockResponse.text);
    uint64_t baseFeePerGas = std::stoull(blockJson["result"]["baseFeePerGas"].get<std::string>(), nullptr, 16);
    
    // Get gas price
    params = nlohmann::json::array();
    auto gasPriceResponse = makeJsonRpcRequest("eth_gasPrice", params);
    if (gasPriceResponse.status_code != 200) {
        throw std::runtime_error("Failed to get gas price");
    }
    nlohmann::json gasPriceJson = nlohmann::json::parse(gasPriceResponse.text);
    uint64_t gasPrice = std::stoull(gasPriceJson["result"].get<std::string>(), nullptr, 16);

    // Compute maxFeePerGas and maxPriorityFeePerGas based on baseFeePerGas
    uint64_t maxPriorityFeePerGas = 1000000000;
    uint64_t maxFeePerGas = (baseFeePerGas * 2) + maxPriorityFeePerGas;

    return FeeData(gasPrice, maxFeePerGas, maxPriorityFeePerGas);
}
