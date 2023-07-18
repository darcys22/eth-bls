// provider.cpp
#include <iostream>

#include "eth-bls/provider.hpp"
#include "eth-bls/utils.hpp"


Provider::Provider(const std::string& name, const std::string& _url) 
    : clientName(name), url(_url) {
    // Initialize client
}

Provider::~Provider() {
    // Cleanup client
}

void Provider::connectToNetwork() {
    // Here we can verify connection by calling some simple JSON RPC method like `net_version`
    auto response = makeJsonRpcRequest("net_version", cpr::Body("{}"));
    if (response.status_code == 200) {
        std::cout << "Connected to the Ethereum network.\n";
    } else {
        std::cout << "Failed to connect to the Ethereum network.\n";
    }
}

void Provider::disconnectFromNetwork() {
    // Code to disconnect from Ethereum network
    std::cout << "Disconnected from the Ethereum network.\n";
}

cpr::Response Provider::makeJsonRpcRequest(const std::string& method, const nlohmann::json& params) {
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

std::string Provider::callReadFunction(const ReadCallData& callData) {
    // Prepare the params for the eth_call request
    nlohmann::json params = nlohmann::json::array();
    params[0]["to"] = callData.contractAddress;
    params[0]["data"] = callData.data;
    params[1] = "latest"; // or the block number you want to query against

    // Make the request
    cpr::Response response = makeJsonRpcRequest("eth_call", params);

    if (response.status_code == 200) {
        // Parse the response
        nlohmann::json responseJson = nlohmann::json::parse(response.text);

        // Check if the result field is present and not null, if it exists then it contains the return value
        if (!responseJson["result"].is_null()) {
            return responseJson["result"];
        }
    }

    // If we couldn't get the result, throw an exception
    throw std::runtime_error("Unable to get the result of the function call");
}

uint32_t Provider::getNetworkChainId() {
    // Make the request takes no params
    nlohmann::json params = nlohmann::json::array();
    cpr::Response response = makeJsonRpcRequest("net_version", params);

    if (response.status_code == 200) {
        // Parse the response
        nlohmann::json responseJson = nlohmann::json::parse(response.text);

        // Check if the result field is present and not null, if it exists then it contains the network id as a string
        if (!responseJson["result"].is_null()) {
            std::string network_id_string = responseJson["result"];
            uint64_t network_id = std::stoull(network_id_string, nullptr, 10);
            if (network_id > std::numeric_limits<uint32_t>::max()) {
                throw std::runtime_error("Network ID does not fit into 32 bit unsigned integer");
            } else {
                return static_cast<uint32_t>(network_id);
            }
        }
    }

    // If we couldn't get the network ID, throw an exception
    throw std::runtime_error("Unable to get Network ID");
}

uint64_t Provider::getTransactionCount(const std::string& address, const std::string& blockTag) {
    nlohmann::json params = nlohmann::json::array();
    params.push_back(address);
    params.push_back(blockTag);

    // Make the request
    cpr::Response response = makeJsonRpcRequest("eth_getTransactionCount", params);

    if (response.status_code == 200) {
        // Parse the response
        nlohmann::json responseJson = nlohmann::json::parse(response.text);

        // Check if the result field is present and not null
        if (!responseJson["result"].is_null()) {
            // Get the transaction count
            std::string transactionCountHex = responseJson["result"];

            // Convert the transaction count from hex to decimal
            uint64_t transactionCount = std::stoull(transactionCountHex, nullptr, 16);

            // Return the transaction count
            return transactionCount;
        }
    }

    // If we couldn't get the transaction count, throw an exception
    throw std::runtime_error("Unable to get transaction count");
}

std::optional<nlohmann::json> Provider::getTransactionByHash(const std::string& transactionHash) {
    nlohmann::json params = nlohmann::json::array();
    params.push_back(transactionHash);

    // Make the request
    cpr::Response response = makeJsonRpcRequest("eth_getTransactionByHash", params);

    if (response.status_code == 200) {
        // Parse the response
        nlohmann::json responseJson = nlohmann::json::parse(response.text);

        if (responseJson.find("error") != responseJson.end())
            throw std::runtime_error("Error getting transaction: " + responseJson["error"]["message"].get<std::string>());

        // Check if the result field is present and not null
        if (!responseJson["result"].is_null()) {
            // Return the block number
            return responseJson["result"];
        }
    }

    // If we couldn't get the block number, return an empty optional
    return std::nullopt;
}

// Create and send a raw transaction returns the hash but will also check that it got into the mempool
std::string Provider::sendTransaction(const Transaction& signedTx) {
    std::string hash = sendUncheckedTransaction(signedTx);

    std::future<std::string> futureTx = std::async(std::launch::async, [&]() -> std::string {
        std::vector<int> timeouts = { 4000, 100, 1000 };

        while(true) {
            const auto maybe_tx = getTransactionByHash(hash);
            if(maybe_tx) {
                return hash;
            }

            if(timeouts.empty()) {
                throw std::runtime_error("Transaction request timed out");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(timeouts.back()));
            timeouts.pop_back();
        }
    });

    return futureTx.get();
}

// Create and send a raw transaction returns the hash without checking if it succeeded in getting into the mempool
std::string Provider::sendUncheckedTransaction(const Transaction& signedTx) {
    nlohmann::json params = nlohmann::json::array();
    params.push_back(signedTx.serialized());
    
    auto response = makeJsonRpcRequest("eth_sendRawTransaction", params);
    if (response.status_code == 200) {
        nlohmann::json responseJson = nlohmann::json::parse(response.text);

        if (responseJson.find("error") != responseJson.end())
            throw std::runtime_error("Error sending transaction: " + responseJson["error"]["message"].get<std::string>());

        std::string hash = responseJson["result"].get<std::string>();

        return hash;
    } else {
        throw std::runtime_error("Failed to send transaction");
    }
}

uint64_t Provider::waitForTransaction(const std::string& txHash, int64_t timeout) {
    std::future<uint64_t> futureTx = std::async(std::launch::async, [&]() -> uint64_t {
        auto start = std::chrono::steady_clock::now();
        while(true) {
            const auto maybe_tx_json = getTransactionByHash(txHash);

            // If transaction is received, resolve the promise
            if(maybe_tx_json && !(*maybe_tx_json)["blockNumber"].is_null()) {
                // Parse the block number from the hex string
                std::string blockNumberHex = (*maybe_tx_json)["blockNumber"];
                return utils::fromHexStringToUint64(blockNumberHex);
            }

            auto now = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > timeout) {
                throw std::runtime_error("Transaction inclusion in a block timed out");
            }

            // Wait for a while before next check
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    
    return futureTx.get();
}

uint64_t Provider::getBalance(const std::string& address) {
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

FeeData Provider::getFeeData() {
    // Get latest block
    nlohmann::json params = nlohmann::json::array();
    params.push_back("latest");
    params.push_back(true);
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
    uint64_t maxPriorityFeePerGas = 3000000000;
    uint64_t maxFeePerGas = (baseFeePerGas * 2) + maxPriorityFeePerGas;

    return FeeData(gasPrice, maxFeePerGas, maxPriorityFeePerGas);
}
