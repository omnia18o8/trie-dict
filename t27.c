#include "td27.h"
#define APOSTROPHE 26
#define LOWERCASE_ASCII_CONVERSION 97
#define UPPERCASE_ASCII_CONVERSION 65

dict* dict_init(void);
bool isvalid_str(const char* str);
int char2index(const char* str);
bool update_terminal_node(dict* p);
bool dict_addword(dict* p, const char* str);
void cmp_max_freq(const dict* p, int* max_freq);
dict* create_subnode(dict* p, const char* wd);
void tmp_term_node(dict* p, const char* wd);
void rm_term_node(dict* p, const char* wd);
void get_suffix(const dict* node, char* buffer, int depth, char* best_suffix, int* max_freq);
void traverse_tree(const dict* node, char* buffer, int depth, char* best_suffix, int* max_freq);

dict* dict_init(void) {
  dict *d = (dict*)calloc(1, sizeof(dict));
  if(!d){
   return NULL;
  } 
  for (int i = 0; i < ALPHA; i++) {
       d->dwn[i] = NULL;
   }
  d->up = NULL; 
  d->terminal = false;
  d->freq = 0;
  return d;
}

bool isvalid_str(const char* str) {
   if (*str == '\0') {
       return false;
   }

   while (*str) {
       if (!isalpha(*str) && *str != '-' && *str != '\'') {
           return false;
       }
       str++;
   }
   return true;
}

int char2index(const char* str) {
   int index;
   if (*str == '\'') {
       index = APOSTROPHE; 
   } else if (islower(*str)) {
       index = *str - LOWERCASE_ASCII_CONVERSION; 
   } else if (isupper(*str)) {
       index = *str - UPPERCASE_ASCII_CONVERSION;
   } else { 
       return -1; 
   }
   return index;
}

bool create_node(dict* p, const char* str, int i) {
   if (!p || !str){
       return false;
   }
   if (!p->dwn[i]) {
       p->dwn[i] = dict_init();
       if (!p->dwn[i]){
           return false;
       }
       p->dwn[i]->up = p;
   }
   return true;
}

bool update_terminal_node(dict* p) {
   if (!p){
       return false;
   }

   if (p->terminal) {
       p->freq++; 
       return false; 
   }

   p->terminal = true;
   p->freq = 1; 
   return true; 
}

bool dict_addword(dict* p, const char* str) {
   if (!p || !str){
       return false;
   }

   if (*str == '\0') {
       return update_terminal_node(p); 
   }

   int i = char2index(str);
   if (i == -1) {
       return false;
   }

   if (!create_node(p, str, i)) { 
       return false;
   }

   return dict_addword(p->dwn[i], str + 1);
}

int dict_nodecount(const dict* p) {
   if (!p){
       return 0;
   }
   int count = 1;
   for (int i = 0; i < ALPHA; i++) {
       count += dict_nodecount(p->dwn[i]);
   }
   return count;
}

int dict_wordcount(const dict* p) {
   if (!p){
       return 0;
   }

   int count = 0;
   if (p->terminal){
       count += p->freq;
       } 

   for (int i = 0; i < ALPHA; i++) {
       count += dict_wordcount(p->dwn[i]);
   }
   return count;
}

void dict_free(dict** p) {
   if (!p || !*p ){
       return;
   }
   for (int i = 0; i < ALPHA; i++) {
       dict_free(&(*p)->dwn[i]);
   }
   free(*p);
   *p = NULL;
}

void cmp_max_freq(const dict* p, int* max_freq) {
   if (!p) {
       return;
   }

   if (p->terminal && p->freq > *max_freq) {
       *max_freq = p->freq;
   }

   for (int i = 0; i < ALPHA; i++) {
       cmp_max_freq(p->dwn[i], max_freq);
   }
}

int dict_mostcommon(const dict* p) {
   if (!p) {
       return 0;
   }
   int max_freq = 0;
   cmp_max_freq(p, &max_freq);
   return max_freq;
}

unsigned dict_cmp(dict* p1, dict* p2) {
   if (!p1 || !p2){
       return -1;
   }

   int depth1 = 0, depth2 = 0;
   for (dict* n = p1; n; n = n->up){
       depth1++;
   }
   for (dict* n = p2; n; n = n->up){
       depth2++;
   }

   unsigned distance = 0;
   while (depth1 > depth2) {
       p1 = p1->up;
       distance++;
       depth1--;
   }
   while (depth2 > depth1) {
       p2 = p2->up;
       distance++;
       depth2--;
   }

   while (p1 != p2) {
       p1 = p1->up;
       p2 = p2->up;
       distance += 2;
   }

   return distance;
}

dict* dict_spell(const dict* p, const char* str) {
   if (!p || !str) {
       return NULL;
   }

   if (*str == '\0') {
       if (p->terminal) {
           return (dict*)p;
       } else {
           return NULL; 
       }
   }

   int i = char2index(str);
   if (i == -1|| p->dwn[i] == NULL) {
       return NULL; 
   }

   return dict_spell(p->dwn[i], str + 1);
}

dict* create_subnode(dict* p, const char* wd) {
   if (!p || !wd){
       return NULL;
   }
   dict* current = p;
   while (*wd) {
       int i = char2index(wd);
       if (i == -1) {
           return NULL;
       }

       if (!current->dwn[i]) {
           current->dwn[i] = dict_init();
           current->dwn[i]->up = current; 
       }

       current = current->dwn[i];
       wd++;
   }
   return current;
}

void tmp_term_node(dict* p, const char* wd) {
   dict* node = create_subnode(p, wd);
   if (node) {
       node->terminal = true;
       node->freq = 0; 
   }
}

void rm_term_node(dict* p, const char* wd) {
   dict* node = dict_spell(p, wd);
   if (node && node->terminal && node->freq == 0) {
       node->terminal = false;
   }
}

void get_suffix(const dict* node, char* buffer, int depth, char* suffix, int* max_freq) {
   if (node->terminal && node->freq > *max_freq) {
       *max_freq = node->freq;
       buffer[depth] = '\0';
       strcpy(suffix, buffer);
   }
}

void traverse_tree(const dict* node, char* buffer, int depth, char* suffix, int* max_freq) {
   if (!node){
       return;
   }

   get_suffix(node, buffer, depth, suffix, max_freq);

   for (int i = 0; i < ALPHA; i++) {
       if (node->dwn[i]) {
           if(i == APOSTROPHE){
               buffer[depth] = '\'';
           } else {
               buffer[depth] = 'a' + i;
           }
           traverse_tree(node->dwn[i], buffer, depth + 1, suffix, max_freq);
       }
   }
}

void dict_autocomplete(const dict* p, const char* wd, char* ret) {
   if (!p || !wd || !ret) {
       ret[0] = '\0';
       return;
   }

   tmp_term_node((dict*)p, wd);
   dict* terminal_node = (dict*)dict_spell(p, wd);

   if (!terminal_node) {
       ret[0] = '\0';
       rm_term_node((dict*)p, wd);
       return;
   }

   char buffer[256] = "";
   char best_suffix[256] = "";
   int max_freq = 0;

   buffer[0] = '\0';
   traverse_tree(terminal_node, buffer, 0, best_suffix, &max_freq);
   strcpy(ret, best_suffix); 
   rm_term_node((dict*)p, wd);
}
