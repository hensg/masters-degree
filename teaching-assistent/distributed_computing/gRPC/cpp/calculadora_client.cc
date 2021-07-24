#include <grpcpp/security/credentials.h>
#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>

#include "src-gen/calculadora.pb.h"
#include "src-gen/calculadora.grpc.pb.h"

class CalculadoraClient {
    public:
        CalculadoraClient(std::shared_ptr<grpc::Channel> channel)
            : stub_(Calculadora::NewStub(channel)) {}

        double calcule(double a, double b, std::string op) {
            Parametros parametros;
            parametros.set_a(a);
            parametros.set_b(b);
            parametros.set_operacao(op);

            grpc::ClientContext context;

            Resultado resultado;

            grpc::Status status = stub_->Calcule(&context, parametros, &resultado);

            if (status.ok())
                return resultado.valor();
            else {
                std::cerr << "Failed, error code = " << status.error_code() <<
                    ", message=" << status.error_message() << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    private:
        std::unique_ptr<Calculadora::Stub> stub_;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << 
            "Wrong number of parameters, " <<
            "usage: ./calculadora_client <server_address>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string server_addr = argv[1]; 

    CalculadoraClient client(
            grpc::CreateChannel(
                server_addr, grpc::InsecureChannelCredentials()));

    double resultado = client.calcule(99, 1, "+");
    std::cout << "99+1=" << resultado << std::endl;
    return 0;
}
