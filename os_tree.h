#ifndef OS_TREE_H_INCLUDED
#define OS_TREE_H_INCLUDED

/*********************************/
/* HEADER FILE FOR os_tree.c     */
/*********************************/

#include "io_utils.h"

/* function declarations */
void OS_INORDER_TREE_WALK (os_tree_header* Tree, os_tree_node *node_x, FILE *fp);
os_tree_node *OS_ITERATIVE_TREE_SEARCH (os_tree_header *Tree, os_tree_node *node_x, float key_k);
os_tree_node *OS_TREE_MINIMUM (os_tree_header* Tree, os_tree_node *node_x);
os_tree_node *OS_TREE_MAXIMUM (os_tree_header* Tree, os_tree_node *node_x);
os_tree_node *OS_TREE_SUCCESSOR(os_tree_header* Tree, os_tree_node *node_x);
os_tree_node *OS_TREE_PREDECESSOR (os_tree_header* Tree, os_tree_node *node_x);
void OS_LEFT_ROTATE (os_tree_header *Tree, os_tree_node *node_x);
void OS_RIGHT_ROTATE (os_tree_header *Tree, os_tree_node *node_x);
void OS_INSERT_FIXUP (os_tree_header *Tree, os_tree_node *node_z);
os_tree_node *OS_INSERT (os_tree_header *Tree, customer* data_record);
void OS_DELETE_FIXUP (os_tree_header *Tree, os_tree_node *node_x);
os_tree_node *OS_DELETE(os_tree_header *Tree, os_tree_node *node_z);
os_tree_node *OS_SELECT(os_tree_node *node_x, unsigned int i);
unsigned int OS_RANK(os_tree_header *Tree, os_tree_node *node_x);
void print_node_data (os_tree_node *node_x);
void print_customer_data(os_tree_node *node_x, FILE *fp);
void OS_ENUMERATE(os_tree_header *Tree, os_tree_node *x, float low, float high, FILE *fp);

#endif // OS_TREE_H_INCLUDED
