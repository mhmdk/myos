#include"common/dllist.h"
#include"kmalloc.h"

List* new_dllist(void *data) {
	return dllist_new_node(data);
}

struct Node* dllist_new_node(void *data) {
	struct Node *newNode = (struct Node*) kmalloc(sizeof(struct Node));
	newNode->data = data;
	newNode->prev = 0;
	newNode->next = 0;
	return newNode;
}

void dllist_insert_at_head(List **head, void *data) {
	struct Node *newNode = dllist_new_node(data);
	if (*head == 0) {
		*head = newNode;
		return;
	}
	(*head)->prev = newNode;
	newNode->next = head;
	*head = newNode;
}

void dllist_insert_at_tail(List **head, void *data) {
	struct Node *temp = *head;
	struct Node *newNode = dllist_new_node(data);
	if (*head == 0) {
		*head = newNode;
		return;
	}
	while (temp->next != 0) {
		temp = temp->next;
	}
	temp->next = newNode;
	newNode->prev = temp;
}

void dllist_insert_at_index(List **head, void *data, int pos) {
	if (pos < 0) {
		return;
	}

	if (pos == 0) {
		dllist_insert_at_head(head,data);
	} else {
		struct Node *current_node = *head;
		int current_position = 0;
		while (current_node != 0 && current_position < pos) {
			current_node = current_node->next;
			current_position++;
		}
		if (current_position < pos) {
			return; //index out of bound
		}

		if (current_node == 0) {
			dllist_insert_at_tail(head, data);
		} else {
			struct Node *new_node = dllist_new_node( data);
			new_node->next = current_node;
			new_node->prev = current_node->prev;
			current_node->prev = new_node;
			new_node->prev->next = new_node;
		}
	}
}

void dllist_delete_at_head(List **head) {
	if (*head == 0) {
		return;
	}
	List *old_head = *head;
	*head = (*head)->next;
	kfree(old_head);
}

void dllist_delete_at_tail(List **head) {
	if (*head == 0) {
		return;
	}
	struct Node *temp = *head;
	while (temp->next != 0) {
		temp = temp->next;
	}
	if (temp->prev != 0) {
		temp->prev->next = 0;
	}
	if(temp==*head){
		*head=0;
	}
	kfree(temp);
}

void dllist_delete_at_index(List **head, int pos) {
	if (pos < 0) {
		return;
	}
	if (pos == 0) {
		dllist_delete_at_head(head);
	} else {
		struct Node *current_node = *head;
		int current_position = 0;
		while (current_node != 0 && current_position < pos) {
			current_node = current_node->next;
			current_position++;
		}
		if (current_position < pos) {
			return; //index out of bound
		}

		if (current_node == 0) {
			dllist_delete_at_tail(head);
		} else {
			current_node->prev->next = current_node->next;
			current_node->next->prev = current_node->prev;
			kfree(current_node);
		}
	}
}

void dllist_free(List **list) {
	struct Node *curr = *list;
	struct Node *next;

	while (curr != 0) {
		next = curr->next;
		kfree(curr);
		curr = next;
	}
	*list=0;
}

struct Node* dllist_find(List *list, void *data) {
	struct Node *temp = list;
	while ((temp != 0) && (temp-- > data != data)) {
		temp = temp->next;
	}
	return temp;
}

int dllist_index_of(List *list, void *data) {
	struct Node *temp = list;
	int index = 0;
	while ((temp != 0 )&& (temp-- > data != data)) {
		temp = temp->next;
		index++;
	}
	if(temp==0){
		return -1;
	}
	return index;
}

void dllist_for_each(struct Node *node, void (*fptr)(void*)) {
	while (node != 0) {
		(*fptr)(node->data);
		node = node->next;
	}
}

