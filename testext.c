#include <assert.h>
#include <string.h>
#include "ext.h"

void test(void) {
   dict* d = dict_init();
   assert(d != NULL);
   assert(d->size == 0);

   assert(isvalid_str("Kool-Aid"));
   assert(!isvalid_str("Hello*"));
   assert(!isvalid_str("Hello@World"));
   assert(!isvalid_str("12345"));
   assert(!isvalid_str(""));

   assert(valid_hash("test", INITIAL_CAPACITY));
   assert(valid_hash("anotherTest", INITIAL_CAPACITY));
   assert(valid_hash("12345", INITIAL_CAPACITY));

   unsigned long primary_hash = bersteins_hash("hello", d->capacity);
   unsigned long secondary_hash = fast_secondary_hash("hello", d->capacity);
   assert(insert_word(d, "hello", primary_hash, secondary_hash) == true);
   assert(strcmp(d->buckets[primary_hash].wd, "hello") == 0);
   assert(d->buckets[primary_hash].freq == 1);

   dict_free(&d);
   d = dict_init();

   const char* words[100] = {
       "From", "the", "movie", "10", "things", "i", "hate", "about", "you",
       "I", "hate", "the", "way", "you", "talk", "to", "me", "And", "the",
       "way", "you", "cut", "your", "hair", "I", "hate", "the", "way",
       "you", "drive", "my", "car", "I", "hate", "it", "when", "you", "stare",
       "I", "hate", "your", "big", "dumb", "combat", "boots", "I", "hate", "it",
       "when", "you", "lie"
       };

   int num_words = sizeof(words) / sizeof(words[0]);
   for (int i = 0; i < num_words; i++) {
       dict_addword(d, words[i]);
       }

   assert(dict_wordcount(d) == 51);
   assert(dict_mostcommon(d) == 6);
   dict* entry_dict = dict_spell(d, "hate");
   assert(entry_dict != NULL);

   dict_free(&d);
   assert(d == NULL);
}


void test2(void) {
    dict* d = NULL;

   assert(dict_spell(d, "the")==NULL);
   assert(dict_wordcount(NULL)==0);

   d = dict_init();
   assert(dict_addword(d, "car"));
   assert(dict_addword(d, "cart"));
   assert(dict_addword(d, "part"));
   assert(dict_wordcount(d)==3);
   assert(dict_mostcommon(d)==1);
   dict_free(&d);

   d = dict_init();
   assert(d);
   assert(dict_wordcount(d)==0);
   assert(dict_spell(d, NULL)==NULL);

   assert(dict_spell(d, "car")==NULL);
   assert(dict_addword(d, "car"));
   assert(dict_spell(d, "car"));
   assert(dict_wordcount(d)==1);
   assert(dict_spell(d, "carted")==NULL);
   assert(dict_addword(d, "carted"));
   assert(dict_spell(d, "carted"));
   assert(dict_wordcount(d)==2);

   
   assert(dict_spell(d, "cart")==NULL);
   assert(dict_addword(d, "Cart"));
   assert(dict_spell(d, "cart"));
   assert(dict_wordcount(d)==3);

   assert(dict_spell(d, "cart'd")==NULL);
   assert(dict_addword(d, "cart'd"));
   assert(dict_spell(d, "Cart'd"));
   assert(dict_wordcount(d)==4);

   assert(dict_mostcommon(d)==1);
   dict_addword(d, "cart");
   assert(dict_mostcommon(d)==2);
   assert(dict_wordcount(d)==5);
   dict* p = dict_spell(d, "cart");
   assert(p);

   dict_addword(d, "par");
   dict_addword(d, "part");
   dict_addword(d, "parted");
   dict_addword(d, "parter");
   assert(dict_wordcount(d)==9);
   dict_free(&d);
   assert(d==NULL);
}
