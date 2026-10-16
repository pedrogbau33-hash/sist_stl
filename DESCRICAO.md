# Descrição do sistema

Escreva este arquivo sem IA, antes de qualquer código. Apague as instruções
em itálico quando preencher.

## Nome e ideia

O sistema será uma Estante de Leituras, permitindo que o usuário organize livros lidos e por ler. Ele também permite acompanhar as avaliações das leituras e consultar estatísticas sobre os gêneros mais lidos e os livros mais bem avaliados.

## O que o sistema guarda

Livros, contendo:
Título
Autor
Gênero
Status de leitura (lido ou por ler)
Nota (número inteiro de 0 a 10, podendo estar vazia)
Comentário (pode estar vazio)

## O que dá para fazer

Cadastrar livro: pede o título, o autor e o gênero. O sistema cadastra o livro com o status "por ler", sem nota e sem comentário, e informa se o cadastro foi realizado.

Marcar livro como lido ou por ler: pede o título do livro e o novo status. O sistema atualiza o status e informa o resultado. Se o livro passar de "lido" para "por ler", sua nota e seu comentário serão apagados.

Listar livros na estante: mostra os livros cadastrados, com título, autor, gênero, status, nota e comentário.

Adicionar ou editar nota e comentário: pede o título do livro, a nota e o comentário. O sistema registra ou atualiza a avaliação e informa se a operação foi realizada. Só é permitido avaliar livros marcados como lidos.

Mostrar livros de determinado autor: pede o nome do autor e mostra os livros cadastrados que correspondem a ele.

Mostrar gêneros mais lidos: mostra os gêneros organizados pela quantidade de livros lidos em cada um, considerando somente livros com status "lido".

Mostrar livros mais bem avaliados: mostra todos os livros lidos que possuem nota, ordenados da maior nota para a menor. Em caso de empate, o livro cadastrado mais recentemente aparece primeiro.

Remover livro: pede o título do livro e o remove da estante, informando se a remoção foi realizada.

Encerrar programa: encerra a execução do sistema.

## Regras

Não é permitido haver dois livros com o mesmo título simultaneamente na estante, independentemente de diferenças entre letras maiúsculas e minúsculas.  

Um livro removido pode ser cadastrado novamente.

Todo livro novo começa com o status "por ler", sem nota e sem comentário.

A nota deve ser um número inteiro entre 0 e 10.

Somente livros marcados como lidos podem receber nota e comentário.

Ao mudar o status de "lido" para "por ler", a nota e o comentário são apagados.

Livros sem comentário podem permanecer na estante.

A consulta de gêneros mais lidos considera somente livros marcados como lidos.

A consulta dos livros mais bem avaliados considera somente livros lidos que possuem nota.

Em caso de empate na nota, tem preferência o livro cadastrado mais recentemente.

Os gêneros mais lidos serão ordenados pela quantidade de livros lidos, da maior para a menor. Em caso de empate, serão ordenados alfabeticamente.


## Quando algo dá errado

Ao procurar um livro que não existe: "Parece que esse livro nao existe."

Ao consultar um autor sem livros cadastrados: "Parece que esse autor nao possui livros cadastrados."

Ao mostrar informações sobre os livros quando a estante está vazia: "A estante esta vazia."

Ao informar uma nota inválida: "Nota invalida, tente um numero entre 0 e 10."

Ao tentar avaliar um livro que ainda não foi lido: "Voce precisa ler esse livro primeiro."

Ao escolher uma opção inexistente no menu: "Essa opcao nao existe."

Ao tentar cadastrar um livro que já existe na estante: "Parece que voce ja cadastrou esse livro."

Ao tentar mostrar os livros mais bem avaliados quando nenhum livro possui nota: informar que não há livros avaliados.


## Classes que eu imagino

Livro: guarda as informações de cada livro, como título, autor, gênero, status de leitura, nota e comentário.

Estante: guarda a coleção de livros e permite cadastrar, remover, listar e consultar os livros, além de calcular as estatísticas de leitura.


## Exemplo de uso

1. O sistema pergunta: "Deseja adicionar um livro a sua biblioteca?" e o usuário escolhe "Sim" ou "Não".
2. O usuário informa o título do livro.
3. O sistema pergunta o nome do autor.
4. O usuário informa o autor.
5. O sistema pergunta o gênero do livro.
6. O usuário informa o gênero.
7. O sistema cadastra o livro com status "por ler" e informa que o cadastro foi realizado.
8. O usuário escolhe marcar o livro como lido.
9. O sistema solicita a nota e o comentário e registra a avaliação.
10. O usuário consulta os livros mais bem avaliados e o sistema mostra os livros lidos com nota, ordenados da maior para a menor.

