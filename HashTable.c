#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BucketChain.h"

//Hash Table definition
typedef struct HashTable{
	struct BucketChain** Bucket;
	int size;
}HashTable;


//hash table creation
HashTable* HashTable_Create(int s){
	HashTable* new=malloc(sizeof(HashTable));
	new->size=s;
	new->Bucket=malloc(s*sizeof(struct BucketChain*));
	int iter=0;
	for(iter=0;iter<s;iter++){
		new->Bucket[iter]=BucketChain_Alloc();
	}
	return new;	
}


//Hash function
//source:https://gist.github.com/badboy/6267743
int HashFunction(HashTable* ht,unsigned int key){
	key = ~key + (key << 15);
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; 
	key = key ^ (key >> 16);
  	return key%(ht->size);	
}


//get number of nodes currently in hash table
int HashTable_GetNoN(HashTable* ht){
	int iter;
	int sum=0;
	for (iter=0;iter<(ht->size);iter++){
		sum+=BucketChain_GetSize(ht->Bucket[iter]);
	}
	//printf("Number of Nodes:%d\n",sum);
	return sum;
}


//hash table insertion(CREATE NODES)
void HashTable_Insert(HashTable* ht,unsigned int key){
	int pos=HashFunction(ht,key);
	//printf("position:%d\n",pos);
	BucketChain_Insert(ht->Bucket[pos],key);
}

//hash table deletion(DELNODES)
void HashTable_Delete(HashTable* ht,unsigned int key){
	int pos=HashFunction(ht,key);
	//printf("position:%d\n",pos);
	BucketChain_Delete(ht->Bucket[pos],key);
}


//hash table destruction
//deallocation of the whole hash table
void HashTable_Destroy(HashTable* ht){
	int iter;
	for (iter=0;iter<(ht->size);iter++){
		BucketChain_Destroy(ht->Bucket[iter]);
	}
	free(ht->Bucket);
	free(ht);
}


//hash table cleanup(bye)
//empty out the hash table(delete all the graph nodes)
void HashTable_Clean(HashTable* ht){
	int iter;
	for (iter=0;iter<(ht->size);iter++){
		BucketChain_Clean(ht->Bucket[iter]);
	}
	printf("success: cleaned memory\n");
}


//add transaction from acc1 to acc2 
void HashTable_AddTran(HashTable* ht,unsigned int acc1,unsigned int acc2,double amount){
	struct BucketChainNode* acc1_ptr;
	struct BucketChainNode* acc2_ptr;
	int pos1=HashFunction(ht,acc1);
	int pos2=HashFunction(ht,acc2);
	int found1=BucketChain_Search(ht->Bucket[pos1],acc1,&acc1_ptr);
	int found2=BucketChain_Search(ht->Bucket[pos2],acc2,&acc2_ptr);
	if (found1==1 && found2==1){//if both accounts exist
		BucketChain_AddTran(acc1_ptr,acc2_ptr,amount,"out");//add outgoing transaction
		BucketChain_AddTran(acc2_ptr,acc1_ptr,amount,"in");//add incoming transaction
	}
	else{//if one account does not exist
		printf("failure:cannot add transaction %d %d->account does not exist\n",acc1,acc2);
	}
}

//delete transaction from acc1 to acc2 
void HashTable_DelTran(HashTable* ht,unsigned int acc1,unsigned int acc2){
	struct BucketChainNode* acc1_ptr;
	struct BucketChainNode* acc2_ptr;
	int pos1=HashFunction(ht,acc1);
	int pos2=HashFunction(ht,acc2);
	int found1=BucketChain_Search(ht->Bucket[pos1],acc1,&acc1_ptr);
	int found2=BucketChain_Search(ht->Bucket[pos2],acc2,&acc2_ptr);
	if (found1==1 && found2==1){//if both accounts exist
		BucketChain_DelTran(acc1_ptr,acc2_ptr,"out");//delete outgoing transaction
		BucketChain_DelTran(acc2_ptr,acc1_ptr,"in");//delete incoming transaction	
	}
	else{//if one account does not exist
		printf("failure:cannot delete transaction %d %d->account does not exist\n",acc1,acc2);
	}
}

//find triangles which acc is involved
void HashTable_Triangle(HashTable* ht,unsigned int acc,double threshold){
	if (threshold < 0){
		printf("failure:cannot find triangle %d->negative threshold\n",acc);
		return;	
	}
	struct BucketChainNode* acc_ptr;
	int pos=HashFunction(ht,acc);
	int found=BucketChain_Search(ht->Bucket[pos],acc,&acc_ptr);
	if(found==1){//if account exists
		BucketChain_Triangle(acc_ptr,threshold);
	}
	else{//if account does not exist
		printf("failure:cannot find triangle %d->accout does not exist\n",acc);
	}
}


//lookup in/out/sum
void HashTable_Lookup(HashTable* ht,unsigned int acc,char* op){
	if((strcmp(op,"in")!=0) && (strcmp(op,"out")!=0) && (strcmp(op,"sum")!=0)){
		printf("failure:cannot complete lookup %d->invalid operation\n",acc);
		return;
	}
	struct BucketChainNode* acc_ptr;
	int pos=HashFunction(ht,acc);
	int found=BucketChain_Search(ht->Bucket[pos],acc,&acc_ptr);
	if(found==1){//if account exists
		BucketChain_Lookup(acc_ptr,op);
	}
	else{
		printf("failure:cannot complete lookup %d->account does not exist\n",acc);
	}
}
 

//find connection between acc1 and acc2
void HashTable_Conn(HashTable* ht,unsigned int acc1,unsigned int acc2){
	struct BucketChainNode* acc1_ptr;
	struct BucketChainNode* acc2_ptr;
	int pos1=HashFunction(ht,acc1);
	int pos2=HashFunction(ht,acc2);
	int found1=BucketChain_Search(ht->Bucket[pos1],acc1,&acc1_ptr);
	int found2=BucketChain_Search(ht->Bucket[pos2],acc2,&acc2_ptr);
	if (found1==1 && found2==1){//if both accounts exist
		int size=HashTable_GetNoN(ht);
		BucketChain_Conn(acc1_ptr,acc2_ptr,size);
	}
	else{//if one account does not exist
		printf("failure:cannot find connection %d %d->account does not exist\n",acc1,acc2);
	}

}


//all cycles acc is involved
void HashTable_AllCycles(HashTable* ht,unsigned int acc){
	struct BucketChainNode* acc_ptr;
	int pos=HashFunction(ht,acc);
	int found=BucketChain_Search(ht->Bucket[pos],acc,&acc_ptr);
	if(found==1){//if account exists
		int size=HashTable_GetNoN(ht);
		BucketChain_AllCycles(acc_ptr,size);
	}
	else{
		printf("failure:cannot find cycles->account %d does not exist\n",acc);
	}
}


//traceflow
void HashTable_Traceflow(HashTable *ht,unsigned int acc,int l){
	if(l<0){
		printf("failure:cannot complete traceflow %d->negative lenght\n",acc);
		return;
	}
	struct BucketChainNode* acc_ptr;
	int pos=HashFunction(ht,acc);
	int found=BucketChain_Search(ht->Bucket[pos],acc,&acc_ptr);
	if(found==1){//if account exists
		
		BucketChain_Traceflow(acc_ptr,l);
	}
	else{
		printf("failure:cannot complete traceflow->account %d does not exist\n",acc);
	}
}


//print
void HashTable_Print(HashTable* ht){
	int iter;
	for (iter=0;iter<(ht->size);iter++){
		BucketChain_Print(ht->Bucket[iter]);
	}	
}

