#include <iostream>

#include "Livro.hpp"
#include "Texto.hpp"

int main() {
    std::cout << std::boolalpha;

    Livro a("  Dom Casmurro  ", "Machado de Assis", "Romance", 1);
    Livro b("Capitaes da Areia", "Jorge Amado", "Romance", 2);

    std::cout << "Livro a -> titulo=[" << a.titulo() << "] autor=[" << a.autor()
              << "] genero=[" << a.genero() << "] lido=" << a.lido()
              << " temNota=" << a.temNota() << " nota=" << a.nota()
              << " comentario=[" << a.comentario() << "] ordem=" << a.ordem() << "\n";
    std::cout << "Livro b -> titulo=[" << b.titulo() << "] autor=[" << b.autor()
              << "] genero=[" << b.genero() << "] lido=" << b.lido()
              << " temNota=" << b.temNota() << " nota=" << b.nota()
              << " comentario=[" << b.comentario() << "] ordem=" << b.ordem() << "\n";

    std::cout << "normalizar([  DoM   Casmurro  ])=[" << Texto::normalizar("  DoM   Casmurro  ") << "]\n";
    std::cout << "aparar([  oi  ])=[" << Texto::aparar("  oi  ") << "]\n";

    Livro mesmoTitulo("DOM CASMURRO", "Outro Autor", "Outro Genero", 3);
    std::cout << "a == mesmoTitulo -> " << (a == mesmoTitulo) << "\n";
    std::cout << "a == b -> " << (a == b) << "\n";

    a.definirLido(true);
    a.definirNota(8);
    b.definirLido(true);
    b.definirNota(8);
    std::cout << "mesma nota, a ordem=1 e b ordem=2 -> a<b: " << (a < b) << " | b<a: " << (b < a) << "\n";

    b.definirNota(9);
    std::cout << "b com nota 9 -> a<b: " << (a < b) << " | b<a: " << (b < a) << "\n";
    std::cout << "a<a: " << (a < a) << "\n";

    a.removerNota();
    a.definirComentario("Muito bom");
    std::cout << "a apos removerNota -> temNota=" << a.temNota() << " nota=" << a.nota()
              << " comentario=[" << a.comentario() << "]\n";

    return 0;
}
