// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "../cryptopp/cryptlib.h"
#include "../cryptopp/rijndael.h"
#include "../cryptopp/modes.h"
#include "../cryptopp/files.h"
#include "../cryptopp/osrng.h"
#include "../cryptopp/hex.h"
#include "../cryptopp/eax.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

using namespace CryptoPP;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::string GenerateCurrentTimestamp()
{
    uint64_t time = timeSinceEpochMillisec();
    std::ostringstream oss;
    uint64_t i = time;
    oss << i;
    std::string plain(oss.str());
    return plain;
}


AutoSeededRandomPool prng;
SecByteBlock key(AES::DEFAULT_KEYLENGTH);
SecByteBlock iv(AES::BLOCKSIZE);


uint64_t ReadTimestamp()
{

    std::string line;
    std::ifstream myfile("T:\\example.txt");
    std::cout << "reading raw cipher from file..." << std::endl;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            std::cout << line << '\n';
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";

    std::string cipher = line;

    // attack the first and last bytes
    //cipher[ 0 ] ^= 0x01;
    //cipher[ cipher.size()-1 ] ^= 0x01;

    std::string recovered;
    try
    {
        EAX< AES >::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        StringSource s(cipher, true,
            new AuthenticatedDecryptionFilter(d,
                new StringSink(recovered)
            ) // StreamTransformationFilter
        );

        std::cout << "recovered text: " << recovered << std::endl;

        uint64_t finalValue;
        std::istringstream iss(recovered);
        iss >> finalValue;
        return finalValue;
        //return 0;
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

__declspec(dllexport) void WriteTimestamp()
{
    HexEncoder encoder(new FileSink(std::cout));
    
    std::string plain = GenerateCurrentTimestamp();
    std::string cipher;

    std::cout << "plain text: " << plain << std::endl;

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());
    try
    {
        EAX< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
            new AuthenticatedEncryptionFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter
        );
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    std::cout << "key: ";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "iv: ";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::ofstream myfile;
    myfile.open("T:\\example.txt");

    std::cout << "cipher text written to file..." << std::endl;
    myfile << cipher;

    myfile.close();
}

__declspec(dllexport) BOOL CheckTimestamp()
{
    uint64_t timestamp = ReadTimestamp();
    std::cout << "final: " << std::endl;
    std::cout << timestamp << std::endl;
    return false;
}

