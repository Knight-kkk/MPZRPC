set(SRC_LIST provider.cpp ../service1.pb.cc)
add_executable(provider ${SRC_LIST})
target_link_libraries(provider mprpc protobuf pthread)

add_executable(zkClient zookeeper.cpp ../../src/zkutil.cpp)
target_link_libraries(zkClient mprpc zookeeper_mt)


