#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"

//struct definition
typedef struct Edge{
 	struct GraphNode* trans_with;
	double trans_amount;
	struct Edge* next;
}Edge;

typedef struct EdgeList{
	Edge* start;
	int size;	
}EdgeList;

typedef struct GraphNode{
	struct EdgeList out;
	struct EdgeList in;
	unsigned int account;
}GraphNode;


//graph node creation and initialization
GraphNode* GraphNode_Create(unsigned int acc){
	GraphNode* new=malloc(sizeof(GraphNode));
	new->account=acc;
	new->out.start=NULL;
	new->out.size=0;
	new->in.start=NULL;
	new->in.size=0;
	printf("success: created %d\n",acc);
	return new;
}


//graph node deletion,returns 1 for success,0 for fail
//<gn> can be deleted only if no incoming or outgoing transactions exist
int GraphNode_Delete(GraphNode* gn){
	if(gn->out.start==NULL && gn->in.start==NULL){
		printf("success: deleted %d\n",gn->account);		
		free(gn);
		return 1;
	}
	else{
		printf("failure:cannot delete %d->transactions exist\n",gn->account);
		return 0;
	}
}

//destroy edgelist
void EdgeList_Destroy(EdgeList* el){
	Edge* todel=el->start;
	while(todel != NULL){
		el->start=todel->next;
        free(todel);
		todel=el->start;
	}
}

//destroy graph node
void GraphNode_Destroy(GraphNode* gn){
	EdgeList_Destroy(&(gn->out));
	EdgeList_Destroy(&(gn->in));
	free(gn);
}

//find transaction with search_acc and return pointer to the 
//transaction's edge
Edge* GraphNode_FindTransaction(EdgeList* el,unsigned int search_acc){
	if(el->start == NULL ){// if empty
		//printf("failure:Cannot find transaction->no transactions\n");
        return NULL;
	}
	Edge* search_node=el->start;
	while (search_node != NULL){
		if(search_node->trans_with->account == search_acc ){
			//printf("Success:transaction found!\n");
            return search_node;
		}
		else{
			search_node=search_node->next;
		}
	}
	//printf("failure:search->transaction not found!\n");
    return NULL;	
}

//add outgoing transaction from <gn> to <dest> 
void GraphNode_AddTranOut(GraphNode* gn,GraphNode* dest,double amount){
	Edge* transaction=NULL;
	transaction=GraphNode_FindTransaction(&(gn->out),dest->account);
	if (transaction==NULL){//transaction not found,add it at start
		Edge* new=malloc(sizeof(Edge));
		new->trans_with=dest;
		new->trans_amount=amount;
		new->next=gn->out.start;
		gn->out.start=new;	
		(gn->out).size++;
		printf("success: added transaction %d %d with amount %.3f\n",
		gn->account,dest->account,new->trans_amount);
	}
	else{//transaction found,update it
		(transaction->trans_amount)+=amount;
		printf("success: added transaction %d %d with amount %.3f\n",
		gn->account,dest->account,amount);
	}	
}

//add incoming transaction to <gn> from <source>
void GraphNode_AddTranIn(GraphNode* gn,GraphNode* source,double amount){
	Edge* transaction=NULL;
	transaction=GraphNode_FindTransaction(&(gn->in),source->account);
	if (transaction==NULL){//transaction not found,add it at start
		Edge* new=malloc(sizeof(Edge));
		new->trans_with=source;
		new->trans_amount=amount;
		new->next=gn->in.start;
		gn->in.start=new;	
		(gn->in).size++;
		//printf("success: added transaction %d %d(IN) with amount %.3f\n",
		//source->account,gn->account,gn->in.start->trans_amount);		
	}
	else{//transaction found,update it
		(transaction->trans_amount)+=amount;
		//printf("success: added transaction %d %d(IN UPDATE) with amount %.3f\n",
		//source->account,gn->account,transaction->trans_amount);
	}		
}

//delete outgoing transaction from <gn> to <dest> 
void GraphNode_DelTranOut(GraphNode* gn,GraphNode* dest){
	Edge* todel=NULL;
	todel=GraphNode_FindTransaction(&(gn->out),dest->account);
	if (todel==NULL){//transaction not found,error
		printf("failure:cannot delete transaction %d %d->transaction not found\n",gn->account,dest->account);
	}
	else{//transaction found,delete it
		Edge* prev=gn->out.start;
		if( prev == todel ){//if first 
			gn->out.start=todel->next;
			free(todel);
			gn->out.size--;
			printf("success: deleted transaction %d %d\n",gn->account,dest->account);
			return;		
		}
		else{
			while (prev->next != NULL ){
				if(prev->next == todel){
		            prev->next=todel->next;
					free(todel);
					gn->out.size--;
					printf("success: deleted transaction %d %d\n",gn->account,dest->account);
					return;	
				}
				else{
					prev=prev->next;
				}
			}
		}
	}
}

