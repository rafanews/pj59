#ifndef table_h
#define table_h

#include "heap.h"
#include "csv.h"
#include "parser.h"

struct item_combo_node {
    char * combo;
    char * bonus;
    struct item_combo_node * next;
};

struct item_node {
    long id;
    char * name;
    char * bonus;
    char * equip;
    char * unequip;
    struct item_combo_node * combo;
};

struct item {
    struct store store;
    struct stack stack;
    struct strbuf strbuf;
    struct map id;
    struct map name;
    struct item_node * item;
};

int item_create(struct item *, size_t, struct heap *);
void item_destroy(struct item *);
int item_parse(enum parser_type, int, struct string *, void *);
int item_script_parse(struct item *, char *);
int item_combo_parse(enum parser_type, int, struct string *, void *);

struct skill_node {
    long id;
    char * name;
    char * description;
    long level;
};

struct skill {
    struct store store;
    struct map id;
    struct map name;
    struct skill_node * skill;
};

int skill_create(struct skill *, size_t, struct heap *);
void skill_destroy(struct skill *);
int skill_parse(enum parser_type, int, struct string *, void *);

struct mob_node {
    long id;
    char * sprite;
    char * kro;
};

struct mob {
    struct store store;
    struct map id;
    struct map sprite;
    struct mob_node * mob;
};

int mob_create(struct mob *, size_t, struct heap *);
void mob_destroy(struct mob *);
int mob_parse(enum parser_type, int, struct string *, void *);

struct mercenary_node {
    long id;
    char * name;
};

struct mercenary {
    struct store store;
    struct map id;
    struct mercenary_node * mercenary;
};

int mercenary_create(struct mercenary *, size_t, struct heap *);
void mercenary_destroy(struct mercenary *);
int mercenary_parse(enum parser_type, int, struct string *, void *);

struct constant_node {
    char * identifier;
    long value;
    char * tag;
    struct range_node * range;
    int variable;
};

struct constant_group_node {
    char * identifier;
    struct map map_identifier;
    struct map map_value;
    struct constant_group_node * next;
};

struct constant {
    struct store store;
    struct map identifier;
    struct map group;
    struct constant_node * constant;
    struct range_node * range;
    struct constant_group_node * constant_group;
};

int constant_create(struct constant *, size_t, struct heap *);
void constant_destroy(struct constant *);
struct constant_group_node * constant_group(struct constant *);
int constant_parse(enum parser_type, int, struct string *, void *);
int constant_data_parse(enum parser_type, int, struct string *, void *);
int constant_group_parse(enum parser_type, int, struct string *, void *);

struct optional_node {
    long index;
    char * string;
    struct optional_node * next;
};

enum integer_flag {
    integer_sign = 1 << 0,
    integer_string = 1 << 1,
    integer_percent = 1 << 2,
    integer_inverse = 1 << 3,
    integer_absolute = 1 << 4
};

struct integer_node {
    long flag;
    long divide;
};

struct array_node {
    long index;
    char * string;
};

struct print_node {
    struct entry_node * entry;
    struct print_node * next;
};

#define ENTRY_MAX 16

struct entry_node {
    long array[ENTRY_MAX];
    size_t count;
    char * identifier;
    size_t length;
    char * string;
    struct entry_node * next;
};

struct argument_node {
    char * identifier;
    char * handler;
    struct print_node * print;
    struct range_node * range;
    struct map * map;
    struct integer_node * integer;
    struct optional_node * optional;
    struct argument_node * next;
};

struct argument {
    struct store store;
    struct map identifier;
    struct argument_node * argument;
    struct entry_node * entry;
    struct print_node * print;
    struct range_node * range;
    struct array_node * array;
    struct integer_node * integer;
    struct optional_node * optional;
};

int argument_create(struct argument *, size_t, struct heap *);
void argument_destroy(struct argument *);
int argument_parse(enum parser_type, int, struct string *, void *);
int argument_entry_parse(struct argument *, char *, size_t);
int argument_entry_create(struct argument *, char *, size_t);

struct table {
    struct parser parser;
    struct item item;
    struct skill skill;
    struct mob mob;
    struct mercenary mercenary;
    struct constant constant;
    struct argument argument;
    struct argument bonus;
    struct argument bonus2;
    struct argument bonus3;
    struct argument bonus4;
    struct argument bonus5;
    struct argument sc_start;
    struct argument sc_start2;
    struct argument sc_start4;
    struct argument statement;
};

int table_create(struct table *, size_t, struct heap *);
void table_destroy(struct table *);
int table_item_parse(struct table *, char *);
int table_item_combo_parse(struct table *, char *);
int table_skill_parse(struct table *, char *);
int table_mob_parse(struct table *, char *);
int table_mercenary_parse(struct table *, char *);
int table_constant_parse(struct table *, char *);
int table_constant_data_parse(struct table *, char *);
int table_constant_group_parse(struct table *, char *);
int table_argument_parse(struct table *, char *);
int table_bonus_parse(struct table *, char *);
int table_bonus2_parse(struct table *, char *);
int table_bonus3_parse(struct table *, char *);
int table_bonus4_parse(struct table *, char *);
int table_bonus5_parse(struct table *, char *);
int table_sc_start_parse(struct table *, char *);
int table_sc_start2_parse(struct table *, char *);
int table_sc_start4_parse(struct table *, char *);
int table_statement_parse(struct table *, char *);

struct item_node * item_start(struct table *);
struct item_node * item_next(struct table *);
struct item_node * item_id(struct table *, long);
struct item_node * item_name(struct table *, char *);

struct skill_node * skill_id(struct table *, long);
struct skill_node * skill_name(struct table *, char *);

struct mob_node * mob_id(struct table *, long);
struct mob_node * mob_sprite(struct table *, char *);

struct mercenary_node * mercenary_id(struct table *, long);

struct constant_node * constant_identifier(struct table *, char *);
struct constant_group_node * constant_group_identifier(struct table *, char *);
struct argument_node * argument_identifier(struct table *, char *);
struct argument_node * bonus_identifier(struct table *, char *);
struct argument_node * bonus2_identifier(struct table *, char *);
struct argument_node * bonus3_identifier(struct table *, char *);
struct argument_node * bonus4_identifier(struct table *, char *);
struct argument_node * bonus5_identifier(struct table *, char *);
struct argument_node * sc_start_identifier(struct table *, char *);
struct argument_node * sc_start2_identifier(struct table *, char *);
struct argument_node * sc_start4_identifier(struct table *, char *);
struct argument_node * statement_identifier(struct table *, char *);

#endif
