#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <string>
#include <ctime>

class Entry {
private:
    std::string title;
    std::string content;
    std::time_t timestamp;
    std::string tags;
    bool encrypted;

public:
    // Constructors
    Entry();
    Entry(const std::string& title, const std::string& content);
    
    // Getters
    std::string getTitle() const;
    std::string getContent() const;
    std::time_t getTimestamp() const;
    std::string getTags() const;
    bool isEncrypted() const;
    
    // Setters
    void setTitle(const std::string& title);
    void setContent(const std::string& content);
    void setTags(const std::string& tags);
    
    // Utility functions
    void encrypt(const std::string& key);
    void decrypt(const std::string& key);
    std::string getFormattedDate() const;
    
    // Serialization
    std::string serialize() const;
    static Entry deserialize(const std::string& data);
};

#endif // ENTRY_HPP 