add_executable(test_enc_msgpack ./test_enc_msgpack.c)
target_link_libraries(test_enc_msgpack enc_msgpack)
add_test(NAME test_enc_msgpack COMMAND test_enc_msgpack)

add_executable(test_dec_msgpack ./test_dec_msgpack.c)
target_link_libraries(test_dec_msgpack dec_msgpack)
add_test(NAME test_dec_msgpack COMMAND test_dec_msgpack)