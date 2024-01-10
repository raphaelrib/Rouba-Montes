#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define Vermelho "\033[1;31m"
#define Preto "\033[1;30m"
#define Reset "\033[0m"
const char *cor;

typedef struct carta {
  int naipe;
  int numero;
} Carta;

typedef struct elemento {
  Carta valor;
  struct elemento *proximo;
  struct elemento *anterior;
} Elemento;

typedef struct pilha {
  Elemento *topo;
} Pilha;

typedef struct jogador {
  char nome[20];
  Pilha *monte;
  int numCart;
} Jogador;

typedef struct lista {
  Elemento *inicio;
} Lista;

void intercala(Jogador a[], int n) {
  Jogador *temp = (Jogador *)malloc(n * sizeof(Jogador));
  int meio = n / 2;
  int i = 0;
  int j = meio;
  int k = 0;

  while (i < meio && j < n) {
    if (a[i].numCart > a[j].numCart) {
      temp[k] = a[i];
      i++;
    } else {
      temp[k] = a[j];
      j++;
    }
    k++;
  }
  if (i == meio) {
    while (j < n) {
      temp[k] = a[j];
      j++;
      k++;
    }
  } else {
    while (i < meio) {
      temp[k] = a[i];
      i++;
      k++;
    }
  }

  for (i = 0; i < n; i++) {
    a[i] = temp[i];
  }

  free(temp);
}

void mergeSort(Jogador a[], int n) {
  if (n > 1) {
    int meio = n / 2;
    mergeSort(a, meio);
    mergeSort(a + meio, n - meio);
    intercala(a, n);
  }
}

Pilha *criaPilha() {
  Pilha *p;
  p = (Pilha *)malloc(sizeof(Pilha));
  p->topo = NULL;
  return p;
}
void liberaPilha(Pilha *p) {
  while (p->topo != NULL) {
    Elemento *aux;
    aux = p->topo;
    p->topo = aux->proximo;
    free(aux);
  }
}
int empilha(Pilha *p, Carta v) {
  Elemento *novo;
  novo = (Elemento *)malloc(sizeof(Elemento));
  novo->valor = v;
  novo->proximo = p->topo;
  p->topo = novo;
  return 1;
}
int pilhaVazia(Pilha *p) {
  if (p->topo == NULL) {
    return 1;
  } else {
    return 0;
  }
}
int desempilha(Pilha *p, Carta *v) {
  if (pilhaVazia(p)) {
    return 0;
  } else {
    Elemento *aux;
    aux = p->topo;
    *v = aux->valor;
    p->topo = aux->proximo;
    free(aux);
    return 1;
  }
}

int countPilha(Pilha *p) {
  Elemento *aux = p->topo;
  int count = 0;
  while (aux != NULL) {
    aux = aux->proximo;
    count++;
  }
  return count;
}
void mostrarCarta(int numero, int naipe) {
  if (naipe == 1 || naipe == 2)
    cor = Preto;
  else if (naipe == 3 || naipe == 4)
    cor = Vermelho;
  else
    cor = Reset;
  printf("%sCarta %d - Naipe %d%s\n", cor, numero, naipe, Reset);
}

void mostrarPilha(Pilha *p) {
  if (pilhaVazia(p)) {
    printf("A pilha está Vazia!!!\n");
  } else {
    Elemento *aux = p->topo;
    for (int i = 0; aux != NULL; i++) {
      mostrarCarta(aux->valor.numero, aux->valor.naipe);
      aux = aux->anterior;
    }
  }
}
Lista *criarLista() {
  Lista *l = (Lista *)malloc(sizeof(Lista));
  l->inicio = NULL;
  return l;
}
void limparLista(Lista *l) {
  Elemento *aux = l->inicio;
  while (aux->proximo != NULL) {
    aux->anterior = aux->proximo;
    aux->proximo = aux->proximo->proximo;
    free(aux->anterior);
  }
  l->inicio = NULL;
}
void excluirLista(Lista *l) {
  limparLista(l);
  free(l);
}
int listaVazia(Lista *l) {
  if (l->inicio == NULL) {
    return 1;
  } else {
    return 0;
  }
}
int countLista(Lista *l) {
  Elemento *aux = l->inicio;
  int count = 0;
  while (aux != NULL) {
    aux = aux->proximo;
    count++;
  }
  return count;
}

