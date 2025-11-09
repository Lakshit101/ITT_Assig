#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define BLOCK_SIZE 512
#define MAX_BLOCK 1024

int myStrcmp(const char *s1, const char *s2);
char *myStrcpy(char *dest, const char *src);
size_t myStrcspn(const char *s, const char *reject);
char *myStrtok(char *str, const char *delim);
double myCeil(double x);

typedef struct FreeNode{
    int index;
    struct FreeNode* prev;
    struct FreeNode* next;
}FreeNode;

typedef struct FileNode{
    char name[50];
    int type;
    struct FileNode* parent;
    struct FileNode* child;
    struct FileNode* nextSibling;
    int blockPointerCount;
    int blockPointer[512];
}FileNode;

int gNumberOfFreeNodes=MAX_BLOCK;
FreeNode* gFreeNodeHead=NULL;
FreeNode* gFreeNodeTail=NULL;
char virtualDisk[MAX_BLOCK][BLOCK_SIZE];

FreeNode* createFreeNode(int index);
FreeNode* createFreeNodeList();
FileNode* createFileNode(int type,FileNode* parent , char* name);
void addChild(FileNode* parent , FileNode* newNode);
void lsCommand(FileNode* tail);
void mkdirCommand(FileNode* PWD , char* name);
FileNode* isPresent(FileNode* tail,char* name);
void cdCommand(FileNode** PWD , char* name);
void rmdirCommand(FileNode* PWD,char* name);
void createCommand(FileNode* PWD ,char* name);
void readCommand(FileNode* PWD , char* name);
void pwdCommand(FileNode* PWD ,FileNode* root);
void dfCommand();
void deleteCommand(FileNode* PWD ,char* name);
int allocateMemory();
void deallocateMemory(int index);
void writeCommand(FileNode* PWD , char* name ,char* fileContent);
void freeAllMemory(FileNode* root);
void freeDoublyLinkedList();



int main(){
    gFreeNodeHead=createFreeNodeList();    

    FileNode* root=createFileNode(0,NULL , "/");
    FileNode* PWD=root;

    printf("$ ./vfs\n/ >Compact VFS - ready. Type 'exit' to quit.\n");

    char terminalInput[60];
    while(1){
        printf("%s>",PWD->name);
        fgets(terminalInput ,60 , stdin);

        terminalInput[myStrcspn(terminalInput, "\n")] = 0;

        char* command=myStrtok(terminalInput, " ");
        char* name=myStrtok(NULL, " ");

        if( myStrcmp(command , "exit") == 0){
            printf("Memory released. Exiting program...\n");
            freeAllMemory(root);
            freeDoublyLinkedList();
            free(root);
            exit(0);
        }
        else if( myStrcmp(command , "mkdir") == 0){
            if(name==NULL){
                printf("Please Enter a valid directory name.\n");
            }else
                mkdirCommand( PWD, name);
        }else if( myStrcmp(command , "ls") == 0){
            lsCommand(PWD->child);
        }else if( myStrcmp(command, "cd") == 0){
            if( myStrcmp(name , "..") == 0){
                if(PWD==root){
                    printf("No parent Directory , you are in root.\n");
                }else
                    PWD=PWD->parent;
            }else{
                cdCommand(&PWD , name);
            }
        }else if( myStrcmp(command , "rmdir")==0){
            rmdirCommand(PWD,name);
        }else if( myStrcmp(command , "create") == 0){
            if(name==NULL)
                printf("Please enter a valid file name.\n");
            else    
                createCommand(PWD,name);
        }else if(myStrcmp(command , "pwd")==0){
            pwdCommand(PWD,root);
            printf("\n");
        }else if(myStrcmp(command , "write")==0){
            char* fileContent=myStrtok(NULL ,"\n");
            writeCommand(PWD,name,fileContent);
        }else if(myStrcmp(command, "read")==0){
            readCommand(PWD,name);
        }else if(myStrcmp(command , "df")==0){
            dfCommand();
        }else if(myStrcmp(command , "delete")==0){
            deleteCommand(PWD,name);
        }else{
            printf("Enter a valid command.\n");
        }
    }
}


FileNode* createFileNode(int type,FileNode* parent ,char* name){
    FileNode* temp=(FileNode*)malloc(sizeof(FileNode)); 
    if(temp==NULL){
        printf("Memory allocatio Fail\n");
        free(temp);
        exit(1);
    }
    myStrcpy(temp->name,name);
    temp->type=type;
    temp->parent=parent;
    temp->child=NULL;
    temp->nextSibling=NULL;
    return temp;
}


void addChild(FileNode* parent , FileNode* newNode){
    if(parent->child==NULL){
        parent->child=newNode;
        newNode->nextSibling=newNode;
    }else{
        newNode->nextSibling=parent->child->nextSibling;
        parent->child->nextSibling=newNode;
    }
}

void lsCommand(FileNode* tail){
    if(tail==NULL){
        printf("(empty)\n");
        return;
    }
    FileNode* temp=tail;
    do{
        printf("%s\t",temp->name);
        temp=temp->nextSibling;
    }while(temp!=tail);
    printf("\n");
}

void mkdirCommand(FileNode* PWD ,char *name){
    if(isPresent(PWD->child ,name)==NULL){
        FileNode* newNode=createFileNode( 0 ,PWD ,name);
        addChild(PWD ,newNode);
        printf("Directory %s created successfully\n",name);
    }else{
        printf("Directory with %s already exists.\n",name);
    }
}

FileNode* isPresent(FileNode* tail,char* name){
    if(tail==NULL)
        return NULL;
    
    FileNode* temp=tail;
    do{
        if(myStrcmp(temp->name , name) == 0)
            return temp;
        temp=temp->nextSibling;
    }while(temp!=tail);
    return NULL;
}

void cdCommand(FileNode** PWD , char* name){
    if( (*PWD)->child==NULL ){
        printf("NO %s directory exists",name);
        return ;
    }
    
    FileNode* temp= (*PWD)->child;
    do{
        if(myStrcmp(temp->name , name) == 0){
            *PWD=temp;
            printf("Moved to /%s\n",name);
            return ;
        }
        temp=temp->nextSibling;
    }while(temp!= *PWD);
    return ;
}

FileNode* deleteFileNode(FileNode* tail , FileNode* target){

    FileNode* temp=tail;

    do{
        if(temp->nextSibling==target){
            if(temp->nextSibling == temp){
                free(temp);
                return NULL;
            }else{
                FileNode* toDelete=temp->nextSibling;
                temp->nextSibling=temp->nextSibling->nextSibling;
                free(toDelete);
                break;
            }
        }
        temp=temp->nextSibling;
    }while(temp!=tail);
    return temp;
}

void rmdirCommand(FileNode* PWD,char* name){
    if(PWD->child==NULL){
        printf("No %s directory exists\n",name);
        return;
    }
    FileNode* toDelete=isPresent(PWD->child , name);
    if(toDelete==NULL || toDelete->type==1){
        printf("No %s directory exists\n",name);
        return;
    }

    if(toDelete->child!=NULL){
        printf("Directory is not emply.\n");
        return;
    }
    PWD->child=deleteFileNode(PWD->child,toDelete);
    printf("Directory %s deleted successfully\n",name);
}

void createCommand(FileNode* PWD ,char* name){
    if(isPresent(PWD->child ,name)==NULL){
        FileNode* newNode=createFileNode( 1 ,PWD ,name);
        addChild(PWD ,newNode);
        printf("File %s created successfully\n",name);
    }else{
        printf("File with %s already exists.\n",name);
    }
}

void pwdCommand(FileNode* PWD ,FileNode* root){
    if(PWD==root)
        return;

    pwdCommand(PWD->parent,root);
    printf("/%s",PWD->name);
}

FreeNode* createFreeNode(int index){
    FreeNode* temp=(FreeNode*)malloc(sizeof(FreeNode)); 
    if(temp==NULL){
        printf("Memory allocation fails.\n");
        exit(1);
    }
    temp->index=index;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}

FreeNode* createFreeNodeList(){
    FreeNode* headFN=NULL;
    FreeNode* prev=NULL;

    for(int index=0 ; index< MAX_BLOCK ; index++){
        FreeNode* temp=createFreeNode(index);
        if(headFN==NULL){
            headFN=temp;
            prev=temp;
        }else{
            prev->next=temp;
            temp->prev=prev;
            prev=prev->next;
        }
    }
    gFreeNodeTail=prev;
    return headFN;
}

void writeCommand(FileNode* PWD , char* name , char* fileContent){
    FileNode* file=isPresent(PWD->child , name);
    if(file==NULL || file->type==0){
        printf("No such File exists.\n");
    }else{
        int sizeOfContent=0;
        while( fileContent[sizeOfContent]!='\0')
            sizeOfContent++;

        int BlockNeeded=myCeil((double)sizeOfContent/BLOCK_SIZE);
        
        if(BlockNeeded > gNumberOfFreeNodes){
            printf("Insufficient Memory.\n");
            return;
        }
        for(int block = 0 ;block < file->blockPointerCount ; block++){
            deallocateMemory(file->blockPointer[block]);
        }
        
        file->blockPointerCount=BlockNeeded;

        for(int block = 0 ; block <BlockNeeded ; block++ ){
            file->blockPointer[block]=allocateMemory();
        }
        
        int contentIndex = 0;
        int i; // <--- Corrected declaration position
        for(int block=0 ; block<BlockNeeded ; block++){
            
            // Loop now uses the external 'i'
            for(i = 0 ; contentIndex < sizeOfContent && i<BLOCK_SIZE ; i++){ 
                virtualDisk[file->blockPointer[block]][i]=fileContent[contentIndex++];
            }
            
            if(contentIndex >= sizeOfContent) {
                virtualDisk[file->blockPointer[block]][i] = '\0'; // 'i' is now in scope
            }
        }

        printf("Data written successfully (size = %d bytes)\n",sizeOfContent);
    }
}

void readCommand(FileNode* PWD , char* name){
    FileNode* file=isPresent(PWD->child , name);
    if(file==NULL || file->type==0){
        printf("No such File exists.\n");
    }else{
        for(int block=0 ; block < file->blockPointerCount ; block++){
            for(int i = 0 ; i<BLOCK_SIZE && virtualDisk[file->blockPointer[block]][i]!='\0' ; i++){
                printf("%c", virtualDisk[file->blockPointer[block]][i]);
            }
        }
        printf("\n");
    }

}

void dfCommand(){
    printf("Total Blocks : %d\n",MAX_BLOCK);
    printf("Used Blocks : %d\n",MAX_BLOCK - gNumberOfFreeNodes);
    printf("Free Block : %d\n",gNumberOfFreeNodes);
    printf("Disk Usage : %.2lf%%\n", (double)(MAX_BLOCK - gNumberOfFreeNodes) / MAX_BLOCK * 100.00); 
}

int allocateMemory(){
    int index=gFreeNodeHead->index;
    gFreeNodeHead=gFreeNodeHead->next;
    gNumberOfFreeNodes--;
    if(gFreeNodeHead!=NULL)
        gFreeNodeHead->prev=NULL;
    
    return index;
}

void deleteCommand(FileNode* PWD ,char* name){
    FileNode* file=isPresent(PWD->child , name);
    if(file==NULL || file->type==0){
        printf("No such File exists.\n");
    }else{

        for(int block = 0 ;block < file->blockPointerCount ; block++){
            deallocateMemory(file->blockPointer[block]);
        }

        PWD->child=deleteFileNode(PWD->child,file);
        printf("File %s deleted successfully\n",name);
    }

}

void deallocateMemory(int index){
    FreeNode* toADD=createFreeNode(index);
    gFreeNodeTail->next=toADD;
    toADD->prev=gFreeNodeTail;
    gFreeNodeTail=gFreeNodeTail->next;
    gNumberOfFreeNodes++;
}

void freeAllMemory(FileNode* root){
    if(root==NULL || root->child==NULL)
        return;
    FileNode* temp=root->child;
    do{
        if(temp->child!=NULL){
            freeAllMemory(temp);
        }
        if(temp->type == 1) { 
            for(int block = 0 ;block < temp->blockPointerCount ; block++){
                deallocateMemory(temp->blockPointer[block]);
            }
        }
        
        FileNode* toDelete=temp;
        temp=temp->nextSibling;
        free(toDelete);
    }while(temp!=root->child);
}

void freeDoublyLinkedList(){
    while(gFreeNodeHead!=NULL){
        FreeNode* toDelete=gFreeNodeHead;
        gFreeNodeHead=gFreeNodeHead->next;
        if(gFreeNodeHead!=NULL)
            gFreeNodeHead->prev=NULL;
        free(toDelete);
    }
}

int myStrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char *myStrcpy(char *dest, const char *src) {
    char *original_dest = dest;
    while ((*dest++ = *src++));
    return original_dest;
}

size_t myStrcspn(const char *s, const char *reject) {
    size_t count = 0;
    const char *p;
    while (*s != '\0') {
        p = reject;
        while (*p != '\0') {
            if (*s == *p) {
                return count;
            }
            p++;
        }
        s++;
        count++;
    }
    return count;
}

char *myStrtok(char *str, const char *delim) {
    static char *last = NULL;
    if (str != NULL) {
        last = str;
    }
    if (last == NULL || *last == '\0') {
        return NULL;
    }

    char *token_start = last;
    
    while (*token_start != '\0' && *token_start == *delim) {
        token_start++;
    }
    
    if (*token_start == '\0') {
        last = NULL;
        return NULL;
    }

    char *token_end = token_start;
    while (*token_end != '\0' && *token_end != *delim) {
        token_end++;
    }

    if (*token_end != '\0') {
        *token_end = '\0';
        last = token_end + 1;
    } else {
        last = NULL;
    }

    return token_start;
}

double myCeil(double x) {
    int whole = (int)x;
    if (x > whole) {
        return (double)(whole + 1);
    } else {
        return (double)whole;
    }
}
