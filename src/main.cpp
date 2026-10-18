#include "Estante.hpp"
#include "Interface.hpp"

int main() {
    Estante estante;
    Interface interface(estante);
    interface.executar();
    return 0;
}
