
#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	assert(elemSize > 0);
	assert(numBuckets > 0);
	assert(hashfn != NULL);
	assert(comparefn != NULL);
	
	h->elemSize = elemSize;
	h->bucketSize = numBuckets;
	h->numElems = 0;
	h->hashFn = hashfn;
	h->compFn = comparefn;
	h->freeFn = freefn;
	h->data = (vector**)malloc(sizeof(vector*) * h->bucketSize);
	assert(h->data != NULL);

	vector** data = h->data;
	for(int i = 0; i < h->bucketSize; i++){
		vector* v;
		v = malloc(sizeof(vector));
		*data = v;
		VectorNew(*data, h->elemSize, freefn, 5);
		data++;
		}
}

void HashSetDispose(hashset *h){
	for(int i = 0; i < h->bucketSize; i++){
		VectorDispose((h->data)[i]);
		free((void*)((h->data)[i]));
	}
	free((void*)h->data);
}

int HashSetCount(const hashset *h){
	 return h->numElems; 
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData){
	assert(mapfn != NULL);
	for(int i = 0; i < h->bucketSize; i++){
		vector* currVector = h->data[i];
		for(int j = 0; j < currVector->logLen; j++){
			mapfn(VectorNth(h->data[i], j), auxData);
		}
	}
}







void HashSetEnter(hashset *h, const void *elemAddr){
	assert(elemAddr != NULL);
	
	int index = h->hashFn(elemAddr, h->bucketSize);
	assert(index >= 0);
	assert(index < h->bucketSize);
	vector* curV = h->data[index];
	if(VectorLength(curV) != 0){
		int searchElem = VectorSearch(h->data[index], elemAddr, h->compFn, 0, true);
	if(searchElem == -1){
	VectorAppend(h->data[index], elemAddr);
	VectorSort(h->data[index], h->compFn);
		h->numElems++;
	} else VectorReplace(h->data[index], elemAddr, searchElem);
	}else {
		VectorInsert(h->data[index], elemAddr, 0);
		h->numElems++;
	}
}


void *HashSetLookup(const hashset *h, const void *elemAddr){
	assert(elemAddr != NULL);
	//sworad viyeneb am funqcias?
	int index = h->hashFn(elemAddr, h->bucketSize);
	assert(index >= 0);
	assert(index < h->bucketSize);
	vector* ourVec = h->data[index];

	int search = VectorSearch(ourVec, elemAddr, h->compFn, 0, false);
	if(search == -1)
		return NULL; 

	void* addr=VectorNth(ourVec, search);
	return addr;	
}