//delete incoming transaction to <gn> from <source>
void GraphNode_DelTranIn(GraphNode* gn,GraphNode* source){
	Edge* todel=NULL;
	todel=GraphNode_FindTransaction(&(gn->in),source->account);
	if (todel==NULL){//transaction not found,error
		//printf("failure:cannot delete transaction(IN)->transaction not found\n");
	}
	else{//transaction found,delete it
		Edge* prev=gn->in.start;
		if( prev == todel ){//if first 
			gn->in.start=todel->next;
			free(todel);
			gn->in.size--;
			//printf("success: deleted transaction %d %d (IN)\n",source->account,gn->account);
			return;		
		}
		else{
			while (prev->next != NULL ){
				if(prev->next == todel){
		            prev->next=todel->next;
					free(todel);
					gn->in.size--;
					//printf("success: deleted transaction %d %d (IN)\n",source->account,gn->account);
					return;	
				}
				else{
					prev=prev->next;
				}
			}
		}
	}
}

//finds all the triangles <gn1> is invloved where each 
//transaction is grater than threshold
void GraphNode_Triangle(GraphNode* gn1,double threshold){
	printf("success: triangle(%d,%.3f)=\n",gn1->account,threshold);
	Edge* e1=gn1->out.start;
	while(e1 != NULL){
		if(e1->trans_amount >= threshold){
			GraphNode* gn2=e1->trans_with;
			Edge* e2=gn2->out.start;
			while(e2 != NULL){
				if(e2->trans_amount >= threshold){
					GraphNode* gn3=e2->trans_with;
					Edge* e3=gn3->out.start;
					while(e3 != NULL){
						if(e3->trans_amount >= threshold){
							if(e3->trans_with == gn1){
								printf("(%d,%d,%d)\n",gn1->account,gn2->account,gn3->account);
							}
						}
						e3=e3->next;
					}
				}
				e2=e2->next;
			}
		}
		e1=e1->next;
	}
}


//connection between <source> and <dest>
//existance check upper levels
void GraphNode_Connection(GraphNode* source,GraphNode* dest,int sizeOfGraph){
	struct Stack* parents=Stack_Create(sizeOfGraph);//create a stack for parents//remember destroy
	struct Stack* discovered=Stack_Create(sizeOfGraph);//create a stack to store discovered nodes//remember destroy
	Stack_Push(parents,source);
	Stack_Push(discovered,source);	
	while(Stack_IsEmpty(parents)==0){//while parents stack is not empty
		GraphNode* top=Stack_GetTop(parents);
		Edge* transactions=top->out.start;
		GraphNode* neighbour;
		while (transactions != NULL){//for all neighbours
			neighbour=transactions->trans_with;
			if(Stack_Search(discovered,neighbour)==1){//if already discovered
				transactions=transactions->next;//go to next neighbour
			}
			else{//if not discovered
				Stack_Push(parents,neighbour);
				Stack_Push(discovered,neighbour);
				if(neighbour == dest){//found dest
					printf("success: conn(%d,%d) = ",source->account,dest->account);
					printf("(");
					Stack_Print(parents);
					printf(")\n");
					Stack_Destroy(parents);
					Stack_Destroy(discovered);
					return;
				}
				goto end;//break out of inner while without popping		
			}		
		}
		//all neighbours of current top discovered or no neighbours
		Stack_Pop(parents,&neighbour);
		end: ;	
	}
	printf("success: conn(%d,%d) = not found\n",source->account,dest->account);
	Stack_Destroy(parents);
	Stack_Destroy(discovered);
}


