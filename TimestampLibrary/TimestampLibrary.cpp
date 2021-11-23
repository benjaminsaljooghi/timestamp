// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "../cryptopp/cryptlib.h"
#include "../cryptopp/rijndael.h"
#include "../cryptopp/modes.h"
#include "../cryptopp/files.h"
#include "../cryptopp/osrng.h"
#include "../cryptopp/hex.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>


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

void ReadTimestamp()
{
    using namespace CryptoPP;

    std::string line;
    std::ifstream myfile("T:\\example.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            std::cout << line << '\n';
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";


    /*********************************\
    \*********************************/

    //try
    //{
    //    CBC_Mode< AES >::Decryption d;
    //    d.SetKeyWithIV(key, key.size(), iv);

    //    StringSource s(cipher, true,
    //        new StreamTransformationFilter(d,
    //            new StringSink(recovered)
    //        ) // StreamTransformationFilter
    //    ); // StringSource

    //    std::cout << "recovered text: " << recovered << std::endl;
    //}
    //catch (const Exception& e)
    //{
    //    std::cerr << e.what() << std::endl;
    //    exit(1);
    //}
}

__declspec(dllexport) void WriteTimestamp()
{
    uint64_t time = timeSinceEpochMillisec();
    std::ostringstream oss;
    uint64_t i = time;
    oss << i;
    std::string plain(oss.str());

    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    //std::string plain = "CBC Mode Test";
    std::string cipher, recovered;

    std::cout << "plain text: " << plain << std::endl;

    /*********************************\
    \*********************************/

    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

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
    //myfile << cipher;

    HexEncoder fileEncoder(new FileSink(myfile));

    std::cout << "cipher text written to file..." << std::endl;
    fileEncoder.Put((const byte*)&cipher[0], cipher.size());
    fileEncoder.MessageEnd();
    //std::cout << std::endl;

    myfile.close();


}

__declspec(dllexport) BOOL CheckTimestamp()
{
    ReadTimestamp();
    return false;
}

