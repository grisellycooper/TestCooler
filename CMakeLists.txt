cmake_minimum_required(VERSION 3.1)
#set(CMAKE_CXX_STANDARD 11)
project(testcooler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# To find GLM
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(Threads REQUIRED)
find_package(GLFW3 REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)

# Add main file
file(GLOB MAIN_FILE "${CMAKE_SOURCE_DIR}/main.cpp")

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.cpp	
)

# Define the include DIRs
include_directories(
    ${CMAKE_SOURCE_DIR}/include    
    ${OPENGL_INCLUDE_DIR}    
    ${GLM_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIR}
)

# Define the executable
add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SOURCE_FILES})

# Define the link libraries
target_link_libraries(${PROJECT_NAME} 
	${GLFW_LIBRARY}
    ${OPENGL_LIBRARIES}         
)
