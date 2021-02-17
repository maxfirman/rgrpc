#!/bin/sh
g++ -std=gnu++11 -D_WIN32_WINNT=0x600 -DNDEBUG `pkg-config --cflags grpc++ protobuf` -fpic  -g -O2 -g  -c server.cpp -o server.o
g++ -std=gnu++11 -o server.exe server.o ../src/data.grpc.pb.o ../src/data.pb.o `pkg-config --libs grpc++ protobuf` -lgrpc++_reflection -lcrypt32 -lWs2_32 -ldbghelp