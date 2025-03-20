
#ifndef JsonInteraction
#define JsonInteraction

#include "json.hpp";
#include "BlockObjects.h"
#include "GHash.h"
#include <filesystem>
#include <fstream>

using json = nlohmann::json;

// Takes file name and returns a json objects with the contents of the file
json readJson(std::string name)
{
    // Causes error when writing chain is needed in read chain
    std::filesystem::current_path("BlockChain");     // figuire out a way to take this code out to generalize the function for any json
    json data;
    std::ifstream file(name);
    
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << name << std::endl;
        return data;
    }
    data = json::parse(file);
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0)
    {
        return json::object();
    }
    file.seekg(0, std::ios::beg);

    file >> data;
    file.close();

    return data;
}

// Takes a file name and a json object, if the file already exist append data, otherwise create file
json writeJson(std::string fileName, json newData)
{
    json data = readJson(fileName);
    data.update(newData);
    std::ofstream out(fileName);
    out << data.dump(4);
    out.close();
    return data;
}

// A reader function that will be used in the createChain function and only needs given file name(folder chain not needed)
static Block readBlock(std::string name)
{
    json data = readJson(name);
    unsigned long long int nonce = data["nonce"];
    std::string message = data["message"];
    std::string signature = data["signature"];
    Integer prevHash = HexToInt(data["prevHash"]);
    Integer blockHash = HexToInt(data["blockHash"]);
    Integer difficulty = HexToInt(data["difficulty"]);
    
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    std::string gall = data["publicKey"];
    StringSource KeyDecrypt(hexToBinary(gall),true);
    publicKey.Load(KeyDecrypt);

    Block newBlock(nonce, message, signature, publicKey, prevHash, blockHash, difficulty);
    return newBlock;
}

// Takes a inputed Block then outputs it in the Blockchain folder
void writeBlock(std::string fileName, Block myBlock)
{
    json data;
    data["nonce"] = myBlock.nonce;
    data["message"] = myBlock.message;
    data["signature"] = myBlock.signature;
    data["prevHash"] = IntToHex(myBlock.prevHash);
    data["blockHash"] = IntToHex(myBlock.blockHash);
    data["difficulty"] = IntToHex(myBlock.difficulty);

    std::string pKeyDer;
    StringSink KeyEnc(pKeyDer);
    std::cout << "pKeyDer";
    myBlock.publicKey.Save(KeyEnc);
    
    data["publicKey"] = binaryToHex(pKeyDer);

    writeJson(fileName, data);
}

void WriteChain(int size)
{
    if (!std::filesystem::exists("BlockChain"))
    {
        std::cout << "BlockChain Directory Does Not Exist, Creating one\n";
        std::filesystem::create_directory("BlockChain");
    }
    std::filesystem::current_path("BlockChain");

    


    OID curve = ASN1::secp256r1();

    for (int i = 0; i < size; i++)
    {
        ECDSA<ECP, SHA256>::PrivateKey sKey;
        ECDSA<ECP, SHA256>::PublicKey pKey;

        std::string signature;
        std::string message;
        std::cout << "What is your message?";
        std::cin >> message;

        GenerateKey(sKey, pKey, curve);
        SignKey(message, signature, sKey);
        Block mine(message, signature, pKey);

        std::string file = "Block";
        file += std::to_string(i);
        file += ".json";
        //std::cout << file << std::endl;
        writeBlock(file, mine);
    }
}



BlockChain ReadChain()
{
    const std::filesystem::path ChainFolder("BlockChain");
    BlockChain myChain;
    for (auto const& fileBlock : std::filesystem::directory_iterator(ChainFolder))
    {
        std::string fileName = fileBlock.path().filename().string();
        Block newBlock = readBlock(fileName);
        myChain.add(newBlock);
    }
    return myChain;
}

#endif 
