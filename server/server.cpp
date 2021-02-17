/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "../src/data.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using data::DataRequest;
using data::DataResponse;
using data::DataService;
using data::Record;


// Logic and data behind the server's behavior.
class DataServiceImpl final : public DataService::Service {
	Status GetData(ServerContext* context, const DataRequest* request,
		DataResponse* reply) override {
		std::cout << "request recieved: " << request->name() << " " << request->num_records() << std::endl;
		for (int i = 0; i < request->num_records(); i++) {
			Record * record = reply->add_records();
			record->set_int_val(i);
			record->set_string_val(request->name());
			record->set_float_val((float)i * 3.142);
		}
		return Status::OK;
	}
	Status GetDataStream(ServerContext* context, const DataRequest* request, ServerWriter<Record>* writer) override {
		std::cout << "request recieved: " << request->name() << " " << request->num_records() << std::endl;
		Record record;
		for (int i = 0; i < request->num_records(); i++) {
			Record record;
			record.set_int_val(i);
			record.set_string_val(request->name());
			record.set_float_val((float)i * 3.142);
			writer->Write(record);
		}
		return Status::OK;
	}
};

void RunServer() {
	std::string server_address("0.0.0.0:50051");
	DataServiceImpl service;

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	// Finally assemble the server.
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}

int main(int argc, char** argv) {
	RunServer();

	return 0;
}