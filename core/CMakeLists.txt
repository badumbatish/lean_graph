add_executable(traveller src/traveller.cpp)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE include/)

add_executable(dummy src/dummy.cpp)
target_include_directories(dummy PRIVATE include/)
target_compile_options(dummy
    PUBLIC
    -Wall -Wextra -Wpedantic -Werror
)