//find all cycles <source> is involved
void GraphNode_AllCycles(GraphNode* source,int sizeOfGraph){
	int found=0;
	struct Stack* parents=Stack_Create(sizeOfGraph);//create a stack for parents//remember destroy
	struct Stack* discovered=Stack_Create(sizeOfGraph);//create a stack to store discovered nodes//remember destroy
	Stack_Push(parents,source);
	Stack_Push(discovered,source);	
	while(Stack_IsEmpty(parents)==0){//while parents stack is not empty
		GraphNode* top=Stack_GetTop(parents);
		Edge* transactions=top->out.start;
		GraphNode* neighbour;
		while (transactions != NULL){//for all neighbours
			neighbour=transactions->trans_with;
			if(Stack_Search(discovered,neighbour)==1){//if already discovered
				transactions=transactions->next;//go to next neighbour
			}
			else{//if not discovered
				Stack_Push(parents,neighbour);
				Stack_Push(discovered,neighbour);
				Edge* find_source=GraphNode_FindTransaction(&(neighbour->out),source->account);
				if(find_source != NULL){//found cycle
					if(Stack_GetSize(parents) >= 3){//at least 3 nodes involved
						if (found == 0){//first cycle found
							printf("success: cycles %d = \n",source->account);
						}
						found=1;
						printf("(");
						Stack_Print(parents);
						printf(")\n");	
					}
				}
				goto end;//break out of inner while without popping	
			}		
		}
		//all neighbours of current top discovered or no neighbours
		Stack_Pop(parents,&neighbour);
		end: ;	
	}
	if (found==0) printf("success: no cycle found for %d\n",source->account);
	Stack_Destroy(parents);
	Stack_Destroy(discovered);
}



//find all the paths of length <l> starting from <source>
//and calculate the amount of money for each path
//called recursively
void GraphNode_Traceflow(GraphNode* source,int l,double sum,struct Stack* parents){
	if (l==0){//path of lenght l,print it
		printf("(");
		Stack_Print(parents);
		printf(",%.3f)\n",sum);
		return;	
	}
	Edge* transactions=source->out.start;
	while ((transactions != NULL)){//for all neighbours
		GraphNode* neighbour=transactions->trans_with;
		if(Stack_Search(parents,neighbour)==1){//if neighbour is already in path go to next one
			transactions=transactions->next;
			//continue;
		}
		else{//if not,add in path,update sum and call traceflow for it 
			Stack_Push(parents,neighbour);
			sum+=(transactions->trans_amount);
			GraphNode_Traceflow(neighbour,l-1,sum,parents);
			Stack_Pop(parents,&neighbour);//when done with neighbour ,pop stack,update sum and go to next
			if (Stack_IsEmpty(parents)==0){
				GraphNode* top=Stack_GetTop(parents);
				Edge* find_trans=GraphNode_FindTransaction(&(top->out),neighbour->account);
				sum-=(find_trans->trans_amount);
			}
			transactions=transactions->next;//go to next neighbour
		}
		//transactions=transactions->next;	
	}	
}


//lookup for <gn>
//op=="in" print incoming amount of money
//op=="out" print outcoming amount of money
//op=="sum" print in-out
void GraphNode_Lookup(GraphNode* gn,char*op){
	if(strcmp(op,"in")==0){//add all incoming transactions
		double sum=0;
		Edge* transaction;
		transaction=gn->in.start;
		while(transaction != NULL){
			sum+=(transaction->trans_amount);
			transaction=transaction->next;
		}
		printf("success:in(%d) = %.3f\n",gn->account,sum);
	}
	else if(strcmp(op,"out")==0){//add all outgoing transactions
		double sum=0;
		Edge* transaction;
		transaction=gn->out.start;
		while(transaction != NULL){
			sum+=(transaction->trans_amount);
			transaction=transaction->next;
		}
		printf("success:out(%d) = %.3f\n",gn->account,sum);
	}
	else{//op==sum,in-out
		double sum_in=0;
		double sum_out=0;
		Edge* transaction;
		transaction=gn->in.start;
		while(transaction != NULL){
			sum_in+=(transaction->trans_amount);
			transaction=transaction->next;
		}
		transaction=gn->out.start;
		while(transaction != NULL){
			sum_out+=(transaction->trans_amount);
			transaction=transaction->next;
		}
		printf("success:sum(%d) = %.3f\n",gn->account,(sum_in-sum_out));		
	}	
}

//print transactions of <gn>
void GraphNode_Print(GraphNode* gn){
	printf("vertex(%d)=",gn->account);
	Edge* transaction;
	transaction=gn->out.start;
	while(transaction != NULL){
		printf("(%d , %.3f),",transaction->trans_with->account,transaction->trans_amount);
		transaction=transaction->next;
	}
	printf("\n");	
}


//print <gn> account
void GraphNode_PrintAcc(GraphNode* gn){
	printf("%d ",gn->account);
}
