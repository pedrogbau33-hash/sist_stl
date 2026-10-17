#ifndef LIVRO_HPP
#define LIVRO_HPP

#include <string>

class Livro {
public:
    Livro(const std::string& titulo, const std::string& autor, const std::string& genero, int ordem);

    std::string titulo() const;
    std::string autor() const;
    std::string genero() const;
    bool lido() const;
    bool temNota() const;
    int nota() const;
    std::string comentario() const;
    int ordem() const;

    void definirLido(bool lido);
    void definirNota(int nota);
    void removerNota();
    void definirComentario(const std::string& comentario);
    void removerComentario();

    bool operator==(const Livro& outro) const;
    bool operator<(const Livro& outro) const;

private:
    std::string titulo_;
    std::string autor_;
    std::string genero_;
    bool lido_;
    bool temNota_;
    int nota_;
    std::string comentario_;
    int ordem_;
};

#endif
