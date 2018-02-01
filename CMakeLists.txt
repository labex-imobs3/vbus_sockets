cmake_minimum_required(VERSION 2.8.3)
project(vbus_sockets)

add_definitions(-std=c++14)
find_package(catkin REQUIRED)

# Export headers and library
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
)

# Declare library
include_directories(include)
FILE(GLOB CppFiles src/*.cpp)
add_library(${PROJECT_NAME} ${CppFiles})

# Mark executables and/or libraries for installation
install(TARGETS
  ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Mark header files for installation
install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

