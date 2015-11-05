#include "mpc.h"

#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy) - 1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Lisrb    = mpc_new("lisrb");
  
  mpca_lang(MPCA_LANG_DEFAULT, 
      "                                                    \
        number   : /-?[0-9]+/ ;                            \
        operator : '+' | '-' | '*' | '/' ;                 \
        expr     : <number> | '(' <operator> <expr>+ ')' ; \
        lisrb    : /^/ <operator> <expr>+ /$/ ;            \
      ",
      Number, Operator, Expr, Lisrb);

  puts("Lisrb version 0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char* input = readline("lisrb> ");

    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lisrb, &r)) {
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expr, Lisrb);

  return 0;
}

