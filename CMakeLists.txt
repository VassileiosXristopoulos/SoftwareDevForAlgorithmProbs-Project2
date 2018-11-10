cmake_minimum_required(VERSION 3.5)
project(cluster)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(cluster
        "src/DataSetMap.cpp"
        "src/Item.cpp"
        "src/Util.cpp"
        "src/ClusterMaster.cpp"
        "src/Cluster.cpp"
        "src/cluster.cpp")

add_executable(cluster ${cluster})
