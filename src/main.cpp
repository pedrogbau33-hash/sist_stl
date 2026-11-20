#include "Arquivo.hpp"
#include "Estante.hpp"
#include "Interface.hpp"

int main() {
    Estante estante;
    Arquivo arquivo("estante.txt");
    Interface interface(estante, arquivo);

    interface.executar();
    return 0;
}
