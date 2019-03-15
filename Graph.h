struct Stack;//forward declaretion
struct GraphNode* GraphNode_Create(unsigned int acc);
int GraphNode_Delete(struct GraphNode* gn);
void GraphNode_Destroy(struct GraphNode* gn);
void GraphNode_AddTranIn(struct GraphNode* gn,struct GraphNode* source,double amount);
void GraphNode_AddTranOut(struct GraphNode* gn,struct GraphNode* dest,double amount);
void GraphNode_DelTranOut(struct GraphNode* gn,struct GraphNode* dest);
void GraphNode_DelTranIn(struct GraphNode* gn,struct GraphNode* source);
void GraphNode_Lookup(struct GraphNode* gn,char*op);
void GraphNode_Print(struct GraphNode* gn);
void GraphNode_Triangle(struct GraphNode* gn,double threshold);
void GraphNode_PrintAcc(struct GraphNode* gn);
void GraphNode_AllCycles(struct GraphNode* source,int sizeOfGraph);
void GraphNode_Traceflow(struct GraphNode* source,int l,double sum,struct Stack* parents);

