

void DoSecondPass(char*, objCode* , symbolTable* , definitionTable*, int);
int findSymbol(symbolTable*, char*);
int findDefinition(definitionTable*, char*);
int evaluateNum(int, objCode*);
int evaluate(char*, symbolTable*, objCode*);
