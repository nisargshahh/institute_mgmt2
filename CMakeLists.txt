cmake_minimum_required(VERSION 3.16)
project(institute_mgmt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 (or Qt5 if you installed Qt5)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Find SQLite3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Enable Qt's MOC (Meta-Object Compiler)
set(CMAKE_AUTOMOC ON)

# Create executable
add_executable(institute_mgmt institute_mgmt.cpp)

# Link Qt libraries
target_link_libraries(institute_mgmt Qt6::Core Qt6::Widgets)

target_link_libraries(institute_mgmt ${SQLITE3_LIBRARIES})
target_include_directories(institute_mgmt PRIVATE ${SQLITE3_INCLUDE_DIRS})
target_compile_options(institute_mgmt PRIVATE ${SQLITE3_CFLAGS_OTHER})