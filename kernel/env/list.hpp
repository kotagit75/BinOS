#pragma once

typedef struct Node{
  void *data;
  struct Node *next;
}node;
typedef struct _list{
    node *head;
}list;
void list_init(list *list);
void list_insertion(list *list,void *num);
void list_deletion(list *list,void *num);