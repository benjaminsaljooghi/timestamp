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
#include <stdio.h>

#include "TimestampLibrary.h"

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

AutoSeededRandomPool prng;
SecByteBlock key(AES::DEFAULT_KEYLENGTH);
SecByteBlock iv(AES::BLOCKSIZE);

uint64_t time_since_epoch_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::string gen_current_timestamp()
{
    uint64_t time = time_since_epoch_ms();
    std::ostringstream oss;
    uint64_t i = time;
    oss << i;
    std::string plain(oss.str());
    return plain;
}

uint64_t Timestamp::read_timestamp(std::string timestamp_path)
{
    std::string line;
    std::ifstream myfile(timestamp_path);
    std::cout << "reading raw cipher from file..." << std::endl;
    if (myfile.is_open())
    {
        getline(myfile, line);
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file";
        return 0;
    }

    std::string cipher;

    HexDecoder decoder(new StringSink(cipher));
    decoder.Put((const byte*)&line[0], line.size());
    decoder.MessageEnd();

    // EXPERIMENT: attack the first byte
    // cipher[ 0 ] ^= 0x01;

    std::string recovered;
    try
    {
        EAX< AES >::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        StringSource s(cipher, true,
            new AuthenticatedDecryptionFilter(d,
                new StringSink(recovered)
            )
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
    }
}

bool file_exists(const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

__declspec(dllexport) bool Timestamp::write_timestamp(std::string timestamp_path)
{
    if (file_exists(timestamp_path)) {
        std::cout << "file exists!" << std::endl;
        return false;
    }
    std::ofstream timestamp_file;
    timestamp_file.open(timestamp_path);

    HexEncoder encoderFile(new FileSink(timestamp_file));

    std::string plain = gen_current_timestamp();

    std::cout << "writing timestamp to file: " << plain << std::endl;

    std::string cipher;
    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());
    try
    {
        EAX< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
            new AuthenticatedEncryptionFilter(e,
                new StringSink(cipher)
            )
        );
    }
    catch (const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    
    encoderFile.Put((const byte*)&cipher[0], cipher.size());
    encoderFile.MessageEnd();
    timestamp_file.close();
    std::cout << "cipher text written to file." << std::endl;

    return true;
}

__declspec(dllexport) bool Timestamp::check_expiry(std::string timestamp_path, uint64_t expires_after)
{
    std::cout << "checking file..." << std::endl;

    if (!file_exists(timestamp_path)) {
        std::cout << "file does not exist so I couldn't check the timestamp" << std::endl;
        return false;
    }

    uint64_t current = time_since_epoch_ms();
    uint64_t timestamp = Timestamp::read_timestamp(timestamp_path);
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

