#include <assert.h>
#include <string.h>
#include "td27.h"


void test(void) {
   //Testing dict_init
   dict* b = dict_init();
   assert(b->terminal == false);
   assert(b->freq == 0);
   assert(b->up == NULL);
   for (int i = 0; i < ALPHA; i++) {
       assert(b->dwn[i] == NULL);
   }
   assert(update_terminal_node(b));
   dict_free(&b);

   //Testing addword, dict_spell, wordcount, mostcommon
   dict* p = dict_init();
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
       dict_addword(p, words[i]);
   }
   assert(dict_wordcount(p) == 50);
   assert(dict_mostcommon(p) == 6);
   assert(dict_spell(p, "hate"));
   dict_free(&p);

   // Test create_subnode
   dict* d = dict_init();
   dict* node = create_subnode(d, "apple");
   assert(node != NULL);
   assert(node->terminal == false);
   assert(d->dwn[char2index("a")] != NULL);
   assert(d->dwn[char2index("a")]->dwn[char2index("p")] != NULL);

   dict* banana_node = create_subnode(d, "banana");
   assert(banana_node != NULL);
   assert(banana_node != node);

   // Test tmp_term_node and rm_term_node
   tmp_term_node(d, "apple");
   assert(node->terminal == true);
   assert(node->freq == 0);

   rm_term_node(d, "apple");
   assert(node->terminal == false);

   // Test get_suffix and addword
   char buffer[256] = "";
   char suffix[256] = "";
   int max_freq = 0;

   dict_addword(d, "apples");
   dict_addword(d, "applet");
   dict_addword(d, "apples");

   buffer[0] = '\0';
   get_suffix(node, buffer, 5, suffix, &max_freq); 
   assert(strcmp(suffix, "") == 0);
   traverse_tree(node, buffer, 0, suffix, &max_freq);
   assert(strcmp(suffix, "s") == 0);
   assert(max_freq == 2);

   // Test traverse_tree
   dict* root = create_subnode(d, "app");
   buffer[0] = '\0';
   suffix[0] = '\0';
   max_freq = 0;

   traverse_tree(root, buffer, 0, suffix, &max_freq);
   assert(strcmp(suffix, "les") == 0);
   assert(max_freq == 2);

   dict_free(&d);
   assert(d == NULL);
}









void test2(void) {
dict* d = NULL;
char str[50];
   // No words are in empty tree
   assert(dict_spell(d, "the")==NULL);
   // No nodes in empty tree
   assert(dict_nodecount(NULL)==0);
   assert(dict_wordcount(NULL)==0);

/* The figure in the assignment  */
   d = dict_init();
   assert(dict_addword(d, "car"));
   assert(dict_addword(d, "cart"));
   assert(dict_addword(d, "part"));
   assert(dict_nodecount(d)==9);
   assert(dict_wordcount(d)==3);
   assert(dict_mostcommon(d)==1);
   dict* q1 = dict_spell(d, "car");
   dict* q2 = dict_spell(d, "part");
   assert(dict_cmp(q1, q2)==7);
   // It's unsigned
   assert(dict_cmp(q2, q1)==7);
   // 2 steps up from 'carter' is 'cart'
   dict_autocomplete(d, "car", str);
   // Most frequently stored word after car is car+t
   assert(strcmp(str, "t")==0);
   dict_free(&d);

/* A slightly different example */
   d = dict_init();
   // Should be a non-NULL address
   assert(d);
   // Top node always exists
   assert(dict_nodecount(d)==1);
   assert(dict_wordcount(d)==0);
   // Can't add a NULL string
   assert(dict_spell(d, NULL)==NULL);

   // 'car' isn't in the tree
   assert(dict_spell(d, "car")==NULL);
   // So add it
   assert(dict_addword(d, "car"));
   // It's there now
   assert(dict_spell(d, "car"));
   // Top node + 'c' + 'a' + 'r'
   assert(dict_nodecount(d)==4);
   assert(dict_wordcount(d)==1);

   // carted isn't in there
   assert(dict_spell(d, "carted")==NULL);
   // Put it in
   assert(dict_addword(d, "carted"));
   // It is now
   assert(dict_spell(d, "carted"));
   // 'car' -> 'carted' is 3 extra nodes
   assert(dict_nodecount(d)==7);
   assert(dict_wordcount(d)==2);

   // Cart isn't there as a terminal word
   assert(dict_spell(d, "cart")==NULL);
   // Put it in (case-insensitive)
   assert(dict_addword(d, "Cart"));
   // It's now there
   assert(dict_spell(d, "cart"));
   // No new nodes were created
   assert(dict_nodecount(d)==7);
   assert(dict_wordcount(d)==3);

   // Cart'd isn't there as a terminal word
   assert(dict_spell(d, "cart'd")==NULL);
   // Put it in
   assert(dict_addword(d, "cart'd"));
   // It's now there
   assert(dict_spell(d, "Cart'd"));
   // New nodes were created
   assert(dict_nodecount(d)==9);
   assert(dict_wordcount(d)==4);

   // Only one of each word at the moment
   assert(dict_mostcommon(d)==1);
   // Increments the freq variable
   // Should return false (repeat)
   assert(!dict_addword(d, "cart"));
   // Now 'cart' has been added twice 
   assert(dict_mostcommon(d)==2);
   // But no new nodes were created
   assert(dict_nodecount(d)==9);
   // Fifth word
   assert(dict_wordcount(d)==5);

   // Pointer to the bottom of cart...
   dict* p = dict_spell(d, "cart");
   assert(p);

   // Add some new words
   dict_addword(d, "par");
   dict_addword(d, "part");
   dict_addword(d, "parted");
   dict_addword(d, "parter");
   // 7 new nodes: p+a+r+t+e+d+r
   assert(dict_nodecount(d)==16);
   // 5 car prefixed + 4 par prefixed
   assert(dict_wordcount(d)==9);
   dict_free(&d);
   assert(d==NULL);
}