int insereUp(Lista *l, Carta v) {
  Elemento *c = (Elemento *)malloc(sizeof(Elemento));
  c->proximo = NULL;
  c->valor = v;
  if (listaVazia(l)) {
    l->inicio = c;
  } else {
    Elemento *aux;
    aux = l->inicio;
    while (aux->proximo != NULL) {
      aux = aux->proximo;
    }
    aux->proximo = c;
  }
  return 1;
}
int inserePos(Lista *l, int p, Carta v) {
  Elemento *novo = (Elemento *)malloc(sizeof(Elemento));
  novo->valor = v;
  novo->proximo = NULL;

  if (p < 1 || p > countLista(l) + 1) {
    return 0;
  } else if (listaVazia(l)) {
    l->inicio = novo;
  } else if (p == 1) {
    novo->proximo = l->inicio;
    l->inicio = novo;
  } else {
    Elemento *aux = l->inicio;
    int count = 1;
    while (aux != NULL && count + 1 < p) {
      aux = aux->proximo;
      count++;
    }
    novo->proximo = aux->proximo;
    aux->proximo = novo;
  }
  return 1;
}
int insereOrdenado(Lista *l, Carta c) {
  Elemento *aux = l->inicio;
  int p = 1;
  while (aux != NULL) {
    if (c.numero <= aux->valor.numero) {
      break;
    } else {
      p++;
    }
    aux = aux->proximo;
  }
  inserePos(l, p, c);
  return 0;
}

int removeUP(Lista *l, Carta *v) {
  if (listaVazia(l)) {
    return 0;
  } else {
    Elemento *aux = l->inicio;

    if (aux->proximo == NULL) {
      *v = aux->valor;
      free(aux);
      l->inicio = NULL;
    } else {
      while (aux->proximo->proximo != NULL) {
        aux = aux->proximo;
      }
      *v = aux->proximo->valor;
      free(aux->proximo);
      aux->proximo = NULL;
    }
    return 1;
  }
}
int removepos(Lista *l, int p, Carta *v) {
  Elemento *aux = l->inicio;

  if (p < 1 || p > countLista(l)) {
    return 0;
  } else if (p == 1) {
    *v = aux->valor;
    l->inicio = l->inicio->proximo;
    free(aux);
    return 1;
  } else {
    int count = 1;
    while (aux->proximo != NULL && count + 1 < p) {
      aux = aux->proximo;
      count++;
    }
    Elemento *aux2 = aux->proximo;
    *v = aux2->valor;
    aux->proximo = aux->proximo->proximo;
    free(aux2);
    return 1;
  }
}
int recuperachave(Lista *l, Carta v) {
  Elemento *aux = l->inicio;
  int count = 1, retorno = 0;

  while (aux != NULL) {
    if (aux->valor.numero == v.numero) {
      retorno = count;
      break;
    } else {
      aux = aux->proximo;
      count++;
    }
  }
  return retorno;
}
int recuperapos(Lista *l, int p, Carta *v) {
  Elemento *aux = l->inicio;
  int count = 1;

  while (aux != NULL) {
    if (count == p) {
      *v = aux->valor;
      return 1;
      break;
    } else {
      aux = aux->proximo;
      count++;
    }
  }
  return 0;
}

void mostrarLista(Lista *l) {
  if (listaVazia(l))
    printf("A lista está Vazia!!!\n");
  else {
    Elemento *aux = l->inicio;
    for (int i = 0; aux != NULL; i++) {
      mostrarCarta(aux->valor.numero, aux->valor.naipe);
      aux = aux->proximo;
    }
  }
}

