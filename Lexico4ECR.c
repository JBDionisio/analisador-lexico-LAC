//Analisador Léxico 4ECR
//Referência Apostila 2.1.1

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

char *codigo;
int tamanho,pos;

const char *ops = "+-*/";

typedef struct{
	int tipo;  //Token
	int valor; //Lexema
}Token;

//A struct a seguir representa o NÓ da lista encadeada
struct lista{
	int tipo;
	int valor;
	int linha; //Ocorrências
	int coluna;
	struct lista* next; //Ponteiro para próximo nó
};
typedef struct lista Tabela;

Tabela* t;

#define TRUE 1
#define FALSE 0

#define TOK_NUM 0
#define TOK_OP 1
#define TOK_PONT 2

#define SOMA 0
#define SUB 1
#define MUL 2
#define DIV 3

#define PARESQ 0
#define PARDIR 1

Tabela* cria(void){
	return NULL;
}

Tabela* add(Tabela* t,Token *tok,int linha,int coluna) {
	Tabela* no = (Tabela*) malloc(sizeof(Tabela));
	no->tipo = tok->tipo;
	no->valor = tok->valor;
	no->linha = linha;
	no->coluna = coluna;
	no->next = t;
	return no;
}

void inicializaAnalise(char* prog) {
	codigo = prog;
	tamanho = strlen(codigo);	
	pos = 0;	
}

char leCaractere(void) {
	char c;
	
	if(pos<tamanho) {
		c=codigo[pos];
		pos++;
	} else {
		c = -1; // to check
	}
	
	return c;
}

int operador(char c) {
	int res;
	
	switch(c) {
		case '+':
			res = SOMA;
			break;
		case '-':
			res = SUB;
			break;
		case '*':
			res = MUL;
			break;
		case '/':
			res = DIV;
			break;
		default:
			res = -1;
	}
	
	return res;
}

Token *proximoToken(Token *token) {
	char c;
	char valor[200];
	int vpos = 0;
	
	c = leCaractere();
	
	while(isspace(c)) {
		c = leCaractere();
	}
	
	if(isdigit(c)) {
		token->tipo = TOK_NUM;
		
		valor[vpos++] = c;			
		c = leCaractere();
		while(isdigit(c)) {
			valor[vpos++] = c;			
			c = leCaractere();
		}
		pos--;
		
		valor[vpos] = '\0';
		token->valor = atoi(valor);
		
		t = add(t, token, 2, 3);
				
	} else if(strchr(ops,c) != NULL) {
		token->tipo = TOK_OP;
		token->valor = operador(c);
		
		t = add(t, token, 2, 3);
	} else if(c == '(' || c == ')') {
		token->tipo = TOK_PONT;
		token->valor = (c == '(') ? PARESQ : PARDIR;
		
		t = add(t, token, 2, 3);
	} else {
		return NULL;
	}
	
	return token;
}

char *operadorStr(int valor) {
	char *res;
	
	switch(valor) {
		case SOMA:
			res = "SOMA";
			break;
		case SUB:
			res = "SUBTRACAO";
			break;
		case MUL:
			res = "MULTIPLICACAO";
			break;
		case DIV:
			res = "DIVICAO";
			break;
		default:
			res = "ERROR";
			break;		
	}
	
	return res;
}

char *pontuacaoStr(int valor) {
	char *res;
	
	switch(valor) {
		case PARESQ:
			res = "PARENTESES_ESQ";
			break;
		case PARDIR:
			res = "PARENTESES_DIR";
			break;
		default:
			res = "ERROR";
			break;		
	}
	
	return res;
}

void imprimeToken(Token *token, FILE *file) {
	switch(token->tipo) {
		case TOK_NUM:
			fprintf(file, "|Numero|\t\t - Valor: %d\n", token->valor);
			break;
		case TOK_OP:
			fprintf(file, "|Operador|\t\t - Valor: %s\n", operadorStr(token->valor));
			break;
		case TOK_PONT:
			fprintf(file, "|Pontuacao|\t\t - Valor: %s\n", pontuacaoStr(token->valor));
			break;
		default:
			fprintf(file, "|ERROR| - Token Desconhecido\n");
			break;
	}
}

void imprimeTabelaSimbolos(Tabela *t, FILE *file) {
	Tabela *p;
	int e = 0;
	
	for(p=t;p!=NULL;p=p->next) {
		e++;
		switch(p->tipo) {
			case TOK_NUM:
				fprintf(file, "\nE%d\nToken: num\t Lex: %d \t Ocor: L%d|C%d\n", e, p->valor, p->linha, p->coluna);
				break;
			case TOK_OP:
				fprintf(file, "\nE%d\nToken: op\t Lex: %s \t Ocor: L%d|C%d\n", e, operadorStr(p->valor), p->linha, p->coluna);
				break;
			case TOK_PONT:
				fprintf(file, "\nE%d\nToken: pont\t Lex: %s \t Ocor: L%d|C%d\n", e, pontuacaoStr(p->valor), p->linha, p->coluna);
				break;
			default:
				fprintf(file, "\n|ERROR| Token Desconhecido\n");
				break;
		}
	}
}

int main(void){
	FILE *fptr; //entrada.txt Código-Fonte
	FILE *fptr2; //saida.txt Fluxo de Tokens
	FILE *fptr3; //tabela.txt Tabela de Símbolos	
	
	if((fptr=fopen("entrada.txt","r"))==NULL){
		puts("Não foi possível abrir o arquivo\n\n");
		exit(1);
	}
	
	fptr2=fopen("saida.txt","w");
	fptr3=fopen("tabela.txt","w");
	
	printf("Grande Analisador Lexico da turma 4ECR\n\n");
	printf("Lendo Codigo-Fonte...\n\n");
	
	char entrada[200];
	Token token;
	
	fgets(entrada, 200, fptr);
	
	inicializaAnalise(entrada);
	
	while(proximoToken(&token) != NULL) {
		imprimeToken(&token,fptr2);
	}
	
	imprimeTabelaSimbolos(t, fptr3);
	
	fclose(fptr);
	fclose(fptr2);
	fclose(fptr3);
	
	printf("\n\nAnalise Lexica finalizada com sucesso.\n\n");
	return 0;
}

