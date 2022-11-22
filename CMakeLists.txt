cmake_minimum_required(VERSION 3.5)
project(logger C)

set(LOGGER_VERSION 1.0.0)
add_library(${PROJECT_NAME} logger.c)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "logger.h")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LOGGER_VERSION})