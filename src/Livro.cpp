#include "Livro.hpp"

#include "Texto.hpp"

Livro::Livro(const std::string& titulo, const std::string& autor, const std::string& genero, int ordem)
    : titulo_(Texto::aparar(titulo)),
      autor_(Texto::aparar(autor)),
      genero_(Texto::aparar(genero)),
      lido_(false),
      temNota_(false),
      nota_(-1),
      comentario_(""),
      ordem_(ordem) {
}

std::string Livro::titulo() const {
    return titulo_;
}

std::string Livro::autor() const {
    return autor_;
}

std::string Livro::genero() const {
    return genero_;
}

bool Livro::lido() const {
    return lido_;
}

bool Livro::temNota() const {
    return temNota_;
}

int Livro::nota() const {
    return nota_;
}

std::string Livro::comentario() const {
    return comentario_;
}

int Livro::ordem() const {
    return ordem_;
}

void Livro::definirLido(bool lido) {
    lido_ = lido;
}

void Livro::definirNota(int nota) {
    nota_ = nota;
    temNota_ = true;
}

void Livro::removerNota() {
    nota_ = -1;
    temNota_ = false;
}

void Livro::definirComentario(const std::string& comentario) {
    comentario_ = comentario;
}

void Livro::removerComentario() {
    comentario_ = "";
}

bool Livro::operator==(const Livro& outro) const {
    return Texto::normalizar(titulo_) == Texto::normalizar(outro.titulo_);
}

bool Livro::operator<(const Livro& outro) const {
    if (nota_ != outro.nota_) {
        return nota_ > outro.nota_;
    }
    return ordem_ > outro.ordem_;
}
