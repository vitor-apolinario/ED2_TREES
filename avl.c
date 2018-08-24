#include<stdio.h>
#include<stdlib.h>

/*POSSUI UM PROBLEMA EM UMA ROTAÇÃO ESPECÍFICA*/

typedef struct _nodo{
	int chave;
	int altura, nivel;
	struct _nodo *esq;
	struct _nodo *dir;
	struct _nodo *pai;
} TpNodo;

typedef struct _arvore{
	TpNodo *raiz;
} TpArvore;

void inserir(TpArvore *arvore, TpNodo *nodo,TpNodo *pai);
void mostra(TpNodo *nodo, int nivel);
void rotaciona(TpArvore *avl,TpNodo *pai);
void RightRight(TpArvore *avl,TpNodo *nodo);
void LeftLeft(TpArvore *avl,TpNodo *nodo);
void LeftRight(TpArvore *avl, TpNodo *Nodo);
void RightLeft(TpArvore *avl, TpNodo *nodo);
void novoNodo(TpArvore *arvore, int chave);

TpArvore *inicializa(){
	TpArvore *arvore = (TpArvore*)malloc(sizeof(TpArvore));
	arvore->raiz=NULL;
	return arvore;
}

int max(TpNodo *a, TpNodo *b){
	if(a == NULL && b == NULL){
		return -1;
	}else if(a == NULL){
		return b->altura;
	}else if(b == NULL){
		return a->altura;
	}else
		return a->altura > b->altura ? a->altura : b->altura;
}

int fator_balanceamento(TpNodo *a, TpNodo *b){
	int esq, dir;
	if(a == NULL && b == NULL)
		return 0;
	if(a==NULL){
		esq = -1;
	}else
		esq = a->altura;
	if(b==NULL){
		dir = -1;
	}else
		dir = b->altura;
	return esq - dir;
}

void novoNodo(TpArvore *arvore, int chave){
	TpNodo *nodo = (TpNodo*)malloc(sizeof(TpNodo));
	nodo->chave = chave;
	nodo->esq = NULL;
	nodo->dir = NULL;
	inserir(arvore,nodo,NULL);
}

void inserir(TpArvore *arvore, TpNodo *nodo,TpNodo *pai){
	TpNodo *aux = arvore->raiz;
	TpArvore *nwTree = inicializa();
	if(arvore->raiz == NULL){
		printf("||%d|| Inserido na raíz\n\n",nodo->chave);
		arvore->raiz = nodo;
		arvore->raiz->pai = pai;
		if(pai!= NULL){
			arvore->raiz->nivel = (pai->nivel+1);
		}else
			arvore->raiz->nivel = 0;
		arvore->raiz->esq = arvore->raiz->dir = NULL;
		arvore->raiz->altura = 0;
		return;
	}
	if(nodo->chave == aux->chave){
		printf("Nodo já inserido\nTente novamente\n\n");
		return;
	}
	if(nodo->chave > aux->chave && aux->dir == NULL){
		printf("||%d|| Inserido a direita de ||%d||\n\n",nodo->chave,aux->chave);
		aux->dir = nodo;
		nodo->pai = aux;
		nodo->altura = 0;
		nodo->esq = nodo->dir = NULL;
		nodo->nivel = nodo->pai->nivel +1;
		if(aux->esq==NULL)
			aux->altura++;
		return;
	}else if(nodo->chave < aux->chave && aux->esq == NULL){
		printf("||%d|| Inserido a esquerda de ||%d||\n\n",nodo->chave,aux->chave);
		aux->esq = nodo;
		nodo->pai = aux;
		nodo->altura = 0;
		nodo->esq = nodo->dir =NULL;
		nodo->nivel = nodo->pai->nivel +1;
		if(aux->dir==NULL)
			aux->altura++;
		return;
	}else if(nodo->chave > aux->chave){
		nwTree->raiz = aux->dir;
		inserir(nwTree,nodo,aux);		
		aux->altura =  max(aux->esq,aux->dir) +1;
		if(fator_balanceamento(aux->esq,aux->dir) < -1 || fator_balanceamento(aux->esq,aux->dir)>1){		
			if(pai == NULL){
				nwTree = arvore;
			}else 
				nwTree->raiz = aux;
		rotaciona(nwTree,aux);
		}
		return;
	}else if(nodo->chave < aux->chave){
		nwTree->raiz = aux->esq;
		inserir(nwTree,nodo,aux);
		aux->altura = max(aux->esq,aux->dir) + 1;
		if(fator_balanceamento(aux->esq,aux->dir) < -1 || fator_balanceamento(aux->esq,aux->dir)>1){		
			if(pai == NULL){
				nwTree = arvore;
			}else 
				nwTree->raiz = aux;
		rotaciona(nwTree,aux);
		}	
		return;
	}
}

