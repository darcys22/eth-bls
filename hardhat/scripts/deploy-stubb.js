const chalk = require('chalk')
 
async function main() {
    const [deployer] = await ethers.getSigners();
    const network = await ethers.provider.getNetwork()

    console.log("Deploying contracts with the account:", await deployer.getAddress());

    const Stubb = await ethers.getContractFactory("BLSStubb");

    const stubb = await Stubb.deploy();

    console.log(
        '  ',
        chalk.cyan(`BLS Stubb Contract`),
        'deployed to:',
        chalk.greenBright(await stubb.getAddress()),
    )
}
 
main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });

