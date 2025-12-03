#include "ext.h"

dict* dict_init(void) {
   dict *d = (dict*)calloc(1, sizeof(dict));
   if (!d){
       return NULL;
   }

   d->capacity = INITIAL_CAPACITY;
   d->size = 0;
   d->buckets = (HashTable*)calloc(d->capacity, sizeof(HashTable));

   if (!d->buckets) {
       free(d);
       return NULL;
   }
   return d;
}

bool isvalid_str(const char* wd) {
   if (*wd == '\0') {
       return false;
   }

   while (*wd) {
       if (!isalpha(*wd) && *wd != '-' && *wd != '\'') {
           return false;
       }
       wd++;
   }
   return true;
}

void valid_str(const char* input, char* output) {
   if(isvalid_str(input)){
       int j = 0;
       for (int i = 0; input[i] != '\0'; i++) {
           if (isupper(input[i])) {
               output[j++] = tolower(input[i]); 
           } else if (input[i] == '\'') {
               output[j++] = '0' + (APOSTROPHE / 10);
               output[j++] = '0' + (APOSTROPHE % 10);
           } else if (input[i] != '-' ||input[i] != '.') {
               output[j++] = input[i];
           }
       }
       output[j] = '\0';
   }
}

unsigned long bersteins_hash(const char* wd, int capacity) { 
   unsigned long hash = BERSTEINS_HASH_INITIAL;
   while (*wd) {
       hash = (hash * BERSTEINS_HASH_PRIME) + (unsigned char)*wd; 
       wd++;
   }
   hash = hash % capacity;
   if(hash != 0){
       return hash;
   } else {
       return 1;
   }
}

unsigned long fast_secondary_hash(const char* wd, int capacity) {
   unsigned long hash = 0;
   while (*wd) {
       hash = (hash * 17) + (unsigned char)*wd;
       wd++;
   }
   hash = ((hash % (capacity - 1)) + 1);
   if(hash != 0){
       return hash;
   } else {
       return 1;
   }
}

bool valid_hash(const char* wd, int capacity) {
   unsigned long primary_hash = bersteins_hash(wd, capacity);
   unsigned long secondary_hash = fast_secondary_hash(wd, capacity);
   return (primary_hash > 0 && secondary_hash > 0);
}

void rehash_entry(dict* d, HashTable entry) {
   if (!d){
       return;
   }
   unsigned long primary_rehash = bersteins_hash(entry.wd, d->capacity);
   unsigned long secondary_rehash = fast_secondary_hash(entry.wd, d->capacity);

   if (!valid_hash(entry.wd, d->capacity)) {
       return;
   }

   while (d->buckets[primary_rehash].occupied) {
       primary_rehash = (primary_rehash + secondary_rehash) % d->capacity;
   }
   d->buckets[primary_rehash] = entry;
   d->size++;
}

void resize(dict* d) {
   if (!d){
       return;
   }
   int old_capacity = d->capacity;
   HashTable* old_buckets = d->buckets;

   d->capacity *= 2;
   d->size = 0;
   d->buckets = (HashTable*)calloc(d->capacity, sizeof(HashTable));
   if (!d->buckets) {
       free(old_buckets);
       return;
   }

   for (int i = 0; i < old_capacity; i++) {
       if (old_buckets[i].occupied) {
           rehash_entry(d, old_buckets[i]);
       }
   }

   free(old_buckets);
}

double calculate_load_factor(const dict* table) {
   if (table == NULL || table->capacity == 0) {
       return 0.0;
   }
   return (double)table->size / (double)table->capacity;
}

bool insert_word(dict* d, const char* wd, unsigned long primary_rehash, unsigned long secondary_rehash) {
   if (!d || *wd == '\0'){
       return false;
   }
   while (true) {
       if (!d->buckets[primary_rehash].occupied) {
           d->buckets[primary_rehash].wd = (char*)calloc(strlen(wd) + 1, sizeof(char));
           if (!d->buckets[primary_rehash].wd) {
               return false;
           }
           strcpy(d->buckets[primary_rehash].wd, wd);
           d->buckets[primary_rehash].freq = 1; 
           d->buckets[primary_rehash].occupied = true;
           d->size++;
           return true;
       } else if (strcmp(d->buckets[primary_rehash].wd, wd) == 0) {
           d->buckets[primary_rehash].freq++;
           return true;
       }
       primary_rehash = (primary_rehash + secondary_rehash) % d->capacity;
   }
}

bool dict_addword(dict* d, const char* wd) {
   if (!d || !wd || *wd == '\0'){
       return false;
   }

   char processed[256];
   valid_str(wd, processed);

   if (calculate_load_factor(d) >= LOAD_FACTOR_THRESHOLD) {
       resize(d);
   }

   unsigned long primary_hash = bersteins_hash(processed, d->capacity);
   unsigned long secondary_hash = fast_secondary_hash(processed, d->capacity);

   return insert_word(d, processed, primary_hash, secondary_hash);
}

int dict_wordcount(const dict* d) {
   if (!d){
       return 0;
   }

   int total = 0;
   for (int i = 0; i < d->capacity; i++) {
       if (d->buckets[i].occupied) {
           total += d->buckets[i].freq;
       }
   }
   return total;
}

void dict_free(dict** d) {
   if (!d || !*d){
       return;
   }

   for (int i = 0; i < (*d)->capacity; i++) {
       if ((*d)->buckets[i].occupied) {
           free((*d)->buckets[i].wd); 
       }
   }
   free((*d)->buckets);
   free(*d);
   *d = NULL;
}

dict* dict_spell(const dict* d, const char* wd) {
   if (!d || !wd || *wd == '\0') {
       return NULL;
   }

   char processed[256];
   valid_str(wd, processed);

   unsigned long index = bersteins_hash(processed, d->capacity);
   unsigned long step = fast_secondary_hash(processed, d->capacity);

   while (d->buckets[index].occupied) {
       if (strcmp(d->buckets[index].wd, processed) == 0) {
           return (dict*)&d->buckets[index];
       }
       index = (index + step) % d->capacity;
   }
   return NULL;
}

void find_most_common(const dict* d, int* max_freq) {
   if (d == NULL || d->buckets == NULL) {
       return;
   }

   for (int i = 0; i < d->capacity; i++) {
       if (d->buckets[i].occupied && d->buckets[i].freq > *max_freq) {
           *max_freq = d->buckets[i].freq;
       }
   }
}

int dict_mostcommon(const dict* d) {
   if (d == NULL) {
       return 0;
   }

   int max_freq = 0;
   find_most_common(d, &max_freq);
   return max_freq;
}
