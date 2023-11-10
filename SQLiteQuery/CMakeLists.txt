cmake_minimum_required(VERSION 3.1.0)

project(SQLiteQuery
  VERSION
    1.0
  DESCRIPTION
    "SQLiteQuery libraries"
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
		SQLiteQuery.cpp

		)

set(HEADER_FILES
		SQLiteQuery.h
		)
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})

#add_library(${PROJECT_NAME} STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

#add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

add_definitions(-DQT_NO_VERSION_TAGGING)

#set (LIB_PATH "${CMAKE_SOURCE_DIR}/lib")

#link_directories(${LIB_PATH})

#message(STATUS ${LIB_PATH})

qt5_use_modules(${PROJECT_NAME} Core Sql)

include_directories(.)

add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/libSQLiteQuery.a
                ${CMAKE_CURRENT_BINARY_DIR}/../../lib/libSQLiteQuery.a
                )
add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/../../SQLiteQuery/SQLiteQuery.h
                ${CMAKE_CURRENT_BINARY_DIR}/../../inc/SQLiteQuery.h
                )


