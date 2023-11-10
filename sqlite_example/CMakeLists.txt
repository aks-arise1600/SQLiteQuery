cmake_minimum_required(VERSION 3.10.0)

project(sqlite_example
  VERSION
    1.0
  DESCRIPTION
    "sqlite example"
  LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5Core REQUIRED)

set(SRC_FILES
		main.cpp
		)	
		

set (LIB_PATH "${CMAKE_SOURCE_DIR}/../../lib")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} SQLiteQuery)
add_definitions(-DQT_NO_VERSION_TAGGING)

qt5_use_modules(${PROJECT_NAME} Core Sql )

include_directories(. ../inc)


