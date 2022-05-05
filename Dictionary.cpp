//
// Created by Administrator on 2022/5/2.
//
#include "Dictionary.h"
#include <openssl/sha.h>
#include <cstring>
#include <iostream>
#include <cassert>

bool HashTable::insert(const char *element) {
    int64_t hash[4];
    SHA256(reinterpret_cast<const unsigned char *>(element), strlen(element), reinterpret_cast<unsigned char *>(hash));
    size_t idx=hash[0] % s;
    //cout<<"index: "<<idx<<endl;
    HashNode* current_node=&table[idx];
    assert(current_node!= nullptr);
    if(current_node->value == nullptr){
        current_node->value=strdup(element);
        n++;
    }
    else{
      while(current_node->next != nullptr){
          if(strcmp(current_node->value, element)==0){
              return false;
          }
          current_node = current_node->next;
      }
      if(strcmp(current_node->value, element) == 0){
          return false;
      }
      auto* new_node = new HashNode;
      current_node->next = new_node;
      new_node->next = nullptr;
      new_node->value = strdup(element);
      n++;
    }
    return true;
}

HashTable::HashTable(size_t size):s(size) {
    table = new Node[s];
    //for(int i=0;i<s;i++) table[i] = nullptr;
    //table[512] = nullptr;
}

bool HashTable::find(const char *element) {
    int64_t hash[4];
    SHA256(reinterpret_cast<const unsigned char *>(element), strlen(element), reinterpret_cast<unsigned char *>(hash));
    size_t idx=hash[0] % s;
    HashNode* current_node=&table[idx];
    if(current_node->value == nullptr)
        return false;
    else{
        while(current_node != nullptr){
            if(strcmp(current_node->value, element) == 0)
                return true;
            current_node = current_node->next;
        }
    }
    return false;
}

bool HashTable::erase(const char* element) {
    return false;
}

void HashTable::clear() {
    for(int i=0;i<s;i++){
        HashNode* current = &table[i];
        if(current == nullptr) continue;
        else{
            HashNode* next = current->next;
            while(next != nullptr){
                current = next;
                next = current->next;
                delete current->value;
                current->value = nullptr;
                delete current;
                current = nullptr;
            }
            delete table[i].value;
            table[i].value = nullptr;
            table[i].next = nullptr;
        }
    }
    n=0;
}

HashTable::~HashTable() {
    clear();
}



