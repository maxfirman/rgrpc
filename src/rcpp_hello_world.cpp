#include <grpcpp/grpcpp.h>


// [[Rcpp::export]]
int rcpp_hello_world() {
    
    // This call to create channel explodes.
    grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    
    return 0;
}
