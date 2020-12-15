#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
private:
    std::string username;
    std::string passwordHash;
    std::string salt;
    std::string encryptionKey;
    bool isLoggedIn;

public:
    // Constructors
    User();
    User(const std::string& username, const std::string& password);

    // Authentication
    bool login(const std::string& password);
    void logout();
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);
    bool isAuthenticated() const;

    // Getters
    std::string getUsername() const;
    std::string getEncryptionKey() const;

    // Password management
    static std::string hashPassword(const std::string& password, const std::string& salt);
    static std::string generateSalt();
    
    // Serialization
    std::string serialize() const;
    static User deserialize(const std::string& data);

private:
    void generateEncryptionKey();
    bool verifyPassword(const std::string& password) const;
};

#endif // USER_HPP 