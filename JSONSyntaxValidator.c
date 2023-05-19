#include <stdio.h>
#include <stdlib.h>

#define MENU "\n 1 - Analise sintatica\n 2 - Sair\n"
#define SAIR 2
#define MSG_FNE "FNE - Ficheiro nao existe!\n"
#define MSG_FBF "\nFBF - Ficheiro bem formado!\n"
#define MSG_FMF "\nFMF - Ficheiro mal formado!\n"

#define MSG_ERRO_TIPO " "
#define MSG_ERR2_UNDERFLOW "\nerro 02: stack underflow!\n"
#define MSG_ERR1_UNDERFLOW "\nerro 01: stack overflow!\n"

enum Elemento_json_e {
  elemento_json_ERRO = 0,
  elemento_json_STRING,
  elemento_json_NUMBER,
  elemento_json_OBJECT,
  elemento_json_OBJECT_END,
  elemento_json_ARRAY,
  elemento_json_ARRAY_END
};
typedef enum Elemento_json_e Elemento_json;


typedef struct t_contadorElementos {
  int pilha_STRING;
  int pilha_NUMBER;
  int pilha_OBJECT;
  int pilha_ARRAY;
} contadorElementos;


int json_e_string(char ch) { return ch == '"'; }

int json_e_number(char ch) {
  return (ch >= '0' && ch <= '9') || ch == '+' || ch == '-' || ch == '.' ||
         ch == 'e' || ch == 'E';
}

int json_e_object(char ch) { return ch == '{'; }

int json_e_object_end(char ch) { return ch == '}'; }

int json_e_array(char ch) { return ch == '['; }

int json_e_array_end(char ch) { return ch == ']'; }

void printElement(Elemento_json elem) {
  switch (elem) {
  case elemento_json_STRING:
    printf("\"");
    break;

  case elemento_json_ARRAY:
    printf("[");
    break;
  case elemento_json_ARRAY_END:
    printf("]");
    break;

  case elemento_json_NUMBER:
    printf("NUMBER");
    break;

  case elemento_json_OBJECT:
    printf("{");
    break;
  case elemento_json_OBJECT_END:
    printf("}");
    break;

  case elemento_json_ERRO:
  default:
    printf("?");
  }
}

int pop(int pilha, Elemento_json tipo) {

  /* Imprime o tipo */
  printf("\n pop  ");
  printElement(tipo);

  /* Decrementa pilha e depois verifica se deu stack underflow */
  pilha--;
  if (pilha < 0) {
    printf(MSG_ERR2_UNDERFLOW);
    pilha = -1;
  }
  return pilha;
}


int push(int pilha, int max_stack, Elemento_json tipo) {
 
  printf("\npush ");
  printElement(tipo);

  
  pilha++;
  if (pilha > max_stack) {
    printf(MSG_ERR1_UNDERFLOW);
    pilha = -1;
  }

  return pilha;
}


Elemento_json ler_Elemento_json(char ch) {
  Elemento_json tipo; 
  if (json_e_string(ch))
    tipo = elemento_json_STRING;

  else if (json_e_object(ch))
    tipo = elemento_json_OBJECT;

  else if (json_e_object_end(ch))
    tipo = elemento_json_OBJECT_END;

  else if (json_e_array(ch))
    tipo = elemento_json_ARRAY;

  else if (json_e_array_end(ch))
    tipo = elemento_json_ARRAY_END;

  else if (json_e_number(ch))
    tipo = elemento_json_NUMBER;

  else
    tipo = elemento_json_ERRO;

  return tipo;
}


int menu() {
  int opcao = 2; 
  printf(MENU);
  scanf(" %d", &opcao);
  return opcao;
}

contadorElementos parse(char str, contadorElementos cont, int nomesemsentido) {
  enum Elemento_json_e nomecemsentido = ler_Elemento_json(str);

 
  switch (nomecemsentido) {
  case elemento_json_STRING:
    if (cont.pilha_STRING == 1)
      cont.pilha_STRING = pop(cont.pilha_STRING, nomecemsentido);
    else
      cont.pilha_STRING =
          push(cont.pilha_STRING, nomesemsentido, nomecemsentido);
    break;
  case elemento_json_ARRAY:
    cont.pilha_ARRAY = push(cont.pilha_ARRAY, nomesemsentido, nomecemsentido);
    break;
  case elemento_json_ARRAY_END:
    cont.pilha_ARRAY = pop(cont.pilha_ARRAY, nomecemsentido);
    break;
  case elemento_json_NUMBER:
    cont.pilha_NUMBER++;
    break;
  case elemento_json_OBJECT:
    cont.pilha_OBJECT = push(cont.pilha_OBJECT, nomesemsentido, nomecemsentido);
    break;
  case elemento_json_OBJECT_END:
    cont.pilha_OBJECT = pop(cont.pilha_OBJECT, nomecemsentido);
    break;
  case elemento_json_ERRO:
  default:
    printf(MSG_ERRO_TIPO);
  }
  return cont;
}

void detetaErros(contadorElementos resultado) {

  if ((resultado.pilha_ARRAY == 0) && (resultado.pilha_OBJECT == 0) &&
      (resultado.pilha_STRING == 0))
    printf(MSG_FBF);
  else
    printf(MSG_FMF);
}


int detetaStackOverflow(contadorElementos resultado) {
  if ((resultado.pilha_ARRAY == -1) || (resultado.pilha_OBJECT == -1) ||
      (resultado.pilha_STRING == -1))
    return 1;
  else
    return 0;
}

void analise_sintatica() {

  int stacksize = 0;
 
  char filename[100];
 
  contadorElementos resultado = {0};
  FILE *ptr;
  char ch;
  printf("\nInforme o tamanho da stack: ");
  scanf(" %d", &stacksize);
  printf("\nInforme o nome do ficheiro a analisar:");
  scanf(" %s", filename);
  ptr = fopen(filename, "r");
  if (NULL == ptr) {
    printf(MSG_FNE);
    return;
  }

  do {
    ch = fgetc(ptr);
    resultado = parse(ch, resultado, stacksize);
    if (detetaStackOverflow(resultado))
    ch = EOF;
  } while (ch != EOF);
  fclose(ptr);
  detetaErros(resultado);
}


int main(void) {
  int opcao = 1;

  
  while (opcao == 1) {
    opcao = menu();
    switch (opcao) {
   
    case 1:
      analise_sintatica();
      break;

    default:
     
      return 1;
    }
   
  }
  return 0;
}
