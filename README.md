# trie-dict

Dictionary implementations in C for a coursework-style project:
- `t27.c` / `td27.h`: a 27-way trie (letters a–z plus apostrophe) with frequency counts.
- `ext.c` / `ext.h`: a hash-table dictionary using Bernstein’s hash + a secondary hash and resizing.
- `driver.c` and `driverext.c`: simple assertion-based drivers.
- `test.c` and `testext.c`: extra tests used by the builds.

## Build and run
Requires a C99 compiler (e.g., `gcc` or `clang`).

- Build trie binaries: `make`  
  - Optimized: `./t27`  
  - Address/UBSan: `./t27_d`
- Build hash-table binary: `make ext`  
  - Runs the hash implementation tests on start.
- Run the trie binary: `make run` (optimized) or `make rund` (sanitized).
- Clean builds: `make clean`

## Notes
- Overall: both implementations store words, count frequency of repeats, and let you query presence (`dict_spell`), total counts, and the most common word count; tests use asserts to sanity-check behavior.
- Trie: stores words character by character; supports apostrophes; tracks word frequency; includes autocomplete helpers to find the most common suffix below a prefix.
- Hash table: lowercases/normalizes words, allows apostrophes, uses double hashing to resolve collisions, and resizes at a 0.75 load factor.
- The drivers expect wordlist files if you enable the file-loading sections; otherwise the embedded asserts still run.
