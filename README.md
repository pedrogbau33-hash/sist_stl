# Estante de Leituras

Atividade da Unidade 2 de Linguagem de Programação I. Leia `ENUNCIADO.md`
antes de começar.

Sistema em C++11 que registra livros lidos e por ler, com nota e comentário,   
e oferece consultas: livros de um autor, gêneros mais lidos e livros mais bem
avaliados. A descrição completa está em `DESCRICAO.md`.

## Arquivos

```text
.
├── AGENTS.md       regras que a IA lê antes de cada pedido
├── DESCRICAO.md    a descrição do sistema, escrita por você
├── ENUNCIADO.md    a atividade
├── PLANO.md        o plano feito com a IA (criado na etapa 4)
├── README.md       este arquivo
├── exemplos/       arquivos de entrada para testar
├── include/        um .hpp por classe
└── src/            um .cpp por classe, mais a interface
```

## Compilar e executar

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
./sistema
```

Para rodar os exemplos:

```bash
./sistema < exemplos/01_fluxo_principal.txt
./sistema < exemplos/02_campos_duplicado_inexistente.txt
./sistema < exemplos/03_leitura_e_avaliacao.txt
./sistema < exemplos/04_consultas_sem_resultado.txt
./sistema < exemplos/05_entradas_invalidas.txt
```

Se o comando mudar, atualize esta seção. Na correção eu vou usar o que
estiver escrito aqui.

## Onde uso a STL

Escreva esta seção sem IA. Para cada container ou algoritmo da STL que o sistema usa: em que classe ele está, o que guarda, e por que você escolheu ele e não outro.

1. Classe: Estante (campo livros)
O que guarda: std::list<Livro>
Por quê: Escolhi a lista para guardar os livros na ordem em que foram cadastrados.
2. Classe: Estante::buscar
O que guarda: std::find
Por quê: Para procurar um livro pelo título. A busca percorre a lista e compara os títulos usando o operator==, que ignora diferenças entre letras maiúsculas e minúsculas e espaços nas pontas.
3. Classe: Estante::generosMaisLidos
O que guarda: std::map<std::string, GeneroEstatistica>
Por quê: Para reunir os livros do mesmo gênero, mesmo quando o nome foi escrito com diferenças entre maiúsculas e minúsculas. Ele guarda a grafia do gênero e a quantidade de livros lidos.
4. Classe: Estante (consultas)
O que guarda: std::vector<Livro> e std::vector<GeneroEstatistica>
Por quê: Uso vetores para reunir os resultados das consultas. Eles facilitam a organização dos dados e permitem ordenar os livros quando necessário.
5. Classe: Estante::livrosMaisBemAvaliados
O que guarda: std::sort
Por quê: Para colocar os livros com as maiores notas primeiro. Quando dois livros têm a mesma nota, o mais recentemente cadastrado aparece antes.
6. Classe: Livro
O que guarda: Livro::operator==
Por quê: Define quando dois livros são considerados iguais: quando seus títulos correspondem após a normalização. Isso permite que o std::find encontre o livro corretamente.
7. Classe: Livro
O que guarda: Livro::operator<
Por quê: Define a ordem dos livros na classificação: primeiro os de maior nota e, em caso de empate, os cadastrados mais recentemente. O std::sort usa essa regra.

## Como usei a IA

Escreva com as suas palavras:

- Modelo usado: OpenCode DeepSeek V4.1
- Um pedido que deu certo, e por que você acha que deu certo: Pedi um plano de implementação antes de começar a programar e trabalhei uma etapa por vez. Isso ajudou a organizar o desenvolvimento, separar as funcionalidades e verificar cada etapa antes de prosseguir. O plano foi registrado em um commit antes da implementação, e as etapas seguintes também foram registradas em commits separados.
- Um pedido que deu errado, o que veio errado e como você mudou o pedido: Não identifiquei um pedido que tenha dado errado durante o desenvolvimento.
- Uma coisa que a IA sugeriu e você recusou, e por quê: Recusei a ideia de exigir que todo livro com nota tivesse um comentário e vice-versa. Considerei essa regra limitante e pouco prática, pois o usuário deve poder atribuir uma nota sem escrever um comentário, escrever um comentário sem atribuir uma nota ou fornecer ambos.
