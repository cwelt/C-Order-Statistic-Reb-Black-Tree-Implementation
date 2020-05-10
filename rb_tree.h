#ifndef RB_TREE_H_INCLUDED
#define RB_TREE_H_INCLUDED

#include "io_utils.h"

/*********************************/
/* HEADER FILE FOR rb_tree.c     */
/*********************************/

/* function declarations */
void RB_INORDER_TREE_WALK (os_tree_header* Tree, rb_tree_node *node_x, FILE *fp); /*delete after */
rb_tree_node *RB_ITERATIVE_TREE_SEARCH (os_tree_header *Tree, rb_tree_node *node_x, long key_k);
rb_tree_node *RB_TREE_SUCCESSOR(os_tree_header *Tree, rb_tree_node *node_x);
rb_tree_node *RB_TREE_PREDECESSOR (os_tree_header *Tree, rb_tree_node *node_x);
void RB_LEFT_ROTATE (os_tree_header *Tree, rb_tree_node *node_x);
void RB_RIGHT_ROTATE (os_tree_header *Tree, rb_tree_node *node_x);
void RB_INSERT_FIXUP (os_tree_header *Tree, rb_tree_node *node_z);
rb_tree_node *RB_INSERT (os_tree_header *Tree, rb_tree_node* new_node_z);
void RB_DELETE_FIXUP (os_tree_header *Tree, rb_tree_node *node_x);
rb_tree_node *RB_DELETE(os_tree_header *Tree, rb_tree_node *node_z);

#endif // RB_TREE_H_INCLUDED
