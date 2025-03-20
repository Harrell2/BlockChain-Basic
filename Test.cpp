#include <iostream>
#include <sha.h>
#include <string>
#include <oids.h>
#include "ESig.h"
#include <vector>
#include <eccrypto.h>
#include <osrng.h>
#include "GHash.h"
#include "JsonInteraction.h";
#include "BlockObjects.h"
#include <fstream>



using namespace CryptoPP;
using namespace std;
/*
    Currently: Create a Tree system that will be the parent class of Blockchain and merkle root, it is based on a linked list that points to prev
*/
/*
    Idea: pubKey will be a pointer to a wallet containing the money, a block containing the pubkey will denote the wallet spending money
    This will need a revamped system with inputs and outputs, look as lesson 4 for more info
    for now this idea is not implemented, and the sig system does nothing but verifies itself
*/
class Node
{
public:
    std::string id;
    Node* parent;
    int distance;


    Node(std::string name, Node* prev, int size)
    {
        id = name;
        parent = prev;
        distance = size;
    }
};


class Tree
{
public:
    std::vector<Node> container;

    Tree()
    {
        container = {};
    }


};

int main() {
    SHA256 hash;
    OID curve = ASN1::secp256r1();
    
    ECDSA<ECP, SHA256>::PrivateKey sKey;
    ECDSA<ECP, SHA256>::PublicKey pKey;
    
    //BlockChain HarrellDB = ReadChain();

    //cout << HarrellDB.verify(hash);
    
    Tree data;
    Node block("is 0", NULL, 0);
    data.container.push_back(block);
    for (int i = 1; i < 10; i++)
    {
        Node block("is " + to_string(i), &data.container[i-1], i);
        data.container.push_back(block);
    }


    cout << data.container[2].id << "\n" << ((*data.container[2].parent).id).size();

    return 0;
}




