#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"


typedef struct Stack{
	struct GraphNode** accounts;
	int top;
}Stack;

//stack creation of size <size>
Stack* Stack_Create(int size){
	Stack* new=malloc(sizeof(Stack));
	new->accounts=malloc(size*sizeof(struct GraphNode*));
	int iter=0;
	for (iter=0;iter<size;iter++){
		new->accounts[iter]=NULL;
	}
	new->top=-1;//empty stack
	return new;
}

//stack destruction
void Stack_Destroy(Stack* s){
	free(s->accounts);
	free(s);
}

//push
void Stack_Push(Stack* s,struct GraphNode* account){
	s->top++;
	s->accounts[s->top]=account;
}

//pop
int Stack_Pop(Stack* s,struct GraphNode** ret_value){
	if(s->top >= 0){//if not empty
		struct GraphNode* ret=s->accounts[s->top];
		s->accounts[s->top]=NULL;
		s->top--;
		*ret_value=ret;
		return 1;
	}
	else{
		printf("failure:cannot pop->empty stack");
		return 0;
	}	
}


//search for graphnode <account>,return 0 for failure,1 for success
int Stack_Search(Stack* s,struct GraphNode* account){
	if(s->top >= 0){//if not empty
		int iter=0;
		for (iter=0;iter <= (s->top);iter++){
			if(s->accounts[iter] == account){
				return 1;
			}
		}
		return 0;
	}
	else{
		printf("failure:cannot search->empty stack\n");
		return 0;
	}	
}


//return 1 if empty,0 otherwise
int Stack_IsEmpty(Stack* s){
	if(s->top >= 0) return 0;
	else return 1;
}


//return a pointer to the to element
struct GraphNode* Stack_GetTop(Stack* s){
	return s->accounts[s->top];
}


//get size of stack
int Stack_GetSize(Stack* s){
	return (s->top)+1;
}


//print stack
void Stack_Print(Stack* s){
	int iter=0;
	for (iter=0;iter <= s->top;iter++){
		GraphNode_PrintAcc(s->accounts[iter]);
		if(iter != s->top) printf(",");		
	}
	
}
