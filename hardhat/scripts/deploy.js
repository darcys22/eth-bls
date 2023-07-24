const chalk = require('chalk')
const fs = require('fs')
 
async function main() {
    const [deployer] = await ethers.getSigners();
    const network = await ethers.provider.getNetwork()

    console.log("Deploying contracts with the account:", await deployer.getAddress());

    const Library = await ethers.getContractFactory("BN256G2");
    const library = await Library.deploy();
    await library.waitForDeployment();

    const BLS = await ethers.getContractFactory("BLSValidators", {
        libraries: {
            BN256G2: await library.getAddress()
        }
    });

    const bls = await BLS.deploy();

    console.log(
        '  ',
        chalk.cyan(`BLS Validator Contract`),
        'deployed to:',
        chalk.greenBright(await bls.getAddress()),
    )
    fs.writeFileSync(`artifacts/BLS.address`, await bls.getAddress())
    console.log(network.name)
    fs.writeFileSync(`artifacts/BLS.network`, network.name)
}
 
main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });

