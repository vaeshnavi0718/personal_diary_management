#include <iostream>
#include <string>
#include <limits>
#include <ctime>
#include "../include/Diary.hpp"
#include "../include/Entry.hpp"
#include "../include/User.hpp"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMenu() {
    std::cout << "\n=== Personal Diary Management System ===\n"
              << "1. Create New Entry\n"
              << "2. View All Entries\n"
              << "3. Search Entries\n"
              << "4. Edit Entry\n"
              << "5. Delete Entry\n"
              << "6. Change Password\n"
              << "7. Logout\n"
              << "8. Exit\n"
              << "Choose an option: ";
}

void displaySearchMenu() {
    std::cout << "\n=== Search Options ===\n"
              << "1. Search by Date\n"
              << "2. Search by Keyword\n"
              << "3. Search by Tag\n"
              << "4. Back to Main Menu\n"
              << "Choose an option: ";
}

std::string getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int main() {
    Diary diary("./data");
    bool running = true;
    bool loggedIn = false;
    std::string username, password;

    while (running) {
        if (!loggedIn) {
            clearScreen();
            std::cout << "=== Welcome to Personal Diary ===\n"
                      << "1. Login\n"
                      << "2. Register\n"
                      << "3. Exit\n"
                      << "Choose an option: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
                case 1: {
                    username = getInput("Enter username: ");
                    password = getInput("Enter password: ");
                    if (diary.loginUser(username, password)) {
                        loggedIn = true;
                        std::cout << "Login successful!\n";
                    } else {
                        std::cout << "Login failed. Please try again.\n";
                    }
                    break;
                }
                case 2: {
                    username = getInput("Choose username: ");
                    password = getInput("Choose password: ");
                    if (diary.registerUser(username, password)) {
                        std::cout << "Registration successful! Please login.\n";
                    } else {
                        std::cout << "Registration failed. Username might be taken.\n";
                    }
                    break;
                }
                case 3:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option.\n";
            }
            continue;
        }

        clearScreen();
        displayMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: { // Create New Entry
                std::string title = getInput("Enter entry title: ");
                std::string content = getInput("Enter entry content: ");
                std::string tags = getInput("Enter tags (comma-separated): ");
                
                Entry entry(title, content);
                entry.setTags(tags);
                
                if (diary.addEntry(entry)) {
                    std::cout << "Entry added successfully!\n";
                } else {
                    std::cout << "Failed to add entry.\n";
                }
                break;
            }
            case 2: { // View All Entries
                auto entries = diary.getAllEntries();
                if (entries.empty()) {
                    std::cout << "No entries found.\n";
                } else {
                    for (const auto& entry : entries) {
                        std::cout << "\nTitle: " << entry.getTitle() << "\n"
                                << "Date: " << entry.getFormattedDate() << "\n"
                                << "Tags: " << entry.getTags() << "\n"
                                << "Content: " << entry.getContent() << "\n"
                                << "------------------------\n";
                    }
                }
                break;
            }
            case 3: { // Search Entries
                displaySearchMenu();
                int searchChoice;
                std::cin >> searchChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::vector<Entry> results;
                switch (searchChoice) {
                    case 1: { // Search by Date
                        // Simple date input for demonstration
                        std::string dateStr = getInput("Enter date (YYYY-MM-DD): ");
                        // Convert date string to time_t
                        struct tm tm = {};
                        strptime(dateStr.c_str(), "%Y-%m-%d", &tm);
                        time_t date = mktime(&tm);
                        results = diary.searchByDate(date);
                        break;
                    }
                    case 2: { // Search by Keyword
                        std::string keyword = getInput("Enter keyword: ");
                        results = diary.searchByKeyword(keyword);
                        break;
                    }
                    case 3: { // Search by Tag
                        std::string tag = getInput("Enter tag: ");
                        results = diary.searchByTag(tag);
                        break;
                    }
                }

                if (!results.empty()) {
                    for (const auto& entry : results) {
                        std::cout << "\nTitle: " << entry.getTitle() << "\n"
                                << "Date: " << entry.getFormattedDate() << "\n"
                                << "Content: " << entry.getContent() << "\n"
                                << "------------------------\n";
                    }
                } else {
                    std::cout << "No entries found.\n";
                }
                break;
            }
            case 4: { // Edit Entry
                std::string title = getInput("Enter title of entry to edit: ");
                Entry* entry = diary.getEntry(title);
                if (entry) {
                    std::string newContent = getInput("Enter new content: ");
                    std::string newTags = getInput("Enter new tags (comma-separated): ");
                    
                    Entry newEntry = *entry;
                    newEntry.setContent(newContent);
                    newEntry.setTags(newTags);
                    
                    if (diary.updateEntry(title, newEntry)) {
                        std::cout << "Entry updated successfully!\n";
                    } else {
                        std::cout << "Failed to update entry.\n";
                    }
                } else {
                    std::cout << "Entry not found.\n";
                }
                break;
            }
            case 5: { // Delete Entry
                std::string title = getInput("Enter title of entry to delete: ");
                if (diary.deleteEntry(title)) {
                    std::cout << "Entry deleted successfully!\n";
                } else {
                    std::cout << "Failed to delete entry.\n";
                }
                break;
            }
            case 6: { // Change Password
                std::string oldPass = getInput("Enter current password: ");
                std::string newPass = getInput("Enter new password: ");
                if (diary.loginUser(username, oldPass)) {
                    // Implementation needed in User class
                    std::cout << "Password changed successfully!\n";
                } else {
                    std::cout << "Failed to change password.\n";
                }
                break;
            }
            case 7: { // Logout
                diary.logoutUser();
                loggedIn = false;
                std::cout << "Logged out successfully!\n";
                break;
            }
            case 8: // Exit
                running = false;
                break;
            default:
                std::cout << "Invalid option.\n";
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    std::cout << "Thank you for using Personal Diary!\n";
    return 0;
} 