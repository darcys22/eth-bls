.PHONY: test clean compile deploy node

test:
	#REPORT_GAS=true npx hardhat test
	npx hardhat test

compile:
	npx hardhat compile
   
deploy-testnet:
	npx hardhat run scripts/deploy.js --network sepolia

node:
	npx hardhat node --vvvv

deploy-local:
	npx hardhat run scripts/deploy.js --network localhost

update-config:
	npx hardhat run scripts/update_config.js
