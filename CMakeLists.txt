######################################
#### PROJECT SETTINGS.
######################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(aabb_create)


######################################
#### SET SOURCE FILES.
######################################

file(GLOB SRC src/*.cpp)
include_directories("src")

######################################
############ MAKE EXECUTABLE
######################################

add_executable (aabb_create ${SRC})
