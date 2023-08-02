// SPDX-License-Identifier: GPL-3.0
pragma solidity ^0.8.18;

import { BN256G2 } from "./BN256G2.sol";

import "@openzeppelin/contracts/utils/Strings.sol";

import "hardhat/console.sol";

contract BLSValidators {

    struct G1Point {
        uint X;
        uint Y;
    }

    // Encoding of field elements is: X[0] * z + X[1]
    struct G2Point {
        uint[2] X;
        uint[2] Y;
    }

    struct Validator {
        address user;
        uint256 amount;
        G1Point pubkey;
    }

    Validator[] public validators;

    event newValidator(uint256 indexed validatorId);

    G1Point _P1;
    G2Point _P2;

    constructor() {
        _P1 = G1Point(1, 2);
        _P2 = G2Point(
            [11559732032986387107991004021392285783925812861821192530917403151452391805634,
            10857046999023057135944570762232829481370756359578518086990519993285655852781],

            [4082367875863433681332203403145435568316851327593401208105741076214120093531,
            8495653923123431417604973247489272438418190587263600148770280649306958101930]
        );
    }

    function addValidator(uint256 pkX, uint256 pkY, uint256 amount) public {
         validators.push(Validator(msg.sender, amount, G1Point(pkX, pkY)));
         emit newValidator(validators.length - 1);
    }

    function addValidatorTest(uint256 amount, uint256 _pk,uint256 n) public {
        for(uint256 i = 0 ;i < n; i++) {
         // Temporary
         G1Point memory pk = mul(P1(), _pk+i);
         validators.push(Validator(msg.sender,amount, pk));
        }
    }

    function getValidatorsLength() public view returns (uint) {
        return validators.length;
    }

    function clearValidators() public {
        delete validators;
    }

    function getValidatorDetails(uint256 id) public view
     returns(
        address,
        uint256,
        uint256,
        uint256
        ) {
        return (validators[id].user, validators[id].amount, validators[id].pubkey.X, validators[id].pubkey.Y);
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

    function checkSigAGG(uint256 sigs0, uint256 sigs1, uint256 sigs2, uint256 sigs3, uint256 message) public {
        G1Point memory pubkey;
        for(uint256 i = 0; i < validators.length; i++) {
            Validator memory v = validators[i];
            pubkey = add(pubkey, v.pubkey);
        }

        G2Point memory Hm = hashToG2(message);
        G2Point memory signature = G2Point([sigs1,sigs0],[sigs3,sigs2]);
        require(pairing2(P1(), Hm, negate(pubkey), signature), "Something went wrong");
        require(pairing2(P1(), signature, negate(pubkey), Hm), "Something went wrong"); //SD implementation
    }

    function checkAggPubkey(uint256 pkX, uint256 pkY) public {
        require(validators.length > 1);
        G1Point memory pubkey = validators[0].pubkey;
        for(uint256 i = 1; i < validators.length; i++) {
            pubkey = add(pubkey, validators[i].pubkey);
        }
        require(pubkey.X == pkX, "pubkey doesnt match");
        require(pubkey.Y == pkY, "pubkey doesnt match");
    }

    /// @return the generator of G1
    function P1() public view returns (G1Point memory) {
        return _P1;
    }

    /// @return the generator of G2
    function P2() public view returns (G2Point memory) {
        return _P2;
    }

    /// @return the result of computing the pairing check
    /// e(p1[0], p2[0]) *  .... * e(p1[n], p2[n]) == 1
    /// For example pairing([P1(), P1().negate()], [P2(), P2()]) should
    /// return true.
    function pairing(G1Point[] memory p1, G2Point[] memory p2) internal returns (bool) {
        require(p1.length == p2.length);
        uint elements = p1.length;
        uint inputSize = elements * 6;
        uint[] memory input = new uint[](inputSize);

        for (uint i = 0; i < elements; i++)
        {
            input[i * 6 + 0] = p1[i].X;
            input[i * 6 + 1] = p1[i].Y;
            input[i * 6 + 2] = p2[i].X[0];
            input[i * 6 + 3] = p2[i].X[1];
            input[i * 6 + 4] = p2[i].Y[0];
            input[i * 6 + 5] = p2[i].Y[1];
        }

        uint[1] memory out;
        bool success;

        assembly {
            success := call(sub(gas(), 2000), 8, 0, add(input, 0x20), mul(inputSize, 0x20), out, 0x20)
        }
        require(success, "Call to precompiled contract for pairing failed");
        return out[0] != 0;
    }

    /// Convenience method for a pairing check for two pairs.
    function pairing2(G1Point memory a1, G2Point memory a2, G1Point memory b1, G2Point memory b2) internal returns (bool) {
        G1Point[] memory p1 = new G1Point[](2);
        G2Point[] memory p2 = new G2Point[](2);
        p1[0] = a1;
        p1[1] = b1;
        p2[0] = a2;
        p2[1] = b2;
        return pairing(p1, p2);
    }

    function hashToG1(uint256 h) public returns (G1Point memory) {
        return mul(P1(), h);
    }

    // hashes to G2 using the try and increment method
    function mapToG2(uint256 h) public view returns (G2Point memory) {
        // Define the G2Point coordinates
        uint256 x1 = h;
        uint256 x2 = 0;
        uint256 y1;
        uint256 y2;

        bool foundValidPoint = false;

        // Iterate until we find a valid G2 point
        while (!foundValidPoint) {
            // Try to get y^2
            (uint256 yx, uint256 yy) = BN256G2.Get_yy_coordinate(x1, x2);

            // Calculate square root
            (uint256 sqrt_x, uint256 sqrt_y) = BN256G2.FQ2Sqrt(yx, yy);

            // Check if this is a point
            if (sqrt_x != 0 && sqrt_y != 0) {
                y1 = sqrt_x;
                y2 = sqrt_y;
                if (BN256G2.IsOnCurve(x1, x2, y1, y2)) {
                    foundValidPoint = true;
                }
            } else {
                // Increment x coordinate and try again.
                x1 += 1;
            }
        }

        return (G2Point([x2,x1],[y2,y1]));
    }

    function hashToG2(uint256 h) public view returns (G2Point memory) {
        G2Point memory map = mapToG2(h);
        (uint256 x1, uint256 x2, uint256 y1, uint256 y2) = BN256G2.ECTwistMulByCofactor(map.X[1], map.X[0], map.Y[1], map.Y[0]);
        return (G2Point([x2,x1],[y2,y1]));
    }

    function rootFp2(uint256 x, uint256 y) public view returns (uint256, uint256) {
        return BN256G2.FQ2Sqrt(x,y);
    }

    function getWeierstrass(uint256 x, uint256 y) public pure returns (uint256, uint256) {
        return BN256G2.Get_yy_coordinate(x,y);
    }


    function modPow(uint256 base, uint256 exponent, uint256 modulus) internal returns (uint256) {
        uint256[6] memory input = [32, 32, 32, base, exponent, modulus];
        uint256[1] memory result;
        assembly {
            if iszero(call(not(0), 0x05, 0, input, 0xc0, result, 0x20)) {
                revert(0, 0)
            }
        }
        return result[0];
    }

    /// @return r the negation of p, i.e. p.add(p.negate()) should be zero.
    function negate(G1Point memory p) internal pure returns (G1Point memory r) {
        // The prime q in the base field F_q for G1
        uint q = 21888242871839275222246405745257275088696311157297823662689037894645226208583;
        if (p.X == 0 && p.Y == 0)
            return G1Point(0, 0);
        return G1Point(p.X, q - (p.Y % q));
    }

    /// @return r the sum of two points of G1
    function add(G1Point memory p1, G1Point memory p2) internal returns (G1Point memory r) {
        uint[4] memory input;
        input[0] = p1.X;
        input[1] = p1.Y;
        input[2] = p2.X;
        input[3] = p2.Y;
        bool success;
        assembly {
            success := call(sub(gas(), 2000), 6, 0, input, 0xc0, r, 0x60)
        }
        require(success, "Call to precompiled contract for add failed");
    }

    /// @return r the product of a point on G1 and a scalar, i.e.
    /// p == p.mul(1) and p.add(p) == p.mul(2) for all points p.
    function mul(G1Point memory p, uint s) internal returns (G1Point memory r) {
        uint[3] memory input;
        input[0] = p.X;
        input[1] = p.Y;
        input[2] = s;
        bool success;
        assembly {
            success := call(sub(gas(), 2000), 7, 0, input, 0x80, r, 0x60)
        }
        require(success, "Call to precompiled contract for add failed");
    }
}
