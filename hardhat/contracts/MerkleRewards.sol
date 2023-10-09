// SPDX-License-Identifier: GPL-3.0
pragma solidity ^0.8.18;

import "@openzeppelin/contracts/token/ERC20/IERC20.sol";

import "@openzeppelin/contracts/access/Ownable.sol";
import "@openzeppelin/contracts/utils/cryptography/MerkleProof.sol";

import "hardhat/console.sol";

contract MerkleRewards is Ownable {
    IERC20 public immutable token;

    bytes32 public rewardsMerkleRoot = 0x9c092cb86c4a1d8ee84a0bec887e2011c5854232fa3946c6b707a84dbf185b75;
    mapping(address => bool) internal rewardClaimed;
    address[] rewardClaimedList;

    /*constructor(address _token){*/
       /*token = IERC20(_token);*/
    /*}*/
    constructor(){
        address myAddress = 0xE0f5206BBD039e7b0592d8918820024e2a7437b9;
        token = IERC20(address(myAddress));
    }

    function updateRewardsMerkleRoot(bytes32 _merkleRoot) external onlyOwner {
        rewardsMerkleRoot = _merkleRoot;
        resetRewardClaimed();
    }
    
    function abiencode(address target, uint256 balance) public pure returns (bytes memory) {
        return abi.encode(target, balance);
    }

    function hash(address target, uint256 balance) public pure returns (bytes32) {
        return keccak256(abi.encode(target,balance));
    }

    function resetRewardClaimed() internal onlyOwner {
        for (uint256 i = 0; i < rewardClaimedList.length; i++) {
            rewardClaimed[rewardClaimedList[i]] = false;
        }
        delete rewardClaimedList;
    }

    function claimRewards(uint256 quantity, bytes32[] calldata _merkleProof) external {
        require(!rewardClaimed[msg.sender], "Address has already claimed");
        bytes32 leaf = hash(msg.sender, quantity);
        require(MerkleProof.verify(_merkleProof, rewardsMerkleRoot, leaf), "Must be in rewards list");
        rewardClaimed[msg.sender] = true;
        rewardClaimedList.push(msg.sender);
        token.transfer(msg.sender, quantity);
    }

    function withdraw() public payable onlyOwner {
        (bool os, ) = payable(owner()).call{value: address(this).balance}("");
        require(os);
    }

    function validateProof(uint256 _quantity, bytes32[] calldata _merkleProof) external {
        bytes32 leaf = hash(msg.sender, _quantity);
        /*bytes32 leaf = keccak256("1");*/
        console.log(toHexString(uint256(leaf)));
        require(MerkleProof.verify(_merkleProof, rewardsMerkleRoot, leaf), "Must be in rewards list");
    }

    function toHexString(uint256 value) public pure returns (string memory) {
        // Create a lookup table
        bytes16 lookup = "0123456789abcdef";

        // Initialize an array
        bytes memory result = new bytes(64);
        for (uint i = 0; i < 32; i++) {
            result[i*2] = lookup[uint8(value>>(8*(31-i)))/16];
            result[1+i*2] = lookup[uint8(value>>(8*(31-i)))%16];
        }

        // Prefix with 0x
        return string(abi.encodePacked("0x", result));
    }


    function bytesToHexString(bytes memory value) public pure returns (string memory) {
        // Create a lookup table
        bytes16 lookup = "0123456789abcdef";
        
        // Initialize an array based on the length of input
        bytes memory result = new bytes(2 * value.length);  
        for (uint i = 0; i < value.length; i++) {
            result[i*2] = lookup[uint8(value[i]) / 16];
            result[1 + i*2] = lookup[uint8(value[i]) % 16];
        }

        // Prefix with 0x
        return string(abi.encodePacked("0x", result));
    }
}
