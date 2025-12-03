#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ALPHA 27
struct dict {
    struct dict* dwn[ALPHA];
   struct dict* up; 
   bool terminal;
   int freq;
};
typedef struct dict dict;

dict* dict_init(void);
bool dict_addword(dict* p, const char* wd);
int dict_nodecount(const dict* p);
int dict_wordcount(const dict* p);
dict* dict_spell(const dict* p, const char* str);
void dict_free(dict** p);
int dict_mostcommon(const dict* p);
int char2index(const char* str);
bool update_terminal_node(dict* p);
dict* create_subnode(dict* p, const char* wd);
void tmp_term_node(dict* p, const char* wd);
void rm_term_node(dict* p, const char* wd);
void get_suffix(const dict* node, char* buffer, int depth, char* best_suffix, int* max_freq);
void traverse_tree(const dict* node, char* buffer, int depth, char* best_suffix, int* max_freq);

unsigned dict_cmp(dict* p1, dict* p2);
void dict_autocomplete(const dict* p, const char* wd, char* ret);
void test(void);
void test2(void);
