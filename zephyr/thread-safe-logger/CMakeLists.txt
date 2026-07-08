cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(thread_safe_logger)

target_sources(app PRIVATE
    src/main.c
    src/logger.c

)

target_include_directories(app PRIVATE
    includes
)