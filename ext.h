#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 98317
#define LOAD_FACTOR_THRESHOLD 0.75
#define ALPHA 27
#define APOSTROPHE 26
#define BERSTEINS_HASH_INITIAL 5381
#define BERSTEINS_HASH_PRIME 33

typedef struct {
   char* wd;
   int freq;
   bool occupied;
} HashTable;

struct dict{
   HashTable* buckets;
   int capacity;
   int size;
};
typedef struct dict dict;

dict* dict_init(void);
bool isvalid_str(const char* wd);
void valid_str(const char* input, char* output);

unsigned long bersteins_hash(const char* wd, int capacity);
unsigned long fast_secondary_hash(const char* wd, int capacity);
bool valid_hash(const char* wd, int capacity);

void rehash_entry(dict* d, HashTable entry);
void resize(dict* d);
double calculate_load_factor(const dict* table);
bool insert_word(dict* d, const char* wd, unsigned long primary_rehash, unsigned long secondary_rehash);

bool dict_addword(dict* d, const char* wd);
int dict_wordcount(const dict* d);
void dict_free(dict** d);
dict* dict_spell(const dict* d, const char* wd);

void find_most_common(const dict* d, int* max_freq);
int dict_mostcommon(const dict* d);

void test(void);
void test2(void);
