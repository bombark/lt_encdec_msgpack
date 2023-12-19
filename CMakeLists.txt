cmake_minimum_required(VERSION 3.0)
project(enc_msgpack)

add_library(enc_msgpack SHARED ./src/enc_msgpack.c)
target_link_libraries(enc_msgpack msgpackc lt_api)
target_compile_options(enc_msgpack PUBLIC -O1)
install(TARGETS enc_msgpack DESTINATION lib)

add_library(dec_msgpack SHARED ./src/dec_msgpack.c)
target_link_libraries(dec_msgpack msgpackc lt_api)
target_compile_options(dec_msgpack PUBLIC -O1)
install(TARGETS dec_msgpack DESTINATION lib)

# test
enable_testing()
add_subdirectory(./tests)