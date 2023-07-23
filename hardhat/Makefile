.PHONY: test clean compile deploy node

test:
	#REPORT_GAS=true npx hardhat test
	npx hardhat test

compile:
	npx hardhat compile
   
deploy-testnet:
	npx hardhat run scripts/deploy.js --network sepolia

node:
	npx hardhat node

deploy-local:
	npx hardhat run scripts/deploy.js --network localhost

