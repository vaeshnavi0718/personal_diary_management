#include "../include/Entry.hpp"
#include "../include/Encryption.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>

Entry::Entry() : timestamp(std::time(nullptr)), encrypted(false) {}

Entry::Entry(const std::string& title, const std::string& content)
    : title(title), content(content), timestamp(std::time(nullptr)), encrypted(false) {}

std::string Entry::getTitle() const {
    return title;
}

std::string Entry::getContent() const {
    return content;
}

std::time_t Entry::getTimestamp() const {
    return timestamp;
}

std::string Entry::getTags() const {
    return tags;
}

bool Entry::isEncrypted() const {
    return encrypted;
}

void Entry::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Entry::setContent(const std::string& newContent) {
    content = newContent;
}

void Entry::setTags(const std::string& newTags) {
    tags = newTags;
}

void Entry::encrypt(const std::string& key) {
    if (!encrypted) {
        content = Encryption::encrypt(content, key);
        encrypted = true;
    }
}

void Entry::decrypt(const std::string& key) {
    if (encrypted) {
        content = Encryption::decrypt(content, key);
        encrypted = false;
    }
}

std::string Entry::getFormattedDate() const {
    std::tm* timeinfo = std::localtime(&timestamp);
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Entry::serialize() const {
    std::stringstream ss;
    ss << title << "\n"
       << timestamp << "\n"
       << tags << "\n"
       << encrypted << "\n"
       << content;
    return ss.str();
}

Entry Entry::deserialize(const std::string& data) {
    std::stringstream ss(data);
    Entry entry;
    
    std::getline(ss, entry.title);
    ss >> entry.timestamp;
    ss.ignore(); // Skip newline
    std::getline(ss, entry.tags);
    ss >> entry.encrypted;
    ss.ignore(); // Skip newline
    std::getline(ss, entry.content, '\0'); // Read until the end
    
    return entry;
} 