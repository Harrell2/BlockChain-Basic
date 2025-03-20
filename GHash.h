#ifndef GHash
#define GHash
#undef byte
#include <iostream>
#include <sha.h>
#include <string>
#include <hex.h>

using namespace CryptoPP;
std::string HexRefrence[6] = { "A","B","C","D","E","F" };

std::string HashThis(SHA256 hash, std::string input)
{
    std::string digest;
    StringSource ball(
        input,
        true,
        new HashFilter(
            hash,
            new HexEncoder(
                new StringSink(digest)
            )
        )
    );
    return digest;
}

Integer HashNum(SHA256 hash, std::string input)
{
    std::string digest;

    StringSource ball(input, true, new HashFilter(hash, new StringSink(digest)));
    Integer sender((const CryptoPP::byte*)digest.data(), digest.size());

    return sender;
}

std::string IntToHex(Integer num)
{
    std::string hexNum = "";
    while (num > 0)
    {
        int remainder = num % 16;
        num /= 16;
        if (remainder < 10) hexNum.insert(0, std::to_string(remainder));
        else hexNum.insert(0, HexRefrence[remainder - 10]);
    }

    while (hexNum.length() < 64) hexNum.insert(0, "0");

    return hexNum;
}

Integer HexToInt(std::string hex)
{
    if (hex.size() % 2 == 1) std::cerr << "Wrong hex length(must be even)";
    std::string binary;

    StringSource carb(hex, true,
        new HexDecoder(
            new StringSink(binary)
        )
    );
    
    Integer intVal;
    if (!binary.empty()) {
        intVal = Integer(
            reinterpret_cast<const CryptoPP::byte*>(binary.data()),
            binary.size()
        );
    }
    
    return intVal;
}

std::string binaryToHex(std::string& data) {
    std::string hex;
    HexEncoder encoder(new StringSink(hex));
    encoder.Put((const CryptoPP::byte*)data.data(), data.size());
    encoder.MessageEnd();
    return hex;
}

std::string hexToBinary(const std::string hex) {
    std::string binary;
    StringSource(hex, true,
        new HexDecoder(
            new StringSink(binary)
        )
    );
    return binary;
}


#endif // !GHash
