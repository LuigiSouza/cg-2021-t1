<h1>Trabalho 1 – Editor de Figuras Vetoriais</h1>

<h3> Descrição </h3>

Desenvolva um programa em C++ para fazer manipulações figuras vetoriais, algo semelhante às
ferramentas que o Microsoft Word oferece.

O programa deve permitir:

- Inserir/excluir figura.
- Cor da figura.
- Preenchimento da figura (Bônus).
- Editar tamanho da figura.
- Editar orientação da figura (giros de 90 graus).
- Enviar para frente/traz.
- Salvar em arquivo e carregar de arquivo.

Extras:

- [x] (+0,5) Sinalizar qual figura está selecionada.
- [x] (+1,0) Rotacionar figura em qualquer ângulo.
- [ ] (+1,5) Concatenar duas figuras (se tornando uma só) = agrupar.
- [x] (+1,0) Permitir inserir polígonos quaisquer.
- [ ] (+2,0) Preencher a figura qualquer (circular, polígono, triangulo, etc) com uma imagem
      (BMP).
- [ ] (+1,0) Painel mostrando uma label para cada figura presente na imagem.
- [ ] (+1,0) Checkbox permitindo deixar determinada figura invisível sem removê-la.

<h2> Getting Started </h2>

Toda a utilização do programa pode ser feita com o mouse, porém, existem teclas de atalhos com as mesmas funcionalidades caso haja preferência.

Para iniciar, compile o programa com a ferramenta CodeBlocks (Release 17.12). Por padrão, o programa irá procurar pelo arquivo `figuras.gr` a fim de carregar figuras pré definidas. Caso não haja tal arquivo, o programa se iniciará com uma tela vazia.

# Usabilidade

Ao iniciar o programa, o usuário poderá ver tanto as figuras as quais se estava editando anteriormente como um painel de edição do programa, com opções como adicionar, deletar e preencher figuras como mostrado na figura abaixo.

<p align="center"> 
  <img src="media/exemplo1.png" alt="canvas"/>
</p>

# Painel Principal

## Adicionar Imagens

Na parte esquerda do painel, se encontrará quatro botões, responsáveis por adicionar as figuras à tela. Respectivamente, os botões adicionam: quadrados, triângulos equiláteros, círculo e polígos de quantos lados quiser.

Para adicionar figuras simples (quadrados, triângulos e círculo), basta pressionar o botão com o desenho desejado. Uma vez selecionado, um círculo verde aparecerá em volta do cursor, mostrando que uma figura foi selecionada e sua cor será verde. Para alterar a cor antes de posicionar a figura, basta clicar na cor desejada e imediatamente, o cursor mudará de cor.

<p align="center"> 
  <img src="media/gifex1.gif" alt="addfig"/>
</p>

Caso se desejar criar figuras complexas, é possível utilizar a opção de polígono, onde cada clique feito irá marcar o vértice a ser criado. Assim como nas figuras simples, é possível alterar a cor durante a seleção do polígono. Para finalizar a criação do polígono, é possível apertar a tecla `enter` no teclado ou pressionar o primeiro vértice adicionado.

Para cancelar a criação do polígono, basta apertar a tecla `esc` ou pressionar qualquer área do painel principal.

<p align="center"> 
  <img src="media/polygonex1.gif" alt="addpol"/>
</p>

## Manipulação de Figuras

Apenas com o mouse, é possível mudar a posição, tamanho e rotação de uma figura, além de alterar seu preenchimento, cor e trazer para frente ou para trás com auxílio do painel principal.

Para se manipular a figura, basta clicar uma vez nela que a mesma ficará em um estado de 'selecionada'. Quando selecionada, uma caixa de opções aparecerá em sua volta, sendo os quatro pontos nos vértices da caixa responsáveis por alterar seu tamanho e proporção. Para alterá-la, basta segurar e arrastar o vértice desejado para a direção que desejar.

<p align="center"> 
  <img src="media/gifex2.gif" alt="sizefig"/>
</p>

Com o botão central da caixa de edição, é possível rotacionar a figura em sentido horário ou anti horário. Funcionando da mesma forma para redimensionar a figura, para rotacionar basta pressionar e arrastar o botão central em torno do próprio eixo de seu centro. Com a figura rotacionada, opções de redimensionamento ainda são possíveis.

<p align="center"> 
  <img src="media/resizeex.gif" alt="rotate"/>
</p>

## Demais Funções do Painel

Painel de cores: com a figura selecionada, é possível alterar a cor da figura pressionando a cor desejada nas opções centrais.

Up: traz a figura para frente em relação as demais (atalho: `+`)

- Ao segurar a tecla `ctrl` e pressionar a opção, a figura irá para a primeira camada.

Down: traz a figura para trás em relação as demais (atalho: `-`)

- Ao segurar a tecla `ctrl` e pressionar a opção, a figura irá para a última camada.

Del: deleta a figura do programa (atalho: `del`)\
Fill: altera o preenchimento da figura (atalho: `f`)\
Save: salva as figuras no arquivo `figuras.gr` (atalho: `ctrl + s`)

<p align="center"> 
  <img src="media/morefunctions.gif" alt="panel"/>
</p>

Para desmarcar a seleção de uma figura, basta pressionar a tecla `esc`. Caso nenhuma figura esteja selecionada, o programa irá se encerrar.

## Figuras.gr

É possível editar o arquivo `Figuras.gr` para iniciar o programa com qualquer figura desejada. Para isso, nessa seção será explicada como funciona o armazenamento do arquivo, dado o seguinte exemplo:

```
4
2 330 240 100 86 -35.479248 0.722983 3.254326 0.00 0.50 0.50 1
1 360 242 100 100 8632.186523 0.966980 1.697000 0.00 0.00 1.00 1
3 595 255 50 -689.335632 -1.057209 3.452323 1.00 1.00 0.00 1
4 6 249 344 0 16 86 0 153 71 108 152 75 67 3 70 309.239502 1.639585 1.503111 0.50 0.50 1.00 1
```

A primeira linha do arquivo, sempre conterá um inteiro N, que representará quantas figuras terão na figura, nesse caso, será 4 figuras.

O primeiro valor das próximas linhas, representarão o formato da figura, seguindo a seguinte tabela:

```
QUADRADO  1
TRIANGULO 2
CIRCULO   3
POLIGONO  4
```

Para cada figura, os próximas valores seguirão sua própria lógica. Para os valores 1 e 2, os dois primeiros valores serão o valor x e y do canto inferior esquerdo da figura, sendo os dois próximos números, respectivamente, a largura e a altura da figura. \
Quando a o valor for o número 3, o círculo só necessitará de 3 valores, sendo eles: o x e y do seu centro e o valor de seu raio.\
Para desenhar um polígono, a lógica é um pouco maior. Primeiro, após o número 4 (código da figura), vem o número de vértices da figura. A seguir, o valor x e y para ser tomado como base para desolcar, as próximas 6 coordenadas x e y escritas que estão em relação ao ponto (0,0). Ou seja, os vértices deve ser escritos em relação e proporção ao ponto (0,0) e assim, deselocados em x e y pelos primeiros valores lido.

A próxima sequência de números é igual para todas as figuras: o próximo valor representa a rotação do objeto em graus. Em seguida, a proporção x e y do objeto, a fim de esticá-lo ou encolhe-lo, e por fim, o valores r, g, b para a cor do objeto e uma flag (0 ou 1) para dizer se o objeto será preenchido ou não.
