cmake_minimum_required(VERSION 3.30)
project(train)

set(CMAKE_CXX_STANDARD 20)

# Find libxml2 package
find_package(LibXml2 REQUIRED)

# Include directories
include_directories(${LIBXML2_INCLUDE_DIR})

#add_definitions(-DMAP_XML_PATH=\"${MAP_XML}\")

# Define the executable target
add_executable(train
        main.cpp
        Train.cpp
        Train.h
        Trains.cpp
        Trains.h
        CommandHandler.cpp
        CommandHandler.h
        Graph.cpp
        Graph.h
)

# Link libxml2 library
target_link_libraries(train ${LIBXML2_LIBRARIES})

# Add this line to specify the source file location
set(USERNAMES_FILE ${CMAKE_SOURCE_DIR}/usernames.xml)

# Define map.xml file location
set(MAP_XML ${CMAKE_SOURCE_DIR}/map.xml)

# Add this line to specify the destination directory in the build folder
set(DESTINATION_DIR ${CMAKE_BINARY_DIR})

# Add this custom command to copy the usernames file to the build folder
add_custom_command(
        TARGET train POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${USERNAMES_FILE} ${DESTINATION_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${MAP_XML} ${DESTINATION_DIR}
        COMMENT "Copying usernames file to build folder"
)