#pragma once
#ifndef BlockObjects
#define BLockObjects

#include <iostream>
#include <sha.h>
#include <string>
#include <vector>
#include <eccrypto.h>
#include "GHash.h"
#include "JsonInteraction.h";

using namespace CryptoPP;

class Block
{
public:

    unsigned long long int nonce = 0;
    std::string message;
    std::string signature;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    Integer prevHash;
    Integer blockHash;
    Integer difficulty = Integer::Power2(248);
    SHA256 hash;

    Block(std::string input, std::string sig, ECDSA<ECP, SHA256>::PublicKey pKey)
    {
        message = input;
        blockHash = miner();
        prevHash = 0;
        signature = sig;
        publicKey = pKey;
    }
    Block(std::string input, std::string sig, Integer lastHash, ECDSA<ECP, SHA256>::PublicKey pKey)
    {
        message = input;
        prevHash = lastHash;
        blockHash = miner();
        signature = sig;
        publicKey = pKey;
    }

    Block(unsigned long long int noncer, std::string input, std::string sig, ECDSA<ECP, SHA256>::PublicKey pKey, Integer lastHash, Integer myHash, Integer diff)
    {
        nonce = noncer;
        message = input;
        signature = sig;
        publicKey = pKey;
        prevHash = lastHash;
        blockHash = myHash;
        difficulty = diff;
    }

    Integer miner()
    {
        while (true)
        {
            if (HashNum(hash, message + std::to_string(nonce)) <= difficulty) break;
            nonce++;
        }
        return HashNum(hash, message + std::to_string(nonce));
    }


};

class BlockChain
{
public:
    std::vector<Block> blockList;

    BlockChain(std::vector<Block> list)
    {
        blockList.swap(list);
    }
    BlockChain()
    {
        blockList = {};
    }

    void add(Block newBlock)
    {
        if (blockList.size() == 0) newBlock.prevHash = 0;
        else newBlock.prevHash = blockList[blockList.size() - 1].blockHash;
        blockList.push_back(newBlock);
    }

    bool verify(SHA256 hash)
    {
        
        for (Block i:blockList)
        {
            if (i.blockHash != HashNum(hash, i.message + std::to_string(i.nonce)))
            {
                std::cout << IntToHex(i.blockHash) << "\n" << HashThis(hash, i.message + std::to_string(i.nonce)) << std::endl;

                return false;
            }
            
            if(!VerifySig(i.publicKey, i.message, i.signature)) return false;
        }

        for (int i = 1; i < blockList.size(); i++)
        {
            if (blockList[i].prevHash != blockList[i - 1].blockHash) return false;
        }

        return true;
    }

    void compare(SHA256 hash, BlockChain otherChain)
    {
        // Could impletment luckiest hash for most work done
        if (!otherChain.verify(hash)) return;
        if (!verify(hash) || blockList.size() >= otherChain.blockList.size()) return;
        blockList.swap(otherChain.blockList);
    }
};

#endif // !BlockObjects
