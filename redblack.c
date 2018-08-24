#include <stdio.h>
#include <stdlib.h>

typedef struct _nodo{
	int chave;
	int cor;
	struct _nodo *esq;
	struct _nodo *dir;
	struct _nodo *pai;
}TpNodo;

typedef struct _arvore{
	TpNodo *raiz;
	TpNodo *sentinela;
}TpArvore;

void insere(TpArvore *arvore, TpNodo *nodo, TpArvore *arvore_original);
void fixup (TpArvore *tree, TpNodo *nodo, TpArvore *arvore_original);
void case1 (TpArvore *tree, TpNodo *nodo, TpArvore *arvore_original);
void case2E(TpArvore *tree, TpNodo *nodo);
void case3E(TpArvore *tree, TpNodo *nodo);
void case2D(TpArvore *tree, TpNodo *nodo);
void case3D(TpArvore *tree, TpNodo *nodo);
void printar(TpNodo *nodo,TpNodo *sentinela);

TpArvore *inicializa(){
	TpArvore *arvore = (TpArvore*)malloc(sizeof(TpArvore));
	arvore->raiz = NULL;
	return arvore;
}
TpNodo *cria_nodo(int chave){
	TpNodo *nodo = (TpNodo*)malloc(sizeof(TpNodo));
	nodo->pai = nodo->esq = nodo->dir = NULL;
	nodo->cor = 1;
	nodo->chave = chave;
	return nodo;
}
void insereEsq(TpNodo *pai, TpNodo *filho){
	pai->esq = filho;
	filho->pai = pai;
}
void insereDir(TpNodo *pai, TpNodo *filho){
	pai->dir = filho;
	filho->pai = pai;
}
void insere(TpArvore *arvore, TpNodo *nodo, TpArvore *arvore_original){
	TpNodo *raiz = arvore->raiz;
	nodo->esq = nodo->dir = arvore->sentinela; //faz o nodo apontar para o sentinela
	TpArvore *nwTree = inicializa();
	nwTree->sentinela = arvore->sentinela;
	if(arvore->raiz == NULL){
		nodo->cor = 0;
		nodo->pai = arvore->sentinela;
		arvore->raiz = nodo;
		return;
	}
	if(nodo->chave == arvore->raiz->esq->chave || nodo->chave == arvore->raiz->dir->chave || nodo->chave == arvore->raiz->chave){
		puts("Nodo já inserido.");
		return;
	}else if(raiz->chave > nodo->chave && raiz->esq == arvore->sentinela){
		insereEsq(raiz,nodo);
		//----------------------------------------------------------------
		if(raiz->cor){
			if(nodo->pai->pai->pai != arvore->sentinela){
				nwTree->raiz = nodo->pai->pai;
				fixup(nwTree,nodo,arvore_original);
			}else{
				fixup(arvore_original,nodo,arvore_original);
			}
		}
		return;
		//----------------------------------------------------------------
	}else if(raiz->chave < nodo->chave && raiz->dir == arvore->sentinela){
		insereDir(raiz,nodo);
		//----------------------------------------------------------------
		if(raiz->cor){
			if(nodo->pai->pai->pai != arvore->sentinela){
				nwTree->raiz = nodo->pai->pai;
				fixup(nwTree,nodo,arvore_original);
			}else{
				fixup(arvore_original,nodo,arvore_original);
			}
		}
		return;
		//----------------------------------------------------------------
	}
	if(raiz->chave > nodo->chave && raiz->esq != arvore->sentinela){
		nwTree->raiz = raiz->esq;
		insere(nwTree,nodo,arvore_original);
		return;
	}else if(raiz->chave < nodo->chave && raiz->dir != arvore->sentinela){
		nwTree->raiz = raiz->dir;
		insere(nwTree,nodo,arvore_original);
		return;
	}
}
void fixup(TpArvore *tree,TpNodo *nodo, TpArvore *arvore_original){
	if(nodo->pai != tree->sentinela){
		if(tree->raiz->chave > nodo->chave){
			//nodo problemático está na subarvore à esquerda de seu avô
			if(tree->raiz->dir->cor){
				case1(tree,nodo,arvore_original);
			}else{
				if(nodo->pai->chave > nodo->chave){
					case3E(tree,nodo);
				}else
					case2E(tree,nodo);
			}
		}else{
			//nodo problemático está na subarvore à direita de seu avô
			if(tree->raiz->esq->cor){
				case1(tree,nodo,arvore_original);
			}else{
				if(nodo->pai->chave > nodo->chave){
					case2D(tree,nodo);
				}else
					case3D(tree,nodo);
			}
		}
	}else
		nodo->cor = 0;
}
void case1(TpArvore *tree, TpNodo *nodo, TpArvore *arvore_original){	
	TpNodo *avo = tree->raiz;	 			
	avo->esq->cor = avo->dir->cor = 0;		
	avo->cor = 1;							
	
	if(avo->pai->cor == 1 || (avo->pai == tree->sentinela && avo->cor == 1)){
		if(avo->pai == tree->sentinela){
			fixup(tree, avo, arvore_original);
		}else{
			if(avo->pai->pai->pai == tree->sentinela){
				fixup(arvore_original,avo,arvore_original);
			}else{
				TpArvore *nwTree = inicializa();
				nwTree->raiz = avo->pai->pai;
				nwTree->sentinela = tree->sentinela;
				fixup(nwTree,avo,arvore_original);
			}
		}
	}
}
void case2E(TpArvore *tree, TpNodo *nodo){
	TpNodo *a = nodo;
	TpNodo *b = a->pai;
	TpNodo *c = tree->raiz;
	TpNodo *t1 = a->esq;
	insereEsq(c,a);
	insereEsq(a,b);
	insereDir(b,t1);
	case3E(tree,b);      
}
void case3E(TpArvore *tree, TpNodo *nodo){
	TpNodo *b = nodo;
	TpNodo *a = b->pai;
	TpNodo *c = a->pai;
	TpNodo *t2 = a->dir;
	tree->raiz = a;
	if(c->pai == tree->sentinela){	
		a->pai = tree->sentinela;
	}else{
		if(c->chave > c->pai->chave){
			insereDir(c->pai,a);
		}else
			insereEsq(c->pai,a);
	}
	insereDir(a,c);
	insereEsq(c,t2);
	a->cor = 0;
	c->cor = 1;
}
void case2D(TpArvore *tree, TpNodo *nodo){
	TpNodo *a = nodo;
	TpNodo *b = a->pai;
	TpNodo *c = tree->raiz;
	TpNodo *t1 = a->dir;
	insereDir(c,a);
	insereDir(a,b);
	insereEsq(b,t1);
	case3D(tree,b);
}
void case3D(TpArvore *tree, TpNodo *nodo){
	TpNodo *b = nodo;
	TpNodo *a = b->pai;
	TpNodo *c = a->pai;
	TpNodo *t2 = a->esq;
	tree->raiz = a;
	if(c->pai == tree->sentinela){	
		a->pai = tree->sentinela;
	}else{
		if(c->chave > c->pai->chave){
			insereDir(c->pai,a);
		}else
			insereEsq(c->pai,a);
	}
	insereEsq(a,c);
	insereDir(c,t2);
	a->cor = 0;
	c->cor = 1;
}
void printar(TpNodo *nodo,TpNodo *sentinela){
	if(nodo->esq != sentinela)
 		printar(nodo->esq,sentinela);
	if(nodo->pai == sentinela)
		printf("Raíz-> ");
	if(nodo != sentinela){
 		printf("Nodo: %d | Pai: %d | Cor: ", nodo->chave, nodo->pai->chave);
 		if(nodo->cor){
 			puts("Vermelho");
 		}else
 			puts("Preto");
 	}
 	if(nodo->dir != sentinela)
 		printar(nodo->dir,sentinela);
}
int main(){
	TpArvore *arvore = inicializa();
	TpNodo *sentinela = (TpNodo*)malloc(sizeof(TpNodo));
	sentinela->cor = 0;
	sentinela->chave = -256;
	arvore->sentinela = sentinela;
	int op, chave = -490;
	TpNodo *cobaia;
	do{
		printf("\n\tMENU\n");
		printf("1. Inserir\n");
		printf("2. Listar\n");
		printf("0. Sair\n");
		scanf("%d", &op);
		switch(op){
			case 1:
				printf("Digite o valor: ");
				scanf("%d", &chave);
				system("clear");
				cobaia = cria_nodo(chave);
				insere(arvore, cobaia, arvore);
				break;
			case 2:
				system("clear");
				if(chave==-490)
					printf("Árvore Vazia\n");
				else
					printar(arvore->raiz, arvore->sentinela);
				break;
		}
	}while(op!=0);
}