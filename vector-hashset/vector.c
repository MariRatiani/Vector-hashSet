
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SIZE 4;

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    assert(initialAllocation >= 0);

    if(initialAllocation == 0){
        v->allocLen = DEFAULT_SIZE;
    }else {
        v->allocLen = initialAllocation;
        }
    
    assert(elemSize > 0);
    v->elemSize = elemSize;
    v->logLen = 0;
    v->freeFunc = freeFn;
    v->data = malloc(elemSize * v->allocLen);
    assert(v->data != NULL);
}


void VectorDispose(vector *v)
{
    if(v->freeFunc != NULL){
     for(int i = 0; i < v->logLen; i++){
        int curInd = i*v->elemSize;
        v->freeFunc((void*)((char*)(v->data) + curInd));
    }
    }
    free(v->data);
}

int VectorLength(const vector *v){ 
    return v->logLen;
 }


void *VectorNth(const vector *v, int position){
    assert(position >= 0);
    assert(position < (v->logLen));
    char* result = (char*)(v->data) + position*(v->elemSize);
    return (void*) result;
}

void VectorReplace(vector *v, const void *elemAddr, int position){
   
    void* ourElem = VectorNth(v, position);
   
    if(v->freeFunc != NULL)
        v->freeFunc(ourElem);

    memcpy(ourElem, elemAddr, v->elemSize);
}

void grow(vector* v){
    if(v->allocLen == v->logLen){
        v->allocLen = v->allocLen * 2;
        v->data = realloc(v-> data, v->allocLen * v->elemSize); 
        assert(v->data != NULL);
    }
}

void VectorInsert(vector *v, const void *elemAddr, int position){
    assert(elemAddr != NULL);
    assert(position <= v->logLen);
    char* ourElem = (char*)(v->data) + position*(v->elemSize);
    grow(v);

     

    void* dest = (void*)(ourElem + v->elemSize);
    if(position != v->logLen)
        memmove(dest,(void*)ourElem, (v->logLen - position)*v->elemSize);

    memcpy((void*)ourElem, elemAddr, v->elemSize); 
    v->logLen++;

}

void VectorAppend(vector *v, const void *elemAddr){
    assert(elemAddr != NULL);
    grow(v);
    void* addr = (void*)((char*)v->data + (v->logLen)*v->elemSize);
    memcpy(addr, elemAddr, v->elemSize);
    v->logLen++;
}

void VectorDelete(vector *v, int position){
    void* addr= VectorNth(v, position);
    if(v->freeFunc != NULL)
        v->freeFunc(addr);
    v->logLen--;
    if(position != v->logLen){
        void* source = (void*)((char*)addr + v->elemSize);
        int byteN = v->elemSize* (v->logLen - position);
        memmove(addr, source, byteN);
    }
}

void VectorSort(vector *v, VectorCompareFunction compare){
    assert(compare != NULL);
    qsort(v->data, v->logLen, v->elemSize,compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData){   assert(mapFn != NULL);

    assert(mapFn != NULL);
    for(int i = 0; i < v->logLen; i++){
        void* elemAddr = (void*)((char*)v->data + i*v->elemSize);
        mapFn(elemAddr, auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ 
    assert(startIndex >= 0);
    assert(startIndex <= v->logLen);

    int position = startIndex;
    void* curAdd = (void*)((char*)v->data + position*(v->elemSize));
    if(isSorted == false){
        while(position != v->logLen){
        int a = searchFn(curAdd, key);
        if(a == 0)
            return position;
        position++;
        curAdd = (void*)((char*)v->data + position*v->elemSize);
    }
    return kNotFound;
    }else {
        void* res = bsearch(key, curAdd,( v->logLen - startIndex), v->elemSize, searchFn);
        if(res == NULL)
            return kNotFound;
        return ((char*)res - (char*)v->data)/v->elemSize;
    }
    
    return kNotFound; 
} 
