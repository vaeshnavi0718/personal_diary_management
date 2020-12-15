# Personal Diary Management System

A secure and efficient command-line personal diary management system implemented in C++ using Object-Oriented Programming principles. This application allows users to maintain an encrypted diary on their computer with features for managing entries, searching, and maintaining privacy.

## Features

- 🔐 **Secure Authentication**
  - User registration and login
  - Password hashing with salt
  - Session management

- 📝 **Entry Management**
  - Create, read, update, and delete diary entries
  - Add tags to entries for organization
  - Automatic timestamps for entries

- 🔒 **Security**
  - Encrypted storage of diary entries
  - Secure password handling
  - Automatic encryption/decryption of entries

- 🔍 **Search Functionality**
  - Search by date
  - Search by keywords
  - Search by tags

## Prerequisites

- C++17 compatible compiler
- CMake (version 3.10 or higher)
- OpenSSL development libraries

### Installing Dependencies

#### macOS
```bash
brew install cmake openssl
```

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install cmake libssl-dev
```

#### Windows
```bash
# Using vcpkg
vcpkg install openssl:x64-windows
```

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/yourusername/personal-diary.git
cd personal-diary
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build .
```

## Usage

1. Run the application:
```bash
./diary_manager
```

2. First-time users should register with a username and password.

3. After logging in, you can:
   - Create new entries
   - View all entries
   - Search entries by date, keyword, or tag
   - Edit existing entries
   - Delete entries
   - Change your password

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── include/                # Header files
│   ├── Diary.hpp          # Main diary management
│   ├── Entry.hpp          # Diary entry structure
│   ├── User.hpp           # User authentication
│   └── Encryption.hpp     # Security utilities
├── src/                   # Source files
│   ├── main.cpp          # Program entry point
│   ├── diary.cpp         # Diary implementation
│   ├── entry.cpp         # Entry implementation
│   ├── user.cpp          # User implementation
│   └── encryption.cpp    # Encryption implementation
└── data/                 # Data storage directory
```

## Security Notes

- Entries are encrypted using XOR encryption with base64 encoding
- Passwords are hashed using SHA-256 with salt
- Each user has their own encryption key derived from their credentials
- Data is stored in encrypted format on disk

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- OpenSSL for cryptographic functions
- C++ Standard Library
- CMake build system 