void embaralha(int n, Pilha *c) {
  Lista *baralho = criarLista();
  int num_cartas = n * 52;
  int pos;
  Carta aux;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= 4; j++) {
      for (int k = 1; k <= 13; k++) {
        aux.naipe = j;
        aux.numero = k;
        insereUp(baralho, aux);
      }
    }
  }
  while (num_cartas > 0) {
    pos = (rand() % num_cartas) + 1;

    removepos(baralho, pos, &aux);
    empilha(c, aux);

    num_cartas--;
  }
}

void roubarMonte(Jogador *j, int jcap, int jv, Carta c) {
  empilha(j[jcap].monte, c);
  Elemento *aux = j[jcap].monte->topo;
  while (aux->anterior != NULL) {
    aux = aux->anterior;
  }
  aux->anterior = j[jv].monte->topo;
  j[jv].monte->topo = j[jcap].monte->topo;
  j[jcap].monte->topo = NULL;
}
int areaDescarte(Jogador *j, int jv, Carta c, Lista *d) {
  int p = recuperachave(d, c);
  if (p != 0) {
    Carta c_add;
    removepos(d, p, &c_add);
    empilha(j[jv].monte, c_add);
    empilha(j[jv].monte, c);
    return 1;
  } else
    return 0;
}
int proprioMonte(Jogador j, Carta c) {
  if (pilhaVazia(j.monte))
    return 0;
  if (j.monte->topo->valor.numero == c.numero) {
    empilha(j.monte, c);
    return 1;
  } else
    return 0;
}

void mostrarOrdenado(Jogador j, int n) {
  Lista *ordenada = criarLista();
  Pilha *temp = criaPilha();
  Pilha *copia = criaPilha();
  Carta aux;
  for (int i = 0; i < n; i++) {
    desempilha(j.monte, &aux);
    empilha(temp, aux);
    empilha(copia, aux);
    insereOrdenado(ordenada, copia->topo->valor);
  }
  while (!pilhaVazia(temp)) {
    desempilha(temp, &aux);
    empilha(j.monte, aux);
  }
  mostrarLista(ordenada);
  excluirLista(ordenada);
  free(temp);
  free(copia);
}

void ranking(Jogador *j, int nj) {

  for (int i = 0; i < nj; i++) {
    j[i].numCart = countPilha(j[i].monte);
  }
  mergeSort(j, nj);
  printf("\n\tRaking do Rouba Montes: \n");
  for (int i = 0; i < nj; i++) {
    printf("\n%d° Colocado:\n", i + 1);
    printf("Nome: %s\n", j[i].nome);
    printf("Quantidade de cartas: %d\n", j[i].numCart);
    printf("Lista de Cartas: \n\n");
    mostrarOrdenado(j[i], j[i].numCart);
    printf("\n\n");
  }
}

