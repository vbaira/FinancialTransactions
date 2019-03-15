struct HashTable* HashTable_Create(int s);
int HashFunction(struct HashTable* ht,unsigned int key);
void HashTable_Insert(struct HashTable* ht,unsigned int key);
void HashTable_Delete(struct HashTable* ht,unsigned int key);
void HashTable_Destroy(struct HashTable* ht);
void HashTable_AddTran(struct HashTable* ht,unsigned int from,unsigned int to,double amount);
void HashTable_DelTran(struct HashTable* ht,unsigned int acc1,unsigned int acc2);
void HashTable_Lookup(struct HashTable* ht,unsigned int acc,char* op);
void HashTable_Clean(struct HashTable* ht);
void HashTable_Print(struct HashTable* ht);
void HashTable_Triangle(struct HashTable* ht,unsigned int acc,double threshold);
void HashTable_Conn(struct HashTable* ht,unsigned int acc1,unsigned int acc2);
int HashTable_GetNoN(struct HashTable* ht);
void HashTable_AllCycles(struct HashTable* ht,unsigned int acc);
void HashTable_Traceflow(struct HashTable *ht,unsigned int acc,int l);
