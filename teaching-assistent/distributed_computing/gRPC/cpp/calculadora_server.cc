#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status_code_enum.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "src-gen/calculadora.pb.h"
#include "src-gen/calculadora.grpc.pb.h"

class CalculadoraImpl final : public Calculadora::Service {
    public:
        virtual ::grpc::Status Calcule(::grpc::ServerContext* context, const ::Parametros* parametros, ::Resultado* resultado) {
            std::cout << "Atendendo requisição de cliente..." << std::endl;

            switch (parametros->operacao()[0]) {
                case '+':
                    resultado->set_valor(parametros->a() + parametros->b());
                    return grpc::Status::OK;
                default:
                    return grpc::Status(grpc::UNIMPLEMENTED, "Operação ainda não implementada.");
            }
        }
};

void RunServer() {
    std::string server_address("0.0.0.0:3030");
    CalculadoraImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
};

int main(int argc, char* argv[]) {
    RunServer();
    return 0;
}
