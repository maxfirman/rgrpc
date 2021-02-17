#!/bin/sh
./rgrpc/server/server.out &
R -e 'library("rgrpc");rgrpc::get_data("hello");rgrpc::get_data_stream("hello stream", 1000000)'