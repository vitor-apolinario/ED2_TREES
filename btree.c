#include <stdio.h>
#include <stdlib.h>

/*
	k: Quantidade de chaves inseridas
	p: Quantidade de ponteiros inseridos
	leaf: Diz se a pg. é folha ou não
	**pointers: Vetor guarda o endereço das filhas
	*father: o "pai" da página atual
 */

typedef struct _page{
	int k, p, leaf;
	int *keys;
	struct _page **pointers;
	struct _page *father;
} TpPage;

TpPage* insert_key(TpPage *p, int key, int T);
TpPage* root(TpPage *p);
void print_page(TpPage *p, int T);
void sort_pointers(TpPage *p);
void insert_pointer(TpPage *p, TpPage *pointer);

TpPage* create_page(int T){
	TpPage *auxiliar 	= (TpPage*)calloc(1, sizeof(TpPage));
    auxiliar->keys  	= (int*)calloc(2*T-1, sizeof(int));
    auxiliar->pointers  = (TpPage**)calloc(2*T, sizeof(TpPage*));
	auxiliar->leaf = 1;
	auxiliar->father = NULL;
	return auxiliar;
}

void sort_page(TpPage *p){			// ordena as keys utilizando um insertion
	int i, j, key;					// https://www.geeksforgeeks.org/insertion-sort/
	int *arr = p->keys, n = p->k;

	for (i = 1; i < n; i++){
	   key = arr[i];
	   j = i-1;

	   while (j >= 0 && arr[j] > key){
	       arr[j+1] = arr[j];
	       j = j-1;
	   }
	   arr[j+1] = key;
	}
	return;
}

void sort_pointers(TpPage *p){		 // ordena(insertion) os ponteiros com base
  //puts("SORT POINTER");				 // no primeiro elemento do vetor de keys
  int i, j, n = p->p;
  TpPage **arr = p->pointers, *key;
  for (i = 1; i < n; i++){
     key = arr[i];
     j = i-1;

     while (j >= 0 && arr[j]->keys[0] > key->keys[0]){
         arr[j+1] = arr[j];
         j = j-1;
     }
     arr[j+1] = key;
  }
  return;
}

TpPage* find_page(TpPage *p, int key, int T){						// procura recursivamente em qual 
	if(p->leaf){													// página será inserido o elemento
		return p;
	}else{
		if(p->keys[0] > key){										// ve se key é menor q o primeiro elemento
			return find_page(p->pointers[0], key, T);
		}else if(key > p->keys[(p->k)-1]){							// ve se key é meior q o ultimo elemento
			return find_page(p->pointers[(p->k)], key, T);
		}else if(p->keys[0] == key || key == p->keys[(p->k)-1]){	// ve se a chave à inserida é igual à uma das pontas do vetor
			return NULL;											// se sim retorna um "nulo" 
		}else{											
			int i;						
			for(i=0; i < (2*T-1); i++){ 							// procura entre os elementos
				if(key > p->keys[i] && key < p->keys[i+1]){
					return find_page(p->pointers[i+1], key, T);
				}else if(key == p->keys[i] || key == p->keys[i+1])	// durante o for verifica de keys[i] ou keys[i+1]
					return NULL;									// é igual à chave a ser inserida
			}
		}
	}
}

