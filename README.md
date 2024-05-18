         ___  ___   _____   ______  _____    _____   _____  
        /   |/   | |  _  \ |___  / |  _  \  |  _  \ /  ___| 
       / /|   /| | | |_| |    / /  | |_| |  | |_| | | |     
      / / |__/ | | |  ___/   / /   |  _  /  |  ___/ | |     
     / /       | | | |      / /__  | | \ \  | |     | |___  
    /_/        |_| |_|     /_____| |_|  \_\ |_|     \_____| 

MPZRPC: A Muduo&Protobuf&ZooKeeper-based RPC framework.

# 📑 Overview
MPZRPC is a RPC framework based-on [Muduo](https://github.com/chenshuo/muduo.git), [Google-Protobuf](https://github.com/protocolbuffers/protobuf.git) and [ZooKeeper](https://github.com/apache/zookeeper.git). It employs Muduo as network communication component, employs Protobuf as serialization and deserialization component of RPC request and response, employs ZooKeeper as middleware to store information of RPC provider, aka. callee. 

# 🛠️ Environment configuration & Build
As mentioned above, this project is based-on Muduo, Protobuf and ZooKeeper, you can follow these tutorials below to configure your basic environment.
- [how to use Muduo in your project?](https://github.com/chenshuo/muduo-tutorial.git)
- [how to use Protobuf in your project?](https://github.com/protocolbuffers/protobuf.git)
- [how to use ZooKeeper in your project?](https://github.com/apache/zookeeper.git)

Then, clone MPZRPC with `git clone https://github.com/Knight-kkk/MPZRPC.git`.

Grant execution permission to automatic building shell script `autobuild.sh` with `chmod 777 autobuild.sh` and run `./autobuild.sh`.





