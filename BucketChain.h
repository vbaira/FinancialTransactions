struct BucketChainNode;//forward declaration
struct BucketChain* BucketChain_Alloc();
int BucketChain_GetSize(struct BucketChain* bc);
void BucketChain_Destroy(struct BucketChain* bc);
void BucketChain_Insert(struct BucketChain* bc,unsigned int acc);
void BucketChain_Delete(struct BucketChain* bc,unsigned int acc);
int BucketChain_Search(struct BucketChain* bc,unsigned int acc,struct BucketChainNode **ret);
void BucketChain_AddTran(struct BucketChainNode* bcn,struct BucketChainNode* trans_with,double amount,char* op);
void BucketChain_DelTran(struct BucketChainNode* bcn,struct BucketChainNode* trans_with,char* op);
void BucketChain_Clean(struct BucketChain*bc);
void BucketChain_Print(struct BucketChain* bc);
void BucketChain_Triangle(struct BucketChainNode* bcn,double threshold);
void BucketChain_Conn(struct BucketChainNode* source,struct BucketChainNode* dest,int sizeOfGraph);
void BucketChain_AllCycles(struct BucketChainNode* source,int sizeOfGrap);
void BucketChain_Traceflow(struct BucketChainNode* source,int l);
