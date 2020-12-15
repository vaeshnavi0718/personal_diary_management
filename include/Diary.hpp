#ifndef DIARY_HPP
#define DIARY_HPP

#include <string>
#include <vector>
#include <memory>
#include "Entry.hpp"
#include "User.hpp"

class Diary {
private:
    std::shared_ptr<User> currentUser;
    std::vector<Entry> entries;
    std::string storageDirectory;

public:
    // Constructors
    Diary();
    explicit Diary(const std::string& storageDir);

    // User management
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    void logoutUser();

    // Entry management
    bool addEntry(const Entry& entry);
    bool deleteEntry(const std::string& title);
    bool updateEntry(const std::string& title, const Entry& newEntry);
    Entry* getEntry(const std::string& title);
    std::vector<Entry> getAllEntries() const;
    
    // Search functionality
    std::vector<Entry> searchByDate(const std::time_t& date);
    std::vector<Entry> searchByKeyword(const std::string& keyword);
    std::vector<Entry> searchByTag(const std::string& tag);

    // Storage management
    bool saveToFile() const;
    bool loadFromFile();

private:
    std::string getUserFilePath() const;
    std::string getEntriesFilePath() const;
    void encryptEntries();
    void decryptEntries();
};

#endif // DIARY_HPP 