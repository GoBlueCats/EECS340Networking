cmake_minimum_required(VERSION 3.9)
project(Routesim_master)

set(CMAKE_CXX_STANDARD 11)

add_executable(Routesim_master
        2dmesh.topo
        alltoall.topo
        circular.topo
        context.cc
        context.h
        demo.event
        demo.topo
        edge.h
        error.h
        event.cc
        event.h
        eventqueue.cc
        eventqueue.h
        generate_simulation.py
        heap.cc
        linear.topo
        link.cc
        link.h
        Makefile
        messages.cc
        messages.h
        node.cc
        node.h
        random.topo
        README
        routesim.cc
        table.cc
        table.h
        topology.cc
        topology.h
        tree.topo)
