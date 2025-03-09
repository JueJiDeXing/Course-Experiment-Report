cmake_minimum_required(VERSION 3.28)
project(Op)

set(CMAKE_CXX_STANDARD 17)


add_executable(ex3_rank ex3/ex3_rank.cpp
        Utils/ColorPrint.h)
add_executable(ex2_rw ex2/ex2_rw.cpp)
add_executable(ex2_pc ex2/ex2_pc.cpp)
add_executable(ex4 ex4/ex4.cpp
        ex4/PCB.h
        ex4/Controller/controller.h
        ex4/Controller/FCFS.h
        ex4/Controller/NPSJF.h
        ex4/Controller/PSJF.h
        ex4/Controller/HRRN.h
        ex4/Controller/RR.h
        ex4/Util.h
        ex4/Controller/PP.h
        ex4/Controller/NPP.h
        ex4/Recorder.h
        ex4/Recorder.cpp
)
add_executable(T test.cpp)
add_executable(Ex5 ex5/ex5.cpp
        ex5/LinkedList.h
        ex5/LRU.h
        ex5/FIFO.h
        ex5/LFU.h
        ex5/PageAlgorithm.h
        ex5/mathematical.h)

add_executable(Ex6 ex6/ex6.cpp
        ex6/Folder.h
        ex6/Ex6Util.h
        ex6/FileSys.h
        ex6/SpaceSys.h
        ex6/SpaceSys.cpp
)
