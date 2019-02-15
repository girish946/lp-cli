#ifndef PASTE_H 
#define PASTE_H

struct MemoryStruct {
  char *memory;
  size_t size;
};

struct Paste{
    char* id;
    char* name;
    char* content;
    char* timestamp;
};

struct Pastes{
    int length;
    struct Paste* paste_array;
};

void  getJson(char*, struct MemoryStruct*);
#endif
