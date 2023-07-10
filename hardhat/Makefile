.PHONY: test clean compile deploy

test:
	#REPORT_GAS=true npx hardhat test
	npx hardhat test

compile:
	npx hardhat compile
   
deploy:
	npx hardhat run scripts/deploy.js --network sepolia

