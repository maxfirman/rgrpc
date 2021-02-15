#include <grpcpp/grpcpp.h>
#include "data.grpc.pb.h"
#include <Rcpp.h>

using data::DataRequest;
using data::DataResponse;
using data::DataService;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using namespace Rcpp;

class DataClient
{
public:
	DataClient(std::shared_ptr<Channel> channel)
		: stub_(DataService::NewStub(channel)) {}

	// Assembles the client's payload, sends it and presents the response back
	// from the server.
	DataFrame GetData(const std::string &dataset_id)
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
			IntegerVector int_col;
			NumericVector float_col;
			CharacterVector string_col;

			for (auto record : reply.records())
			{
				int_col.push_back(record.int_val());
				float_col.push_back(record.float_val());
				string_col.push_back(record.string_val());
				//std::cout << record.int_val() << " " << record.float_val() << " " << record.string_val() << std::endl;
			}

			return DataFrame::create(
				Named("int_col") = int_col,
				Named("float_col") = float_col,
				Named("string_col") = string_col);
		}
		else
		{
			std::cout << status.error_code() << ": " << status.error_message() << std::endl;
			std::cout << "RPC failed" << std::endl;
			return DataFrame::create();
		}
	}

private:
	std::unique_ptr<DataService::Stub> stub_;
};

// [[Rcpp::export]]
DataFrame get_data(std::string &dataset_id)
{
	// This call to create channel explodes in windows.
	DataClient data_service(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
	return data_service.GetData(dataset_id);
}
