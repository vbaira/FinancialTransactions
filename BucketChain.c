#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Graph.h"
#include "Stack.h"

//Stuct definition
typedef struct BucketChainNode{
	struct GraphNode* vertex;
	unsigned int account;
	struct BucketChainNode* next;
}BucketChainNode;


typedef struct BucketChain{
	BucketChainNode* start;
	int size;
}BucketChain;



//Struct allocation
BucketChain* BucketChain_Alloc(){
	BucketChain* bc=malloc(sizeof(BucketChain));
	bc->start=NULL;
	bc->size=0;
	return bc;
}

//get size
int BucketChain_GetSize(BucketChain* bc){
	return bc->size;
}

//Delete specific Node
//called by BucketChain_Delete()
//not header
int BucketChainNode_Delete(BucketChainNode* bcn){	
	if(GraphNode_Delete(bcn->vertex)==1){//graph connection
		free(bcn);
        return 1;
    }
    else{
        return 0;
    }
}

//chain destruction-deallocation
void BucketChain_Destroy(BucketChain* bc){
	BucketChainNode* todel=bc->start;
	while(todel != NULL){
		bc->start=todel->next;
		GraphNode_Destroy(todel->vertex);//graph connection
        free(todel);
		todel=bc->start;
	}
	free(bc);
}

//empty the chain
//deallocate all its nodes(for bye)
void BucketChain_Clean(BucketChain* bc){
	BucketChainNode* todel=bc->start;
	while(todel != NULL){
		bc->start=todel->next;
		GraphNode_Destroy(todel->vertex);//graph connection
        free(todel);
		todel=bc->start;
	}
	bc->start=NULL;
	bc->size=0;
}

//Insert at start of the chain
void BucketChain_Insert(BucketChain* bc,unsigned int acc){
    BucketChainNode* bcn;//not used
	if(BucketChain_Search(bc,acc,&bcn)==1){
		printf("failure:cannot insert %d->account already exists\n",acc);
		return;
	}
	BucketChainNode* new=malloc(sizeof(BucketChainNode));
	new->account=acc;
	new->vertex=GraphNode_Create(acc);//graph connection
	new->next=bc->start;
	bc->start=new;	
	bc->size++;
}

//Delete node <acc>
void BucketChain_Delete(BucketChain* bc, unsigned int acc){
	BucketChainNode* temp=bc->start;
	BucketChainNode* todel=NULL;
    BucketChainNode* ph=NULL;//placeholder
	if(bc->start == NULL ){// if empty
		printf("failure:Cannot delete %d->account not found\n",acc);
		return;
	}
	if(temp->account == acc){//if first 
		todel=temp;
        ph=todel->next;        
        if(BucketChainNode_Delete(todel)==1){//if graph node deletion is successful
            bc->size--;
		    bc->start=ph;
        }
		return;
	}
	else{
		while (temp->next != NULL ){
			if(temp->next->account == acc){
				todel=temp->next;
                ph=todel->next;
                if(BucketChainNode_Delete(todel)==1){//if graph node deletion is successful
                    bc->size--;
				    temp->next=ph;
                }
				return;
			}
			else{
				temp=temp->next;
			}
		}
	}
	if (todel == NULL){
        printf("failure:Cannot delete %d->account not found\n",acc);	
	}
}

//search for node <acc>,returns 0 if not found ,1 if found,and a pointer to it
int BucketChain_Search(BucketChain* bc,unsigned int acc,BucketChainNode** ret){
	if(bc->start == NULL ){// if empty
		//printf("failure:Cannot search->empty bucket\n");
        *ret=NULL;
		return 0;
	}
	BucketChainNode* search_node=bc->start;
	while (search_node != NULL){
		if(search_node->account == acc ){
			//printf("Success:node found!\n");
            *ret=search_node;
			return 1;
		}
		else{
			search_node=search_node->next;
		}
	}
	//printf("failure:search->account not found!\n");
    *ret=NULL;
	return 0;	
}
 
//add transaction
//op=="out"->outgoing transaction from account
//op=="in"->incoming transaction to account
//existance check from  hashtable level
void BucketChain_AddTran(BucketChainNode* bcn,BucketChainNode* trans_with,double amount,char* op){
    if (strcmp(op,"out")==0){
        GraphNode_AddTranOut(bcn->vertex,trans_with->vertex,amount);   
    }
    else if (strcmp(op,"in")==0){
        GraphNode_AddTranIn(bcn->vertex,trans_with->vertex,amount);
    }
    else printf("failure:cannot add transaction %d %d->invalid operation",bcn->account,trans_with->account);
}


//delete transaction
//op=="out"->outgoing transaction from account
//op=="in"->incoming transaction to account
//existance check from  hashtable level
void BucketChain_DelTran(BucketChainNode* bcn,BucketChainNode* trans_with,char* op){
    if (strcmp(op,"out")==0){
       GraphNode_DelTranOut(bcn->vertex,trans_with->vertex);   
    }
    else if (strcmp(op,"in")==0){
       GraphNode_DelTranIn(bcn->vertex,trans_with->vertex);
    }
    else printf("failure:cannot delete transaction %d %d->invalid operation",bcn->account,trans_with->account);
}


//triangle
void BucketChain_Triangle(BucketChainNode* bcn,double threshold){
	GraphNode_Triangle(bcn->vertex,threshold);
}


//lookup for account of bcn
//operation check from hashtable level
//existance check from hashtable level
void BucketChain_Lookup(BucketChainNode* bcn,char* op){
	GraphNode_Lookup(bcn->vertex,op);
}


//connection
void BucketChain_Conn(BucketChainNode* source,BucketChainNode* dest,int sizeOfGraph){
	GraphNode_Connection(source->vertex,dest->vertex,sizeOfGraph);	
}


//all cycles
void BucketChain_AllCycles(BucketChainNode* source,int sizeOfGraph){
	GraphNode_AllCycles(source->vertex,sizeOfGraph);
}


//traceflow,creates the stack to pass to traceflow of graph level
void BucketChain_Traceflow(BucketChainNode* source,int l){
	struct Stack* parents=Stack_Create(l+1);
	Stack_Push(parents,source->vertex);
	printf("success: traceflow(%d,%d) =\n",source->account,l);
	GraphNode_Traceflow(source->vertex,l,0,parents);
	Stack_Destroy(parents);
}


//print
void BucketChain_Print(BucketChain* bc){
	BucketChainNode* bcn=bc->start;
	while(bcn != NULL){
		GraphNode_Print(bcn->vertex);
		bcn=bcn->next;
	}
}