TpPage* split_page(TpPage *p, int T){
	
	if(p->father == NULL && p->leaf){				// split na página que é raíz
		//puts("split pai nulo e p folha");			// e foha ao mesmo tempo
		int i;
		TpPage *novap, *novap_dir;
		novap = create_page(T);						//			[NOVAP]
		novap_dir = create_page(T);					//		[P]		   [NOVAP_DIR]
		novap->leaf = 0;							
		
		insert_key(novap,p->keys[T-1], T);			//		[1,2,3]	 ->	    [2]
													//					 [1]   [3]
		p->k = T-1;		

		for(i=T; i <= 2*T-2; i++)
			insert_key(novap_dir, p->keys[i], T);

		insert_pointer(novap, p);
		insert_pointer(novap, novap_dir);
	
		return root(novap);
	}else if(p->father == NULL && !(p->leaf)){		//split raíz e não folha
		//puts("split pai nulo e p não folha");
		TpPage *novap, *novap_dir;
		novap = create_page(T);
		novap_dir = create_page(T);
		novap->leaf = novap_dir->leaf = 0;

		//insere a chave do meio ná página que "subiu"
		insert_key(novap,p->keys[T-1], T);
		//faz a página que "subiu" apontar para p e a novap_dir
		insert_pointer(novap, p);
		insert_pointer(novap, novap_dir);
		
		//isso "quebra" o vetor no meio
		p->k = T-1;

		int i;
		//insere às chaves na nova página da direita
		for(i=T; i <= 2*T-2; i++)
			insert_key(novap_dir, p->keys[i], T);

		//insere os ponteiros na nova página da direita
    	for(i=T; i < 2*T; i++){
    		if(p->pointers[i] != NULL)
    			insert_pointer(novap_dir,p->pointers[i]);
    	}

		sort_pointers(novap);	

		//se p tiver mais que T ponteiros, seta pra T, pois os outros foram pro outro ponteiro
		if(p->p > T)
			p->p = T;

		//anula os ponteiros que foram pra outra página
		for(i=T; i < 2*T; i++)
    		p->pointers[i] = NULL;

    	return root(p);

	}else if((p->father)->k < (2*T-1)){	 			// caso o pai não seja nulo e 
		//puts("split-promote simples");			// nao esteja cheio
		int i;
		//cria nova página à direita
		TpPage *novap_dir = create_page(T);
		//quebra o vetor no meio
		p->k = T-1;

		//se p é folha, novap_dir é folha, o inverso também
		novap_dir->leaf = p->leaf;

		//insere no pai de p a key do meio do vetor(promote)
		insert_key(p->father,p->keys[T-1], T);

		//insere na nova página da direita as chaves da parte do vetor que foi quebrado
		for(i=T; i <= 2*T-2; i++)
			insert_key(novap_dir, p->keys[i], T);
		
		//os ponteiros serão redistribuídos logo precisa acertar o contador
		if(p->p > T)
			p->p = T;

		//distribui os ponteiros para a pág da direita
    	for(i=T; i < 2*T; i++){
    		if(p->pointers[i] != NULL)
    			insert_pointer(novap_dir,p->pointers[i]);
    	}

		//faz ter o mesmo pai
		novap_dir->father = p->father;
		//insere no pai a referência para novap_dir
		insert_pointer(p->father, novap_dir);
		return root(p);
	}else if((p->father)->k == (2*T-1)){ 		 	//pai não nulo e cheio
		//puts("rec split no pai");					
		/*	quando o pai de quem deve ser splitado está cheio
			ele também deve ser splitado
		*/ 
		return split_page(p->father, T);
	}
}

TpPage* insert_key(TpPage *p, int key, int T){		
	if(p==NULL){
		puts("Não foi possível inserir a chave! página nula!");
		return NULL;
	}
	int i;
	for(i=0; i < p->k ; i++){				// verifica se a chave já está no 
		if(p->keys[i] == key){				// vetor de keys
			puts("Chave já inserida");
			return root(p);
		}
	}

	//se a quantidade de chaves for menor insere
	if(p->k < (2*T-1)){
		p->keys[p->k] = key;
		p->k++;
		if(p->k > 1)
			sort_page(p);
		return root(p);
	}else{
		puts("SPLIT");								// senão
		TpPage *root = split_page(p, T);			// splita p e retorna a nova raíz
		insert_key(find_page(root,key, T),key, T);	// acha a posição que deve ser inserido
		return root;								// retorna a raíz
	}
}

void insert_pointer(TpPage *p, TpPage *pointer){	// insere o ponteiro
	//puts("INSERT POINTER");
	p->pointers[p->p] = pointer;				 	// p->p é a quantidade de ponteiros
	p->p++;											// incrementa o contador de ponteiros
	pointer->father = p;							// faz o filho apontar para o pai
	sort_pointers(p);								// ordena
}

TpPage* root(TpPage *p){							// recursivamente retorna a raíz
		TpPage *root = p;
		do{
			if(root->father == NULL){
				return root;
			}else
				root = root->father;
		}while(p!=NULL);
}

void print_page(TpPage *p, int nivel){
	int i;
	if(p!=NULL){
		printf("|");
		for(i = 0; i < p->k; ++i)
			printf("%d ", p->keys[i]);

		printf("|| Folha:%d K:%d P:%d Nivel: %d\n",p->leaf,p->k,p->p, nivel);
		for(i = 0; i < p->k; ++i){
			print_page(p->pointers[i], nivel+1);
			if(i == (p->k)-1)
				print_page(p->pointers[i+1], nivel+1);
		}
	}
}
int main(){
	int i, key = -490, op, T;
	printf("Digite T: ");
	scanf("%d", &T);
	TpPage *root = create_page(T);	
	do{
		printf("\n\tMENU\n");
		printf("1. Inserir\n");
		printf("2. Listar\n");
		printf("0. Sair\n");
		scanf("%d", &op);
		switch(op){
			case 1:
				printf("Digite o valor: ");
				scanf("%d", &key);
				system("clear");
				if(find_page(root,key, T)!=NULL){						// verifica se o endereço de inserção é não nulo
					root = insert_key(find_page(root,key, T), key, T);
				}else
					puts("Chave já foi inserida!");
				break;
			case 2:
				system("clear");
				if(key==-490)
					printf("Árvore Vazia\n");
				else
					print_page(root, 0);				
				break;
		}
	}while(op!=0);
}
