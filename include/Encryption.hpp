#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>
#include <vector>

class Encryption {
public:
    // Basic encryption/decryption using XOR with key
    static std::string encrypt(const std::string& data, const std::string& key);
    static std::string decrypt(const std::string& encryptedData, const std::string& key);
    
    // Password hashing
    static std::string hashString(const std::string& input);
    
    // Key generation
    static std::string generateKey(const std::string& seed);
    
    // Utility functions
    static std::string base64Encode(const std::vector<unsigned char>& data);
    static std::vector<unsigned char> base64Decode(const std::string& encoded);

private:
    static std::string padKey(const std::string& key, size_t length);
    static unsigned char getRandom();
};

#endif // ENCRYPTION_HPP 