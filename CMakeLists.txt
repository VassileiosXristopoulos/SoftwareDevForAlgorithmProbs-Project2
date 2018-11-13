cmake_minimum_required(VERSION 3.5)
project(cluster)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(cluster
        "src/DataSetMap.cpp"
        "src/Item.cpp"
        "src/Util.cpp"
        "src/ClusterMaster.cpp"
        "src/Cluster.cpp"
        "src/cluster.cpp"
        "src/lsh/EucledianHashTable.cpp"
        "src/lsh/hashFunction.cpp"
        "src/lsh/HashNode.cpp"
        "src/lsh/cosineHashfunction.cpp"
        "src/lsh/AHashTable.cpp"
        "src/lsh/CosineHashTable.cpp"
        "src/hypercube/CosineHypercube.cpp"
        "src/hypercube/CubeEdge.cpp"
        "src/hypercube/BinaryMap.cpp"
        "src/hypercube/AHypercube.cpp"
        "src/hypercube/EucledianHypercube.cpp"
        "src/lsh/lsh.cpp"
        "src/rangeSearch_consts.cpp")

add_executable(cluster ${cluster})
