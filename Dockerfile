FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y r-base libgrpc6 libgrpc-dev libgrpc++-dev protobuf-compiler protobuf-compiler-grpc

RUN R -e 'install.packages("Rcpp", dependencies = TRUE)'

COPY . /rgrpc

RUN cd rgrpc/src && protoc -I=. --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin data.proto

RUN which Rscript

RUN Rscript -e 'Rcpp::compileAttributes(pkgdir = "rgrpc")' && R CMD build && R CMD INSTALL --no-multiarch rgrpc

RUN cd rgrpc/server && \
  g++ -std=gnu++11 -DNDEBUG `pkg-config --cflags grpc++ protobuf` -fpic  -g -O2 -g  -c server.cpp -o server.o && \
  g++ -std=gnu++11 -o server.out server.o ../src/data.grpc.pb.o ../src/data.pb.o -lgrpc++ -lprotobuf -pthread -lgrpc++_reflection

CMD rgrpc/run.sh
