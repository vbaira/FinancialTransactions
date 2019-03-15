#include "HashTable.h"
#include <stdio.h>
#include <string.h>

void Input(FILE * file,struct HashTable* ht);

int main(int argc,char* argv[]){
	/****Input check*******/
	if((argc != 3) && (argc !=5)){
		printf("failure: application call should be something like:\n"
		"\t./elegxos -o Operations -b HashTableEntries\n"
		"\t./elegxos -b HashTableEntries -o Operations\n"
		"\t./elegxos -b HashTableEntries\n");
		return;
	}
	if (argc == 3){
		if (strcmp(argv[1],"-b") != 0){
			printf("failure:application should be called with flag \"-b\"\n");
			return;
		}
	}
	if (argc == 5){//success condition T=((1=-b and 3=-o) or (1=-o and 3=-b)),De morgan for ~T
		if( ( (strcmp(argv[1],"-b")!=0) || (strcmp(argv[3],"-o")!=0) ) 
			&& ( (strcmp(argv[1],"-o")!=0) || (strcmp(argv[3],"-b")!=0) ) )
		{
			printf("failure:application should be called with flags \"-b\" and \"-o\" \n");
			return;	
		}
	}
	/*****************/
	struct HashTable* ht;
	int size;
	if(argc == 3){//get HashTableEntries 
		size=atoi(argv[2]);
	}
	else{
		if((strcmp(argv[1],"-b")==0)) size=atoi(argv[2]);
		else size=atoi(argv[4]);
	}
	ht=HashTable_Create(size);//create hashtable
	if(argc == 5){//if there is an operation file use it
		FILE* operations;
		if((strcmp(argv[1],"-o")==0)) operations=fopen(argv[2],"r");
		else operations=fopen(argv[4],"r");//remember fclose
		Input(operations,ht);
		fclose(operations);
	}
	Input(stdin,ht);//control passed to user
	HashTable_Destroy(ht);//destroy hash table
}


