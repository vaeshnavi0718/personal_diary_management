#include "../include/Encryption.hpp"
#include <random>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

// XOR-based encryption (for demonstration - in production, use a proper encryption library)
std::string Encryption::encrypt(const std::string& data, const std::string& key) {
    std::string paddedKey = padKey(key, data.length());
    std::string result = data;
    
    for (size_t i = 0; i < data.length(); ++i) {
        result[i] = data[i] ^ paddedKey[i];
    }
    
    return base64Encode(std::vector<unsigned char>(result.begin(), result.end()));
}

std::string Encryption::decrypt(const std::string& encryptedData, const std::string& key) {
    std::vector<unsigned char> decodedData = base64Decode(encryptedData);
    std::string data(decodedData.begin(), decodedData.end());
    std::string paddedKey = padKey(key, data.length());
    std::string result = data;
    
    for (size_t i = 0; i < data.length(); ++i) {
        result[i] = data[i] ^ paddedKey[i];
    }
    
    return result;
}

std::string Encryption::hashString(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string Encryption::generateKey(const std::string& seed) {
    return hashString(seed + std::to_string(std::random_device()()));
}

std::string Encryption::base64Encode(const std::vector<unsigned char>& data) {
    static const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    for (unsigned char c : data) {
        char_array_3[i++] = c;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; i < 4; i++) {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }
    
    if (i) {
        for(j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        
        for (j = 0; j < i + 1; j++) {
            ret += base64_chars[char_array_4[j]];
        }
        
        while((i++ < 3)) {
            ret += '=';
        }
    }
    
    return ret;
}

std::vector<unsigned char> Encryption::base64Decode(const std::string& encoded) {
    static const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    std::vector<unsigned char> ret;
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    
    for (char c : encoded) {
        if (c == '=') {
            break;
        }
        
        if (base64_chars.find(c) != std::string::npos) {
            char_array_4[i++] = c;
            if (i == 4) {
                for (i = 0; i < 4; i++) {
                    char_array_4[i] = base64_chars.find(char_array_4[i]);
                }
                
                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
                
                for (i = 0; i < 3; i++) {
                    ret.push_back(char_array_3[i]);
                }
                i = 0;
            }
        }
    }
    
    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }
        
        for (j = 0; j < 4; j++) {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; j < i - 1; j++) {
            ret.push_back(char_array_3[j]);
        }
    }
    
    return ret;
}

std::string Encryption::padKey(const std::string& key, size_t length) {
    if (key.empty()) return std::string(length, '\0');
    
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += key[i % key.length()];
    }
    
    return result;
}

unsigned char Encryption::getRandom() {
    unsigned char buf;
    RAND_bytes(&buf, 1);
    return buf;
} 