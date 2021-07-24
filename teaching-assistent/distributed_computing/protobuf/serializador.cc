#include <iostream>
#include <fstream>
#include <string>

#include "src-gen/person.pb.h"

int serialize_para_arquivo(std::string arquivo) {
    Person p;
    p.set_user_name("joao");
    p.set_favorite_number(11);
    p.add_interests("daydreaming");
    p.add_interests("hacking");

    std::cout << "User name: " << p.user_name() <<
        ", favorite number: "  << p.favorite_number() << std::endl;

    std::ofstream ostream;
    ostream.open(arquivo);
    bool success = p.SerializeToOstream(&ostream);
    if (!success) {
        std::cout << "Failed to serialize" << std::endl;
        return 1;
    }
    ostream.close();
    std::cout << "Serializado para o arquivo '" << arquivo << "'" << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./serializador arquivo" << std::endl;
        return 1;
    }
    return serialize_para_arquivo(argv[1]);
}
