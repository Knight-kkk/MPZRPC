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

If you see these message above, congratulations, the building process is done successfully.
```
[ 50%] Built target mprpc
[ 66%] Built target consumer
[ 83%] Built target provider
[100%] Built target zkClient
```

# 💻 Usage
 Run zookeeper with `/path/to/zookeeper/zkServer.sh start`.
 
 Run RPC provider with `/path/to/MPZRPC/bin/provider -c test.conf`.
 
 Run RPC consumer with `/path/to/MPZRPC/bin/consumer -c test.conf`.

# 🔨 DIY
The `test.conf` above in Section [Usage](#Usage) stores the IP and port information of RPC provider server and ZooKeeper server, its content is as below:
```
# You would better not modify the field names otherwise you will need to modify the corresponding code in ../src/
ProviderIP = 127.0.0.1
ProviderPort = 6666
ZooKeeperIP = 127.0.0.1
# 2181 is the default running port of ZooKeeper
ZooKeeperPort = 2181
```
If you run the provider or(and) ZooKeeper in other IP or(and) Port, you should modify the filed above.






