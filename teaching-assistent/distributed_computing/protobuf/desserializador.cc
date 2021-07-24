#include <iostream>
#include <fstream>
#include <string>

#include "src-gen/person.pb.h"

int deserialize_do_arquivo(std::string arquivo) {
    Person p;

    std::ifstream istream;
    istream.open(arquivo);
    bool success = p.ParsePartialFromIstream(&istream);
    if (!success) {
        std::cout << "Failed to deserialize" << std::endl;
        return 1;
    }
    istream.close();

    std::cout << "User name: " << p.user_name() <<
        ", favorite number: "  << p.favorite_number() << std::endl;

    std::cout << "Deserializado do arquivo '" << arquivo << "'" << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./deserializador arquivo" << std::endl;
        return 1;
    }
    return deserialize_do_arquivo(argv[1]);
}
