// SPDX-License-Identifier: GPL-3.0
pragma solidity ^0.8.18;

contract BLSStubb {

    constructor() {
    }

    function state() public view returns (uint256 blockHeight, bytes32 blockHash) {
        blockHeight = block.number;
        blockHash = keccak256(abi.encodePacked(blockHeight));
        return (blockHeight, blockHash);
    }
}
