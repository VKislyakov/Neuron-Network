cmake_minimum_required(VERSION 2.8)

project(nnSys)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB DataSet.cpp
                Layer.cpp
                moduleSVD.cpp
                Net.cpp
                Neuron.cpp
                SpectralClassifier.cpp)

add_library(nnLib STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main nnLib)