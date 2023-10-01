const chalk = require('chalk')
 
async function main() {
    const [deployer] = await ethers.getSigners();
    const network = await ethers.provider.getNetwork()

    console.log("Deploying contracts with the account:", await deployer.getAddress());

    const Merkle = await ethers.getContractFactory("MerkleRewards");

    const merkle = await Merkle.deploy();

    console.log(
        '  ',
        chalk.cyan(`Merkle Rewards Contract`),
        'deployed to:',
        chalk.greenBright(await merkle.getAddress()),
    )
}
 
main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });

