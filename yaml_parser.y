%language "C"
%output "yaml_parser.c"
%defines "yaml_parser.h"
%verbose
%locations

%define api.prefix {yaml}
%define api.token.prefix {yaml_}
%define api.pure full
%define api.push-pull push

%define lr.type lalr
%define lr.default-reduction accepting
%define lr.keep-unreachable-state false

%define parse.lac full
%define parse.error verbose
%define parse.trace true

%token c_sequence_entry c_mapping_key c_mapping_value
%token c_literal c_folded
%token s_indent s_separate_in_line
%token l_empty b_break
%token nb_char ns_plain_one_line
%start yaml

%code requires {
#include "yaml.h"
}

%code provides {
#define YYSTYPE YAMLSTYPE
#define YYLTYPE YAMLLTYPE
}

%code {
void yyerror(YAMLLTYPE *, struct yaml *, char const *);
}

%define api.value.type {struct yaml_node *}
%parse-param {struct yaml * yaml}

%%

yaml : l_bare_document
     | l_empty_r l_bare_document

l_bare_document : s_indent s_l_block_node
                | l_bare_document s_indent s_l_block_node

s_l_block_node : ns_plain
               | l_block_scalar
               | l_block_sequence
               | l_block_mapping

ns_plain : ns_plain_one_line s_l_comments

l_block_scalar : c_literal s_l_comments s_indent nb_char s_l_comments
               | c_folded s_l_comments s_indent nb_char s_l_comments
               | nb_char s_l_comments

l_block_sequence : c_sequence_entry s_separate s_l_block_node

l_block_mapping : ns_plain_one_line c_mapping_value s_separate s_l_block_node
                | c_mapping_key s_separate ns_plain s_indent c_mapping_value s_separate s_l_block_node

s_separate : s_separate_in_line
           | s_l_comments s_indent

s_l_comments : b_break
             | b_break l_empty_r

l_empty_r : l_empty
          | l_empty_r l_empty

%%

void yyerror(YAMLLTYPE * location, struct yaml * yaml, char const * message) {
    panic("%s (line %d)", message, location->first_line);
}