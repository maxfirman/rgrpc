#include <grpcpp/grpcpp.h>
#include "data.grpc.pb.h"

using data::DataRequest;
using data::DataResponse;
using data::DataService;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class DataClient
{
public:
	DataClient(std::shared_ptr<Channel> channel)
		: stub_(DataService::NewStub(channel)) {}

	// Assembles the client's payload, sends it and presents the response back
	// from the server.
	void GetData(const std::string &dataset_id)
	{

		// Data we are sending to the server.
		DataRequest request;
		request.set_name(dataset_id);

		// Container for the data we expect from the server.
		DataResponse reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
		Status status = stub_->GetData(&context, request, &reply);

		// Act upon its status.
		if (status.ok())
		{
			std::cout << "RPC Suceeded! Recieved:" << std::endl;
			for (auto record : reply.records())
				std::cout << record.int_val() << " " << record.float_val() << " " << record.string_val() << std::endl;
		}
		else
		{
			std::cout << status.error_code() << ": " << status.error_message()
				<< std::endl;
			std::cout << "RPC failed" << std::endl;
		}
	}

private:
	std::unique_ptr<DataService::Stub> stub_;
};

// [[Rcpp::export]]
int rcpp_hello_world() {
    
    // This call to create channel explodes.
    DataClient data_service(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    data_service.GetData("hello");

    return 0;
}
