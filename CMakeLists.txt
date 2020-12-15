cmake_minimum_required(VERSION 3.10)
project(PersonalDiary)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/diary.cpp
    src/entry.cpp
    src/user.cpp
    src/encryption.cpp
)

# Add header files
set(HEADERS
    include/Diary.hpp
    include/Entry.hpp
    include/User.hpp
    include/Encryption.hpp
)

# Create executable
add_executable(diary_manager ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(diary_manager PRIVATE include)

# Add compiler flags
if(MSVC)
    target_compile_options(diary_manager PRIVATE /W4)
else()
    target_compile_options(diary_manager PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Create data directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data) 