#!/bin/sh
protoc -I=. --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=$(where grpc_cpp_plugin) data.proto