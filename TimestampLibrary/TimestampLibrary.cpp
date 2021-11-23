// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "../cryptopp/cryptlib.h"
#include "../cryptopp/rijndael.h"
#include "../cryptopp/modes.h"
#include "../cryptopp/files.h"
#include "../cryptopp/osrng.h"
#include "../cryptopp/hex.h"
#include "../cryptopp/eax.h"

#include <thread>
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

std::string timestamp_file = "T:\\timestamp.txt";
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
            //std::cout << line << '\n';
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
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 0;
        //exit(1);
    }
}

inline bool exists_test0(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

__declspec(dllexport) BOOL WriteTimestamp()
{
    HexEncoder encoder(new FileSink(std::cout));
    
    std::string plain = GenerateCurrentTimestamp();
    std::string cipher;

    std::cout << "writing timestamp to file: " << plain << std::endl;

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

    //std::cout << "key: ";
    //encoder.Put(key, key.size());
    //encoder.MessageEnd();
    //std::cout << std::endl;

    //std::cout << "iv: ";
    //encoder.Put(iv, iv.size());
    //encoder.MessageEnd();
    //std::cout << std::endl;


    if (exists_test0(timestamp_file)) {
        std::cout << "file exists!" << std::endl;
        return false;
    }

    std::ofstream myfile;
    //std::remove(timestamp_file);

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));


    myfile.open(timestamp_file);

    std::cout << "cipher text written to file..." << std::endl;
    myfile << cipher;

    myfile.close();

    return true;
}

__declspec(dllexport) BOOL CheckTimestamp(uint64_t expires_after)
{
    std::cout << "checking file..." << std::endl;

    if (!exists_test0(timestamp_file)) {
        std::cout << "file does not exist so I couldn't check the timestamp" << std::endl;
        return false;
    }

    uint64_t current = timeSinceEpochMillisec();
    uint64_t timestamp = ReadTimestamp();
    if (timestamp == 0)
    {
        std::cout << "timestamp was tampered with!!!" << std::endl;
        return false;
    }

    uint64_t diff = (current - timestamp) / 1e3;
        
    std::cout << diff << " seconds have passed!" << std::endl;
    bool expired = expires_after < diff;

    std::string expiry_status = expired ? "EXPIRED" : "NOT EXPIRED";
    std::cout << "the software has: " << expiry_status << std::endl;

    return !expired;
}

