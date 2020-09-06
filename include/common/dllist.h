#ifndef INCLUDE_COMMON_DLLIST_H_
#define INCLUDE_COMMON_DLLIST_H_


struct Node  {
	void* data;
	struct Node* next;
	struct Node* prev;
};
typedef struct Node List;

List* new_dllist(void* data);
struct Node* dllist_new_node(void* data);
void dllist_insert_at_head(List** head,void* data) ;
void dllist_insert_at_tail(List** head,void* data);
void dllist_insert_at_index(List **head, void *data, int pos);
void dllist_delete_at_head(List **head);
void dllist_delete_at_tail(List **head);
void dllist_delete_at_index(List **head, int pos);
void dllist_free(List **list);
struct Node* dllist_find(List *list, void *data);
int dllist_index_of(List *list, void *data);
void dllist_for_each(struct Node *node, void (*fptr)(void *));


#endif