int main(){
	system("clear");
	TpArvore *arvore = inicializa();
	int op=0, chave=0;
	do{
		printf("MENU\n");
		printf("1: Inserir\n");
		printf("2: Listar\n");
		printf("3: Sair\n");
		scanf("%d", &op);
		switch(op){
			case 1:
				system("clear");
				printf("Digite o valor da chave: ");
				scanf("%d", &chave);
				system("clear");
				if(chave<1){
					printf("Chave negativa.\nTente novamente.\n\n");
				}else
					novoNodo(arvore, chave);
				break;
			case 2:
				system("clear");
				mostra(arvore->raiz, 0);
				printf("\n\n");
				break;
		}
	}while(op!=3);
}

void rotaciona(TpArvore *avl,TpNodo *nodo){
	if(fator_balanceamento(nodo->esq,nodo->dir) < -1){
		if(fator_balanceamento(nodo->dir->esq,nodo->dir->dir) < 0)
			RightRight(avl,nodo);
		else
			RightLeft(avl, nodo);
	}else if(fator_balanceamento(nodo->esq,nodo->dir) >1){
		if(fator_balanceamento(nodo->esq->esq,nodo->esq->dir) > 0) 
			LeftLeft(avl,nodo);
		else
			LeftRight(avl, nodo);
	}
}

void RightRight(TpArvore *avl,TpNodo *nodo){
	TpNodo *a = avl->raiz;
	TpNodo *b = a->dir;
	TpNodo *c = b->esq;
	TpNodo *pai = a->pai;	
	//identifica que vai ter q rotacionar elementos do meio
	if(a->esq != NULL || b->esq != NULL){
		b->esq = a;
		a->pai = b;
		avl->raiz = b;
		a->dir = c;
		c->pai = a;
		a->altura = max(a->esq,a->dir) + 1;
		c->altura = a->altura-1;
		b->pai = pai;
	}else{
	//senão rotação simples
		avl->raiz = b;
		b->pai = pai;
		b->esq = a;
		a->pai = b;
		a->dir = NULL;
		a->altura = 0;
		b->altura = max(b->esq, b->dir) + 1;
	}
	//aponta o pai para a nova raíz nesse caso B
	if(pai != NULL){
		if(pai->chave > b->chave){ 
			pai->esq = b; 
		}else if(pai->chave < b->chave) 
			pai->dir = b; 
	}
}

void LeftLeft(TpArvore *avl,TpNodo *nodo){ 
	TpNodo *a = avl->raiz;
	TpNodo *b = a->esq;
	TpNodo *c = b->dir;
	TpNodo *pai = a->pai;
	//identifica que vai ter q rotacionar elementos do meio
	if(a->dir != NULL || b->dir!=NULL){
		b->dir = a;
		a->pai = b;
		avl->raiz = b;
		a->esq = c;
		c->pai = a;
		a->altura = b->altura-1;
		c->altura = a->altura-1;
		b->pai = pai;
	}else{
	//senão rotação simples	
		avl->raiz = b;
		b->pai = pai;
		b->dir = a;
		a->pai = b;
		a->esq = NULL;
		a->altura = 0;
		b->altura = max(b->esq, b->dir) + 1;
	}
	//aponta o pai para a nova raíz nesse caso B
	if(pai != NULL){
		if(pai->chave > b->chave){ 
			pai->esq = b; 
		}else if(pai->chave < b->chave) 
			pai->dir = b;
	}
}
//A = Raiz da sub-arvore, B = esquerda de A e C = direita de B;
void LeftRight(TpArvore *avl, TpNodo *nodo){
	TpNodo *a = avl->raiz;
	TpNodo *b = a->esq;
	TpNodo *c = b->dir;
	b->pai = c;
	c->pai = a;
	a->esq = c;
	c->esq = b;
	b->dir = NULL;
	b->altura--;
	a->altura--;
	LeftLeft(avl, nodo);
}

void RightLeft(TpArvore *avl, TpNodo *nodo){
	TpNodo *a = avl->raiz;
	TpNodo *b = a->dir;
	TpNodo *c = b->esq;
	b->pai = c;
	c->pai = a;
	a->dir = c;
	c->dir = b;
	b->esq = NULL;
	b->altura--;
	a->altura--;
	RightRight(avl, nodo);
}

void mostra(TpNodo *nodo, int nivel){
	if(nodo){
		printf("\n| Chave: %d |", nodo->chave);
		printf(" Nível: %d |", nivel);
		if(nivel)
			printf(" Pai: %d |", nodo->pai->chave);
		else
			printf(" RAIZ|");
		if(nodo->esq != NULL)
			mostra(nodo->esq, nivel+1);
		if(nodo->dir != NULL)
			mostra(nodo->dir, nivel+1);
	}else
		printf("Arvore Vazia!\n");
}