int main() {

  srand(time(NULL));
  int numB, numJ;
  Jogador *jogadores;
  Pilha *compra = criaPilha();
  Lista *descarte = criarLista();

  printf("\n\t Bem vindo ao jogo de cartas Rouba Montes!!!\n\n");
  printf("Iremos utilizar números para os naipes!\n");
  printf("%s1 - Para Espadas(♠️)%s\n", Preto, Reset);
  printf("%s2 - Para Paus(♣️)%s\n", Preto, Reset);
  printf("%s3 - Para Copas(♥️)%s\n", Vermelho, Reset);
  printf("%s4 - Para Ouros(♦️)%s\n", Vermelho, Reset);
  printf("Entre com a quantidade de jogadores: ");
  scanf("%d", &numJ);
  setbuf(stdin, NULL);

  jogadores = (Jogador *)malloc(numJ * sizeof(Jogador));
  for (int i = 0; i < numJ; i++) {
    printf("Nome do %d° jogador: ", i + 1);
    scanf("%s", jogadores[i].nome);
    jogadores[i].monte = criaPilha();
  }

  printf("Entre com o número de baralhos: ");
  scanf("%d", &numB);
  embaralha(numB, compra);

  printf("\n\tVamos começar o jogo!\n");
  Carta carta_atual;
  int op;
  while (pilhaVazia(compra) == 0) {
    for (int i = 0; i < numJ; i++) {
    jogueNovamente:
      sleep(1);
      system("clear");
      int n = countPilha(compra);
      printf("\nAtualizando mesa:\n");
      sleep(1);
      printf("\nQuantidade de cartas na pilha de compras: \n\n%d\n", n);
      sleep(1);
      printf("\nLista de descarte atual:\n\n");
      mostrarLista(descarte);
      sleep(1);
      printf("\nTopo do monte dos jogadores:\n");
      for (int j = 0; j < numJ; j++) {
        printf("\n\tJogador(a) %s:\n\n", jogadores[j].nome);
        sleep(1);
        if (!pilhaVazia(jogadores[j].monte)) {
          mostrarCarta(jogadores[j].monte->topo->valor.numero,
                       jogadores[j].monte->topo->valor.naipe);
        } else {
          printf("Monte vazio.\n\n");
        }
      }
      if (!pilhaVazia(compra)) {
        printf("\n\t\t%s é a sua vez!\n\n", jogadores[i].nome);
        printf("Sua carta retirada da pilha de compras é: \n");
        sleep(1);
        desempilha(compra, &carta_atual);
        mostrarCarta(carta_atual.numero, carta_atual.naipe);
      } else {
        sleep(1);
        printf("\n\tE pilha de compras vazia, LOGO:\n");
        printf("\n\tJOGO ENCERRADO!\n");
        printf("\n\tMuito obrigado por jogar!\n");
        break;
      }
    outraOP:
      printf("\n\tEscolha uma opção:\n");
      printf("\t1 - Descartar Carta.\n");
      printf("\t2 - Roubar Carta do Descarte.\n");
      printf("\t3 - Roubar Monte do Adversário.\n");
      printf("\t4 - Empilhar Carta no Proprio Monte.\n");
      scanf("%d", &op);
      switch (op) {
      case 1:
        if (recuperachave(descarte, carta_atual)) {
          printf("Carta já está presente no descarte.\n");
          printf("Tente outra jogada.\n");
          goto outraOP;
        }
        insereUp(descarte, carta_atual);
        printf("Carta descartada com sucesso.\n");
        break;
      case 2:
        if (areaDescarte(jogadores, i, carta_atual, descarte)) {
          printf("Carta roubada com sucesso.\n\n");
          printf("Jogue Novamente!\n");
          sleep(1);
          goto jogueNovamente;
        } else {
          printf("Carta não está na área de descarte.\n");
          printf("Tente outra jogada.\n");
          goto outraOP;
        }
        break;
      case 3:
        printf("Deseja roubar o monte de qual jogador?\n");
        char qualJog[20];
        scanf("%s", qualJog);

        int jogadorEncontrado = 0;

        for (int q = 0; q < numJ; q++) {
          if (strcmp(qualJog, jogadores[q].nome) == 0 &&
              strcmp(qualJog, jogadores[i].nome) != 0) {
            jogadorEncontrado = 1;

            if (jogadores[q].monte->topo != NULL &&
                jogadores[q].monte->topo->valor.numero == carta_atual.numero) {
              roubarMonte(jogadores, q, i, carta_atual);
              printf("Monte roubado com sucesso.\n\n");
              printf("Jogue Novamente!\n");
              sleep(1);
              goto jogueNovamente;
            } else {
              printf("Não foi possível roubar o monte deste jogador.\n");
              printf("Tente outra jogada.\n");
              goto outraOP;
            }
          }
        }

        if (!jogadorEncontrado) {
          printf("Jogador não encontrado.\n");
          printf("Tente outra jogada.\n");
          goto outraOP;
        }
        break;

      case 4:
        if (proprioMonte(jogadores[i], carta_atual)) {
          printf("Carta empilhada com sucesso.\n\n");
          printf("Jogue Novamente!\n");
          sleep(1);
          goto jogueNovamente;
        }
        printf("Não foi possível empilhar no próprio monte.\n");
        printf("Tente outra jogada.\n");
        goto outraOP;
        break;
      default:
        printf("Opção inválida.\n");
        printf("Tente outra jogada.\n");
        goto outraOP;
        break;
      }
    }
  }
  ranking(jogadores, numJ);
  return 0;
}