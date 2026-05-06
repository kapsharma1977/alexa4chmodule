#pragma once

#include "macros.h"

class encryp {
  char *key;
  char xor_encrypt(char c, uint8_t at_i_key){return c^key[at_i_key];}
  char xor_decrypt(char encrypted_c, uint8_t at_i_key){return encrypted_c^key[at_i_key];}
  public:
    encryp();
    ~encryp(){if(IsKey()){free(key);ESP.getFreeHeap();}}//free(str);free(dstr);}
    bool IsKey(){return key != NULL ? true : false;}
    bool xor_encrypt(char *s, uint8_t l);// char *s is plain text
    bool xor_decrypt(char *crypted, uint8_t l);
    String Key(){return IsKey() ? String(key) : "";}
    char charAt(uint8_t i){return key[i];}
};