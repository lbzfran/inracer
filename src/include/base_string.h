#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base.h"

typedef struct string string;

string* string_create(const char* str);
void string_free(string* s);

char* string_at(string* s);
const char* string_get(string* s);

void string_add(string* s, char c);
void string_remove(string* s, usize pos, usize len);
void string_insert(string* s, usize pos, const char* str);
void string_replace(string* s, usize pos, usize len, const char* str);

usize string_size(string* s);
usize string_get_alloc(string* s);

struct string {
    size_t alloc;
    size_t length;
    char* data;
};


#endif //BASE_STRING_H