//Function to proccess input data
void Input(FILE * file,struct HashTable* ht){	
	char buf[300];
	char* token;
	char *delimiters=" \n";
	while(fgets(buf,sizeof(buf),file)!=NULL)//read lines until EOF(or ctrl+D)
	{
		token=strtok(buf,delimiters);//first word(command)
		/*for each command, input check for arguments and if everything is ok
		call the right function from the HashTable interface*/
		if (strcmp(token,"createnodes")==0){//createnodes N1 N2 N3....
			while((token=strtok(NULL,delimiters))!=NULL){
				if (strlen(token) != 7){
					printf("failure:cannot create node %s->ID must be a 7 digit number\n",token);
				}
				else{
					if(atoi(token)!=0){
						HashTable_Insert(ht,atoi(token));
					}
					else{//not convertible
						printf("failure:cannot create node %s->ID must be a number\n",token);
					}
				}
			}
		}		
		else if (strcmp(token,"delnodes")==0){//delnodes N1 N2 N3 N4 ....
			while((token=strtok(NULL,delimiters))!=NULL){
				if (strlen(token) != 7){
					printf("failure:cannot delete node %s->ID must be a 7 digit number\n",token);
				}
				else{
					if(atoi(token)!=0){
						HashTable_Delete(ht,atoi(token));
					}
					else{//not convertible
						printf("failure:cannot delete node %s->ID must be a number\n",token);
					}
				}
			}
		}
		else if (strcmp(token,"addtran")==0){//addtran N1 N2 amount
			int error=0;
			int acc1,acc2;
			int amount;
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if (arg_num==1 || arg_num==2){
					if (strlen(token) != 7){
						printf("failure:cannot add transaction ->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							if(arg_num==1)acc1=atoi(token);
							else acc2=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot add transaction->ID must be a number\n");
							error=1;
							break;
						}
					}
				}
				if(arg_num==3){
					amount=atoi(token);
				}
				arg_num++;
			}

			if (error !=1){//if all input is ok
				HashTable_AddTran(ht,acc1,acc2,(double)amount);
			}		
		}
		else if (strcmp(token,"deltran")==0){//deltran N1 N2
			int error=0;
			int acc1,acc2;
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if (arg_num==1 || arg_num==2){
					if (strlen(token) != 7){
						printf("failure:cannot delete transaction ->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							if(arg_num==1)acc1=atoi(token);
							else acc2=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot delete transaction ->ID must be a number\n");
							error=1;
							break;
						}
					}
				}
				arg_num++;
			}

			if (error !=1){//if all input is ok
				HashTable_DelTran(ht,acc1,acc2);
			}		
		}
		else if (strcmp(token,"lookup")==0){//lookup [in | out | sum] N
			int error=0;
			int acc;
			char op[3];
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if(arg_num==1){
					strcpy(op,token);
				}
				if(arg_num==2){
					if (strlen(token) != 7){
						printf("failure:cannot complete lookup->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							acc=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot complete lookup->ID must be a number\n");
							error=1;
							break;
						}
					}
				}			
				arg_num++;
			}			
			if (error !=1){//if all input is ok
				HashTable_Lookup(ht,acc,op);
			}				
		}
		else if (strcmp(token,"triangle")==0){//triangle N k
			int error=0;
			int acc;
			int threshold;
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if (arg_num==1){
					if (strlen(token) != 7){
						printf("failure:cannot find triangle->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							acc=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot find triangle->ID must be a number\n");
							error=1;
							break;
						}
					}
				}
				if (arg_num==2) threshold=atoi(token);
				arg_num++;
			}

			if (error !=1){//if all input is ok
				HashTable_Triangle(ht,acc,(double)threshold);
			}	
		}
		else if (strcmp(token,"conn")==0){//conn N1 N2
			int error=0;
			int acc1,acc2;
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if (arg_num==1 || arg_num==2){
					if (strlen(token) != 7){
						printf("failure:cannot find connection->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							if(arg_num==1)acc1=atoi(token);
							else acc2=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot find connection->ID must be a number\n");
							error=1;
							break;
						}
					}
				}				
				arg_num++;
			}
			if (error !=1){//if all input is ok
				HashTable_Conn(ht,acc1,acc2);
			}	
			
		}
		else if (strcmp(token,"allcycles")==0){//allcycles N
			int error=0;
			int acc;
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if(arg_num==1){
					if (strlen(token) != 7){
						printf("failure:cannot find cycles->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							acc=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot find cycles->ID must be a number\n");
							error=1;
							break;
						}
					}
				}
				arg_num++;
			}
			if (error !=1){//if all input is ok
				HashTable_AllCycles(ht,acc);
			}	
		}
		else if (strcmp(token,"traceflow")==0){//traceflow N l
			int error=0;
			int acc;
			int length;
			int arg_num=1;
			while((token=strtok(NULL,delimiters))!=NULL){
				if(arg_num==1){
					if (strlen(token) != 7){
						printf("failure:cannot complete traceflow->ID must be a 7 digit number\n");
						error=1;
						break;
					}
					else{
						if(atoi(token)!=0){
							acc=atoi(token);
						}
						else{//not convertible
							printf("failure:cannot complete traceflow->ID must be a number\n");
							error=1;
							break;
						}
					}
				}
				if(arg_num==2){
					length=atoi(token);
					if(length <= 0){
						printf("failure:cannot complete traceflow->length should be positive\n");
							error=1;
							break;
					}
				}
				arg_num++;	
			}
			if (error !=1){//if all input is ok
				HashTable_Traceflow(ht,acc,length);
			}			
		}
		else if (strcmp(token,"bye")==0){//bye
			HashTable_Clean(ht);
		}
		else if (strcmp(token,"print")==0){//print
			HashTable_Print(ht);
		}
		else if (strcmp(token,"dump")==0){//dumb(not implemented)
			printf("DUMP NOT IMPLEMENTED\n");
		}
		else{
			printf("failure:not a valid command\n");
		}		
	}
}

