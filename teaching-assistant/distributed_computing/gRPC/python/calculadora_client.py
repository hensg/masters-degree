import logging
import grpc

import calculadora_pb2
import calculadora_pb2_grpc

def run():
    with grpc.insecure_channel('localhost:3030') as channel:
        stub = calculadora_pb2_grpc.CalculadoraStub(channel)
        response = stub.Calcule(calculadora_pb2.Parametros(a=99, b=1, operacao="+"))
        print("99+1=" + str(response.valor))

if __name__ == '__main__':
    logging.basicConfig()
    run()
