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

Listar livros na estante: mostra os livros cadastrados, com título, autor, gênero, status, nota e comentário, na ordem de cadastro, do mais antigo para o mais recente.

Adicionar ou editar nota e comentário: pede o título do livro e permite alterar a nota e o comentário independentemente. O sistema registra as alterações e informa se a operação foi realizada. Só é permitido adicionar ou editar nota e comentário de livros marcados como lidos.

Mostrar livros de determinado autor: pede o nome do autor e mostra os livros cadastrados que correspondem a ele.

Mostrar gêneros mais lidos: mostra os gêneros organizados pela quantidade de livros lidos em cada um, considerando somente livros com status "lido". Em caso de empate, os gêneros são ordenados alfabeticamente.

Mostrar livros mais bem avaliados: mostra todos os livros lidos que possuem nota, ordenados da maior nota para a menor. Em caso de empate, o livro cadastrado mais recentemente aparece primeiro.

Remover livro: pede o título do livro e o remove da estante, informando se a remoção foi realizada.

Encerrar programa: encerra a execução do sistema.

## Regras

Não é permitido haver dois livros com o mesmo título simultaneamente na estante. Para verificar duplicatas e localizar livros pelo título, são ignoradas diferenças entre letras maiúsculas e minúsculas e espaços no começo e no fim.

As buscas por autor também ignoram diferenças entre letras maiúsculas e minúsculas e espaços no começo e no fim.

Os gêneros são agrupados sem diferenciar letras maiúsculas e minúsculas. A grafia exibida será a do livro mais antigo, na ordem de cadastro atual, entre os livros daquele gênero. Se esse livro for removido, será utilizada a grafia do livro mais antigo restante do mesmo gênero.

Ao cadastrar título, autor e gênero, os espaços no começo e no fim são removidos, mas os espaços internos são preservados.

Os campos título, autor e gênero são obrigatórios e não podem ficar vazios.

Um livro removido pode ser cadastrado novamente. Nesse caso, o novo cadastro será considerado o mais recente.

O título não pode ser editado depois do cadastro. Para corrigir um título, o usuário deve remover o livro e cadastrá-lo novamente.

Todo livro novo começa com o status "por ler", sem nota e sem comentário.

O status aceita variações entre letras maiúsculas e minúsculas, mas deve ser exibido sempre como "lido" ou "por ler".

A nota deve ser um número inteiro entre 0 e 10.

Somente livros marcados como lidos podem receber nota e comentário. Um livro pode estar marcado como lido sem possuir nota ou comentário.

Nota e comentário podem ser adicionados, alterados e removidos independentemente. Durante a edição, digitar "-" mantém o valor atual daquele campo, um valor válido adiciona ou substitui o valor, e deixar o campo vazio remove o valor existente. O caractere "-" fica reservado para manter o valor atual, portanto não pode ser cadastrado como comentário exatamente igual a "-".

Se qualquer campo da edição contiver uma entrada inválida, toda a operação será cancelada, sem alterar a nota ou o comentário.

Ao mudar o status de "lido" para "por ler", a nota e o comentário são apagados.

A consulta de gêneros mais lidos considera somente livros marcados como lidos e não mostra gêneros sem livros lidos. Os gêneros são ordenados pela quantidade de livros lidos, da maior para a menor; em caso de empate, são ordenados alfabeticamente.

A consulta dos livros mais bem avaliados considera somente livros lidos que possuem nota. As notas são ordenadas da maior para a menor; em caso de empate, tem preferência o livro cadastrado mais recentemente.


## Quando algo dá errado

Ao procurar um livro que não existe: "Parece que esse livro nao existe."

Ao consultar um autor sem livros cadastrados: "Parece que esse autor nao possui livros cadastrados."

Ao realizar uma operação relacionada aos livros quando a estante está vazia: "A estante esta vazia."

Ao informar uma nota inválida: "Nota invalida, tente um numero entre 0 e 10."

Ao tentar avaliar um livro que ainda não foi lido: "Voce precisa ler esse livro primeiro."

Ao escolher uma opção inexistente no menu ou informar um status inválido: "Essa opcao nao existe."

Ao tentar cadastrar um livro que já existe na estante: "Parece que voce ja cadastrou esse livro."

Ao deixar vazio um campo obrigatório durante o cadastro: "Preencha todos os campos obrigatorios."

Ao consultar os gêneros mais lidos quando não há livros marcados como lidos: "Ainda nao ha livros lidos na estante."

Ao consultar os livros mais bem avaliados quando a estante está vazia: "A estante esta vazia."

Ao consultar os livros mais bem avaliados quando há livros cadastrados, mas nenhum livro lido possui nota: "Ainda nao ha livros avaliados na estante."

Se uma entrada inválida for informada durante a edição da nota e do comentário, a operação inteira será cancelada e nenhum dos dois campos será alterado.


## Classes que eu imagino

Livro: guarda as informações de cada livro, como título, autor, gênero, status de leitura, nota e comentário.

Estante: guarda a coleção de livros e permite cadastrar, remover, listar e consultar os livros, além de calcular as estatísticas de leitura.

Interface: apresenta o menu, recebe as entradas do usuário e mostra os resultados e as mensagens do sistema, mantendo a interação separada das regras de gerenciamento dos livros.


## Exemplo de uso

1. O sistema apresenta o menu e o usuário escolhe cadastrar um livro.
2. O sistema solicita o título, o autor e o gênero.
3. O usuário informa os dados.
4. O sistema cadastra o livro com status "por ler", sem nota e sem comentário, e informa que o cadastro foi realizado.
5. O usuário escolhe marcar o livro como lido e informa o título e o novo status.
6. O sistema atualiza o status e informa o resultado.
7. O usuário escolhe adicionar ou editar nota e comentário e informa o título do livro.
8. O usuário informa a nota e o comentário, ou utiliza as opções de manter ou remover os valores existentes.
9. O sistema registra as alterações válidas.
10. O usuário consulta os livros mais bem avaliados e o sistema mostra os livros lidos que possuem nota, ordenados da maior para a menor, com os mais recentemente cadastrados aparecendo primeiro em caso de empate.

