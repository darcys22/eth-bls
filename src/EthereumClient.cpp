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


/**
* Returns a Uint8Array Array of the raw Bytes of the EIP-1559 transaction, in order.
*
* Format: `[chainId, nonce, maxPriorityFeePerGas, maxFeePerGas, gasLimit, to, value, data,
* accessList, signatureYParity, signatureR, signatureS]`
*
* Use {@link FeeMarketEIP1559Transaction.serialize} to add a transaction to a block
* with {@link Block.fromValuesArray}.
*
* For an unsigned tx this method uses the empty Bytes values for the
* signature parameters `v`, `r` and `s` for encoding. For an EIP-155 compliant
* representation for external signing use {@link FeeMarketEIP1559Transaction.getMessageToSign}.
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

