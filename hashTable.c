
#include<stdbool.h>
#include<stdlib.h>
#include "hashTable.h"
#include<stdio.h>
#include<string.h>

Node newNode(char* token,char* key)
{
	Node temp = (Node)malloc(sizeof(struct node));
	int length_tok = 0;
	int length_key = 0;
	while(token[length_tok] != '\0')
	length_tok++;
	while(key[length_key] != '\0')
	length_key++;

	temp->next = NULL;
	temp->token = (char*)malloc(sizeof(char)*(length_tok+1));
	temp->key = (char*)malloc(sizeof(char)*(length_key+1));
	int i = 0;
	while(i<length_tok)
	{
		temp->token[i] = token[i];
		i++;
	}
	temp->token[i] = '\0';
	i = 0;
	while(i<length_key)
	{
		temp->key[i] = key[i];
		i++;
	}
	temp->key[i] = '\0';

	return temp;
}


struct hashTable* hashinit(int no)
{
	struct hashTable* table = (struct hashTable*)malloc(sizeof(struct hashTable));
	table->no_rows = no;
	table->rows = (List)malloc(sizeof(struct list)*no);
	int i = 0;
	for(i;i<no;i++)
	{
		table->rows[i].head = NULL;
	}

	return table;

}

int hashFunc(char* key,int no)
{
	int length = 0;
	while(key[length] != '\0')
	length++;

	int i = 0;
	long long value = 0;
	for(i;i<length;i++)
	{
		value = 53*value + key[i];
	}
	value = value % no;
	//printf("\n values and no is %lld   %d \n",value,no);
	return value;
}

void listAdd(List lst,Node nd)
{
	Node tmp = lst->head;
	if(tmp == NULL)
	{
		lst->head = nd;
		return;
	}
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	tmp->next = nd;
	printf("head is %s",lst->head->key);
	return;
}

bool lookup(struct hashTable* table,char* key)
{
	int index = hashFunc(key,table->no_rows);
	Node temp = table->rows[index].head;
	while(temp != NULL)
	{
		if(strcmp(temp->key,key) == 0)
		return true;
		temp = temp->next;
	}
	return false;
}




void add(struct hashTable* table,char* key,char* token)
{
	int index = hashFunc(key,table->no_rows);
	printf("index while adding is %d \n",index);

	if(lookup(table,key) == true)
	return;
	else
	{
		listAdd(&(table->rows[index]),newNode(token,key));
	}
	return;
}

int main()
{
	struct hashTable* table = hashinit(2);
	printf("No. of rows %d\n",table->no_rows);
	add(table,"abc","TK_abc");
	add(table,"def","def");
	add(table,"edf","eff");
	add(table,"list","TK_LIST");
	//printf("\n the value in 5 is table->rows[1] is %s ",table->rows[1].head->key);
	if(lookup(table,"abc"))
	{
		printf("abc is present");
	}
	if(lookup(table,"def"))
	{
		printf("\n def is present");
	}
	if(lookup(table,"edf"))
	{
		printf("\n edf is present");
	}
	if(lookup(table,"list1"))
	{
		printf("\n list is present");
	}

	return 0;
}
