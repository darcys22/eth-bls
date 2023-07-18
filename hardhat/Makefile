.PHONY: test clean compile deploy node

test:
	#REPORT_GAS=true npx hardhat test
	npx hardhat test

compile:
	npx hardhat compile
   
deploy:
	npx hardhat run scripts/deploy.js --network sepolia

# run npx hardhat run scripts/deploy.js --network hardhat immediately after
node:
	npx hardhat node


