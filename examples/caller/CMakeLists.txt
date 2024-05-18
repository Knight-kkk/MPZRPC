set(SRC_LIST consumer.cpp ../service1.pb.cc)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf pthread)

