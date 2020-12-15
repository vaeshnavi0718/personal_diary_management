#include "../include/Diary.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

Diary::Diary() : storageDirectory("./data") {
    fs::create_directories(storageDirectory);
}

Diary::Diary(const std::string& storageDir) : storageDirectory(storageDir) {
    fs::create_directories(storageDirectory);
}

bool Diary::registerUser(const std::string& username, const std::string& password) {
    std::string userFile = getUserFilePath();
    if (fs::exists(userFile)) {
        return false; // User already exists
    }
    
    currentUser = std::make_shared<User>(username, password);
    return saveToFile();
}

bool Diary::loginUser(const std::string& username, const std::string& password) {
    std::string userFile = getUserFilePath();
    if (!fs::exists(userFile)) {
        return false; // User doesn't exist
    }
    
    if (loadFromFile()) {
        if (currentUser && currentUser->getUsername() == username) {
            bool success = currentUser->login(password);
            if (success) {
                decryptEntries();
            }
            return success;
        }
    }
    return false;
}

void Diary::logoutUser() {
    if (currentUser) {
        encryptEntries();
        currentUser->logout();
        saveToFile();
    }
    entries.clear();
}

bool Diary::addEntry(const Entry& entry) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return false;
    }
    
    entries.push_back(entry);
    return saveToFile();
}

bool Diary::deleteEntry(const std::string& title) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return false;
    }
    
    auto it = std::find_if(entries.begin(), entries.end(),
                          [&title](const Entry& e) { return e.getTitle() == title; });
    
    if (it != entries.end()) {
        entries.erase(it);
        return saveToFile();
    }
    return false;
}

bool Diary::updateEntry(const std::string& title, const Entry& newEntry) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return false;
    }
    
    auto it = std::find_if(entries.begin(), entries.end(),
                          [&title](const Entry& e) { return e.getTitle() == title; });
    
    if (it != entries.end()) {
        *it = newEntry;
        return saveToFile();
    }
    return false;
}

Entry* Diary::getEntry(const std::string& title) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return nullptr;
    }
    
    auto it = std::find_if(entries.begin(), entries.end(),
                          [&title](const Entry& e) { return e.getTitle() == title; });
    
    return it != entries.end() ? &(*it) : nullptr;
}

std::vector<Entry> Diary::getAllEntries() const {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return std::vector<Entry>();
    }
    return entries;
}

std::vector<Entry> Diary::searchByDate(const std::time_t& date) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return std::vector<Entry>();
    }
    
    std::vector<Entry> results;
    std::tm search_tm = *std::localtime(&date);
    
    for (const auto& entry : entries) {
        std::tm entry_tm = *std::localtime(&entry.getTimestamp());
        if (entry_tm.tm_year == search_tm.tm_year &&
            entry_tm.tm_mon == search_tm.tm_mon &&
            entry_tm.tm_mday == search_tm.tm_mday) {
            results.push_back(entry);
        }
    }
    
    return results;
}

std::vector<Entry> Diary::searchByKeyword(const std::string& keyword) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return std::vector<Entry>();
    }
    
    std::vector<Entry> results;
    for (const auto& entry : entries) {
        if (entry.getTitle().find(keyword) != std::string::npos ||
            entry.getContent().find(keyword) != std::string::npos) {
            results.push_back(entry);
        }
    }
    
    return results;
}

std::vector<Entry> Diary::searchByTag(const std::string& tag) {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return std::vector<Entry>();
    }
    
    std::vector<Entry> results;
    for (const auto& entry : entries) {
        if (entry.getTags().find(tag) != std::string::npos) {
            results.push_back(entry);
        }
    }
    
    return results;
}

bool Diary::saveToFile() const {
    if (!currentUser) {
        return false;
    }
    
    // Save user data
    std::ofstream userFile(getUserFilePath());
    if (!userFile) {
        return false;
    }
    userFile << currentUser->serialize();
    userFile.close();
    
    // Save entries
    std::ofstream entriesFile(getEntriesFilePath());
    if (!entriesFile) {
        return false;
    }
    
    entriesFile << entries.size() << "\n";
    for (const auto& entry : entries) {
        entriesFile << entry.serialize() << "\n---END_ENTRY---\n";
    }
    
    return true;
}

bool Diary::loadFromFile() {
    // Load user data
    std::ifstream userFile(getUserFilePath());
    if (!userFile) {
        return false;
    }
    
    std::stringstream userBuffer;
    userBuffer << userFile.rdbuf();
    currentUser = std::make_shared<User>(User::deserialize(userBuffer.str()));
    userFile.close();
    
    // Load entries
    std::ifstream entriesFile(getEntriesFilePath());
    if (entriesFile) {
        entries.clear();
        size_t entryCount;
        entriesFile >> entryCount;
        entriesFile.ignore(); // Skip newline
        
        std::string entryData;
        std::string line;
        
        for (size_t i = 0; i < entryCount; ++i) {
            entryData.clear();
            while (std::getline(entriesFile, line) && line != "---END_ENTRY---") {
                entryData += line + "\n";
            }
            if (!entryData.empty()) {
                entries.push_back(Entry::deserialize(entryData));
            }
        }
    }
    
    return true;
}

std::string Diary::getUserFilePath() const {
    return storageDirectory + "/user.dat";
}

std::string Diary::getEntriesFilePath() const {
    return storageDirectory + "/entries.dat";
}

void Diary::encryptEntries() {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return;
    }
    
    std::string key = currentUser->getEncryptionKey();
    for (auto& entry : entries) {
        if (!entry.isEncrypted()) {
            entry.encrypt(key);
        }
    }
}

void Diary::decryptEntries() {
    if (!currentUser || !currentUser->isAuthenticated()) {
        return;
    }
    
    std::string key = currentUser->getEncryptionKey();
    for (auto& entry : entries) {
        if (entry.isEncrypted()) {
            entry.decrypt(key);
        }
    }
} 