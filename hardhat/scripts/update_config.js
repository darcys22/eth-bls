const fs = require('fs');
const path = require('path');

async function main() {

    // Read the network from BLS.network file
    const networkFilePath = path.join(__dirname, '../artifacts', 'BLS.network');
    const network = fs.readFileSync(networkFilePath, 'utf8').trim();

    // Map the network to a namespace
    let namespace;
    switch (network) {
      case 'localhost':
        namespace = 'ganache';
        break;
      case 'sepolia':
        namespace = 'sepolia';
        break;
      default:
        console.error(`Unknown network '${network}'`);
        process.exit(1);
    }

    // Read the address from BLS.address file
    const addressFilePath = path.join(__dirname, '../artifacts', 'BLS.address');
    const newAddress = fs.readFileSync(addressFilePath, 'utf8').trim();

    // Read the config file
    const configFilePath = path.join(__dirname, '../../include/eth-bls/config.hpp');
    let configFileData = fs.readFileSync(configFilePath, 'utf8');

    // Isolate the given namespace
    const namespaceStart = configFileData.indexOf(`namespace ${namespace} {`);
    const namespaceEnd = configFileData.indexOf('}', namespaceStart);

    if (namespaceStart === -1 || namespaceEnd === -1) {
      console.error(`namespace ${namespace} not found in the config file`);
      process.exit(1);
    }

    let namespaceData = configFileData.slice(namespaceStart, namespaceEnd);

    // Find the line that contains the BLS_CONTRACT_ADDRESS
    const searchStr = 'inline constexpr std::string_view BLS_CONTRACT_ADDRESS = ';
    const start = namespaceData.indexOf(searchStr);
    if (start === -1) {
      console.error('BLS_CONTRACT_ADDRESS not found in the namespace');
      process.exit(1);
    }

    const end = namespaceData.indexOf(';', start);
    if (end === -1) {
      console.error('Semicolon not found after BLS_CONTRACT_ADDRESS in the namespace');
      process.exit(1);
    }

    // Replace the address
    namespaceData = namespaceData.slice(0, start + searchStr.length) + '"' + newAddress + '"' + namespaceData.slice(end);

    // Replace the namespace in the config file data
    configFileData = configFileData.slice(0, namespaceStart) + namespaceData + configFileData.slice(namespaceEnd);

    // Write the modified data back to the config file
    fs.writeFileSync(configFilePath, configFileData);

    console.log('Contract address updated successfully');
}


main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });
