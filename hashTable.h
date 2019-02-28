

struct node 
{
	char* token;
	struct node* next;
	char* key;
};

typedef struct node* Node ;


struct list
{	
	Node head;
};
typedef struct list* List;



struct hashTable
{
	List rows;
	int no_rows;
};

struct hashTable* hashinit(int no);
int hashFunc(char* key,int no);
void add(struct hashTable* table,char* key,char* token);
bool lookup(struct hashTable* table,char* key);
Node newNode(char* token,char* key);
void listAdd(List lst,Node nd);










