#include "../include/User.hpp"
#include "../include/Encryption.hpp"
#include <random>
#include <sstream>

User::User() : isLoggedIn(false) {}

User::User(const std::string& username, const std::string& password)
    : username(username), isLoggedIn(false) {
    salt = generateSalt();
    passwordHash = hashPassword(password, salt);
    generateEncryptionKey();
}

bool User::login(const std::string& password) {
    if (verifyPassword(password)) {
        isLoggedIn = true;
        generateEncryptionKey();
        return true;
    }
    return false;
}

void User::logout() {
    isLoggedIn = false;
    encryptionKey.clear();
}

bool User::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!verifyPassword(oldPassword)) {
        return false;
    }
    
    salt = generateSalt();
    passwordHash = hashPassword(newPassword, salt);
    generateEncryptionKey();
    return true;
}

bool User::isAuthenticated() const {
    return isLoggedIn;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getEncryptionKey() const {
    return encryptionKey;
}

std::string User::hashPassword(const std::string& password, const std::string& salt) {
    return Encryption::hashString(password + salt);
}

std::string User::generateSalt() {
    const int saltLength = 16;
    std::string salt;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (int i = 0; i < saltLength; ++i) {
        salt += static_cast<char>(dis(gen));
    }
    
    return salt;
}

std::string User::serialize() const {
    std::stringstream ss;
    ss << username << "\n"
       << passwordHash << "\n"
       << salt;
    return ss.str();
}

User User::deserialize(const std::string& data) {
    std::stringstream ss(data);
    User user;
    
    std::getline(ss, user.username);
    std::getline(ss, user.passwordHash);
    std::getline(ss, user.salt);
    
    return user;
}

void User::generateEncryptionKey() {
    if (isLoggedIn) {
        encryptionKey = Encryption::generateKey(username + passwordHash);
    }
}

bool User::verifyPassword(const std::string& password) const {
    return passwordHash == hashPassword(password, salt);
} 