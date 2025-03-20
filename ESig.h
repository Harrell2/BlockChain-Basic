
#ifndef Esig
#define ESig

#include <eccrypto.h>
#include <oids.h>
#include <cryptlib.h>
#include <osrng.h>
#include <hex.h>
#include "GHash.h"
using namespace CryptoPP;


void GenerateKey(ECDSA<ECP, SHA256>::PrivateKey& privateKey, ECDSA<ECP, SHA256>::PublicKey& publicKey,OID curve)
{
    AutoSeededRandomPool rng;
    privateKey.Initialize(rng, curve);
    privateKey.MakePublicKey(publicKey);
}

void SignKey(const std::string& message, std::string& signature, const ECDSA<ECP, SHA256>::PrivateKey& skey )
{ 
    AutoSeededRandomPool rng;
    ECDSA<ECP, SHA256>::Signer signer(skey);

    size_t siglen = signer.MaxSignatureLength(); 
    signature.resize(siglen);
    siglen = signer.SignMessage(rng,
        (const CryptoPP::byte*)message.data(),
        message.size(),
        (CryptoPP::byte*)signature.data());
    signature.resize(siglen);

    signature = binaryToHex(signature);
}

bool VerifySig(ECDSA<ECP, SHA256>::PublicKey pKey, std::string message, std::string signature)
{
    AutoSeededRandomPool rng;
    signature = hexToBinary(signature);

    ECDSA<ECP, SHA256>::Verifier verifier(pKey);
    bool verified = verifier.VerifyMessage(
        (const CryptoPP::byte*)message.data(),
        message.size(),
        (const CryptoPP::byte*)signature.data(),
        signature.size()
    );

    return verified;
}

#endif 
