
#include "rb_tree.h"
#include "os_tree.h"

/***********************************************************************/
/* RB_INORDER_TREE_WALK: print nodes in sorted order of key values.    */
/* time: O(n)                                                          */
/***********************************************************************/
void RB_INORDER_TREE_WALK (os_tree_header* Tree, rb_tree_node *node_z, FILE *fp)
{
    os_tree_node *x;
    if(node_z != Tree->rb_nil_sentinel)
    {
        RB_INORDER_TREE_WALK(Tree, node_z->left, fp);
        //print_node_data(node_x->parallel_node);
        x = node_z->parallel_node;
        fprintf(fp, "\n%-12s %-12s %-12s %-010ld %+-015.2f %-5s\n", x->first_name, x->last_name, x->id_number, x->customer_num, x->balance, (x->color == 0) ? "R" : "B");
        RB_INORDER_TREE_WALK(Tree, node_z->right, fp);
    }
}/***********************************************************************/


/***********************************************************************/
/* RB_ITERATIVE_TREE_SEARCH: iterative version of TREE-SERACH: search  */
/* search for a node with key k. time: O(h)                            */
/***********************************************************************/
rb_tree_node *RB_ITERATIVE_TREE_SEARCH (os_tree_header* Tree, rb_tree_node *node_x, long key_k)
{
    while ((node_x != Tree->rb_nil_sentinel) && (node_x->customer_num != key_k))
    {
        if(key_k < node_x->customer_num)
            node_x = node_x->left;
        else node_x = node_x->right;
    }
    return node_x;
}/***********************************************************************/


/***********************************************************************/
/* RB_TREE_MINIMUM: return node with minimum key value. time: O(h)     */
/***********************************************************************/
rb_tree_node *RB_TREE_MINIMUM (os_tree_header* Tree, rb_tree_node *node_x)
{
    if(node_x != Tree->rb_nil_sentinel)
    {
        while(node_x->left != Tree->rb_nil_sentinel)
            node_x = node_x->left;
        return node_x;
    }
    else return node_x;
}/***********************************************************************/



/***********************************************************************/
/* RB_TREE_MAXIMUM: return node with maximum key value. time: O(h)     */
/***********************************************************************/
rb_tree_node *RB_TREE_MAXIMUM (os_tree_header* Tree, rb_tree_node *node_x)
{
    if(node_x != Tree->rb_nil_sentinel)
    {
        while(node_x->right != Tree->rb_nil_sentinel)
            node_x = node_x->right;
        return node_x;
    }
    else return node_x;
}/***********************************************************************/


/**************************************************************************/
/* RB_TREE_SUCCESSOR: return node with key value which success the key    */
/* value of given node x. time: O(h)                                      */
/**************************************************************************/
rb_tree_node *RB_TREE_SUCCESSOR (os_tree_header* Tree, rb_tree_node *node_x)
{
    rb_tree_node *parent_y;
    if(node_x->right != Tree->rb_nil_sentinel)
        return RB_TREE_MINIMUM(Tree, node_x->right);
    parent_y = node_x->parent;
    while((parent_y != Tree->rb_nil_sentinel) && (node_x == parent_y->right))
    {
        node_x = parent_y;
        parent_y = parent_y->parent;
    }
    return parent_y;
}/***********************************************************************/

/**************************************************************************/
/* RB_TREE_PREDECESSOR: return node with key value which predecessors the */
/* key value of given node x. time: O(h)                                  */
/**************************************************************************/
rb_tree_node *RB_TREE_PREDECESSOR (os_tree_header* Tree, rb_tree_node *node_x)
{
    rb_tree_node *parent_y;
    if(node_x->left != Tree->rb_nil_sentinel)
        return RB_TREE_MAXIMUM(Tree, node_x->left);
    parent_y = node_x->parent;
    while((parent_y != Tree->rb_nil_sentinel) && (node_x == parent_y->left))
    {
        node_x = parent_y;
        parent_y = parent_y->parent;
    }
    return parent_y;
}/***********************************************************************/


/**************************************************************************/
/* RB_LEFT_ROTATE: change pointers for adjust a different order between   */
/* the node in order to assure it's height is "balanced". time: O(1)      */
/**************************************************************************/
void RB_LEFT_ROTATE (os_tree_header *Tree, rb_tree_node *node_x)
{
    rb_tree_node *node_y;
    rb_tree_node *temp;

    node_y = node_x->right; /* Set y. */
    node_x->right = node_y->left; /* Turn y's left subtree into x's right subtree. */
    if(node_y->left != Tree->rb_nil_sentinel)
    {
        temp = node_y->left;
        temp->parent = node_x;
    }
    node_y->parent = node_x->parent;
    if(node_x->parent == Tree->rb_nil_sentinel)
        Tree->rb_root = node_y;
    else
    {
        temp = node_x->parent;
        if (node_x == temp->left)
            temp->left = node_y;
        else temp->right = node_y;
    }
    node_y->left = node_x; /* Put x on y's left. */
    node_x->parent = node_y;

}/***********************************************************************/

/**************************************************************************/
/* RB_RIGHT_ROTATE: change pointers for adjust a different order between  */
/* the node in order to assure it's height is "balanced". time: O(1)      */
/**************************************************************************/
void RB_RIGHT_ROTATE (os_tree_header *Tree, rb_tree_node *node_y)
{
    rb_tree_node *node_x;
    rb_tree_node *temp;

    node_x = node_y->left; /* Set x. */
    node_y->left = node_x->right; /* Turn x's right subtree into y's left subtree. */
    if(node_x->right != Tree->rb_nil_sentinel)
    {
        temp = node_x->right;
        temp->parent = node_y;
    }
    node_x->parent = node_y->parent; /* link y's parent to x. */
    if(node_y->parent == Tree->rb_nil_sentinel)
        Tree->rb_root = node_x;
    else
    {
        temp = node_y->parent;
        if (node_y == temp->right)
            temp->right = node_x;
        else temp->left = node_x;
    }
    node_x->right = node_y; /* Put x on y's right. */
    node_y->parent = node_x;

}/***********************************************************************/

/**************************************************************************/
/* RB_INSERT_FIXUP: maintain red-black tree properties after insertion.   */
/**************************************************************************/
void RB_INSERT_FIXUP (os_tree_header *Tree, rb_tree_node *node_z)
{
    rb_tree_node *parent_of_z = node_z->parent;
    rb_tree_node *grandparent_of_z = parent_of_z->parent;
    rb_tree_node *y_uncle_of_z;
    rb_tree_node *temp;

    while(parent_of_z->color == RED)
    {
        if((parent_of_z) == (grandparent_of_z->left)) /* 3 cases */
        {
            y_uncle_of_z = grandparent_of_z->right;
            if(y_uncle_of_z->color == RED)
            {
                parent_of_z->color = BLACK;     /* Case 1 */
                y_uncle_of_z->color = BLACK;    /* Case 1 */
                grandparent_of_z->color = RED;  /* Case 1 */
                node_z = grandparent_of_z;      /* Case 1 */
            }
            else
            {
                if(node_z == parent_of_z->right)
                {
                    node_z = parent_of_z;       /* Case 2 */
                    RB_LEFT_ROTATE(Tree, node_z);  /* Case 2 */
                    parent_of_z = node_z->parent; /* Case 2 */
                    grandparent_of_z = parent_of_z->parent; /* Case 2 */
                }

                parent_of_z->color = BLACK;           /* Case 3 */
                grandparent_of_z->color = RED;        /* Case 3 */
                RB_RIGHT_ROTATE(Tree, grandparent_of_z); /* Case 3 */
            }
        }
        else
        {
            y_uncle_of_z = grandparent_of_z->left;
            if(y_uncle_of_z->color == RED)
            {
                parent_of_z->color = BLACK;     /* Case 4 */
                y_uncle_of_z->color = BLACK;    /* Case 4 */
                grandparent_of_z->color = RED;  /* Case 4 */
                node_z = grandparent_of_z;      /* Case 4 */
            }
            else
            {
                if(node_z == parent_of_z->left)
                {
                    node_z = parent_of_z;        /* Case 5 */
                    RB_RIGHT_ROTATE(Tree, node_z);  /* Case 5 */
                    parent_of_z = node_z->parent;   /* Case 5 */
                    grandparent_of_z = parent_of_z->parent; /* Case 5 */
                }
                parent_of_z->color = BLACK;           /* Case 6 */
                grandparent_of_z->color = RED;        /* Case 6 */
                RB_LEFT_ROTATE(Tree, grandparent_of_z);  /* Case 6 */
            }
        }
        parent_of_z = node_z->parent;
        grandparent_of_z = parent_of_z->parent;
    }
    temp = Tree->rb_root;
    temp->color = BLACK;
}/***********************************************************************/

/**************************************************************************/
/* RB_INSERT: add a new node to red-black tree.                            */
/**************************************************************************/
rb_tree_node *RB_INSERT (os_tree_header *Tree, rb_tree_node* new_node_z)
{
    rb_tree_node *util_node_x = Tree->rb_root;
    rb_tree_node *parent_node_y = Tree->rb_nil_sentinel;
    rb_tree_node *left = Tree->rb_nil_sentinel;
    rb_tree_node *right = Tree->rb_nil_sentinel;

    while (util_node_x != Tree->rb_nil_sentinel)
    {
        parent_node_y = util_node_x;
        if (new_node_z->customer_num < util_node_x->customer_num)
            util_node_x = util_node_x->left;
        else  util_node_x = util_node_x->right;
    }
    new_node_z->parent = parent_node_y;
    if (parent_node_y == Tree->rb_nil_sentinel)
        Tree->rb_root = new_node_z; /* Tree was empty */
    else if(new_node_z->customer_num < parent_node_y->customer_num)
        parent_node_y->left = new_node_z;
    else parent_node_y->right = new_node_z;

    new_node_z->left = Tree->rb_nil_sentinel;
    new_node_z->right = Tree->rb_nil_sentinel;
    new_node_z->color = RED;
    RB_INSERT_FIXUP(Tree, new_node_z);

    return Tree->rb_root;
}
/***********************************************************************/


/************************************************************************************/
/* RB_DELETE_FIXUP: maintain red-black tree properties after deletion of black node */
/************************************************************************************/
void RB_DELETE_FIXUP (os_tree_header *Tree, rb_tree_node *node_x)
{
    rb_tree_node *parent_of_x = node_x->parent;
    rb_tree_node *w_brother_of_x;
    rb_tree_node *left_nephew;
    rb_tree_node *right_nephew;

    while((node_x != Tree->rb_root) && node_x->color == BLACK)
    {
        if(node_x == parent_of_x->left)
        {
            w_brother_of_x = parent_of_x->right;
            if(w_brother_of_x->color == RED)
            {
                w_brother_of_x->color = BLACK;          /* Case 1 */
                parent_of_x->color = RED;               /* Case 1 */
                RB_LEFT_ROTATE(Tree, parent_of_x);         /* Case 1 */
                parent_of_x = node_x->parent;              /* Case 1 */
                w_brother_of_x = parent_of_x->right;    /* Case 1 */
            }
            left_nephew = w_brother_of_x->left;
            right_nephew = w_brother_of_x->right;
            if((left_nephew->color == BLACK) && (right_nephew->color == BLACK))
            {
                w_brother_of_x->color = RED;            /* Case 2 */
                node_x = node_x->parent;                /* Case 2 */
            }
            else
            {
                if(right_nephew->color == BLACK)
                {
                    left_nephew->color = BLACK;             /* Case 3 */
                    w_brother_of_x->color = RED;            /* Case 3 */
                    RB_RIGHT_ROTATE(Tree, w_brother_of_x);     /* Case 3 */
                    parent_of_x = node_x->parent;           /* Case 3 */
                    w_brother_of_x = parent_of_x->right;    /* Case 3 */
                }
                parent_of_x = node_x->parent;               /* Case 4 */
                w_brother_of_x->color = parent_of_x->color; /* Case 4 */
                parent_of_x->color = BLACK;                 /* Case 4 */
                right_nephew = w_brother_of_x->right;       /* Case 4 */
                right_nephew->color = BLACK;                /* Case 4 */
                RB_LEFT_ROTATE(Tree, parent_of_x);             /* Case 4 */
                node_x = Tree->rb_root;                        /* Case 4 */
            }
        }
        else
        {
           w_brother_of_x = parent_of_x->left;
           if(w_brother_of_x->color == RED)
            {
                w_brother_of_x->color = BLACK;          /* Case 5 */
                parent_of_x->color = RED;               /* Case 5 */
                RB_RIGHT_ROTATE(Tree, parent_of_x);        /* Case 5 */
                parent_of_x = node_x->parent;           /* Case 5 */
                w_brother_of_x = parent_of_x->left;     /* Case 5 */
            }
            left_nephew = w_brother_of_x->left;
            right_nephew = w_brother_of_x->right;
            if((left_nephew->color == BLACK) && (right_nephew->color == BLACK))
            {
                w_brother_of_x->color = RED;            /* Case 6 */
                node_x = node_x->parent;                /* Case 6 */
            }
            else
            {
                if(left_nephew->color == BLACK)
                {
                    right_nephew->color = BLACK;           /* Case 7 */
                    w_brother_of_x->color = RED;           /* Case 7 */
                    RB_LEFT_ROTATE(Tree, w_brother_of_x);     /* Case 7 */
                    parent_of_x = node_x->parent;          /* Case 7 */
                    w_brother_of_x = parent_of_x->left;    /* Case 7 */
                }
                parent_of_x = node_x->parent;               /* Case 8 */
                w_brother_of_x->color = parent_of_x->color; /* Case 8 */
                parent_of_x->color = BLACK;                 /* Case 8 */
                left_nephew = w_brother_of_x->left;         /* Case 8 */
                left_nephew->color = BLACK;                 /* Case 8 */
                RB_RIGHT_ROTATE(Tree, parent_of_x);            /* Case 8 */
                node_x = Tree->rb_root;                        /* Case 8 */
            }
        }
    }
    node_x->color = BLACK;
}/***********************************************************************/



/************************************************************************************/
/* RB_DELETE: delete a given node z.                                                */
/************************************************************************************/
rb_tree_node *RB_DELETE(os_tree_header *Tree, rb_tree_node *node_z)
{
    rb_tree_node *node_x;
    rb_tree_node *node_y;
    rb_tree_node *temp;
    os_tree_node *os_temp;
    if((node_z->left == Tree->rb_nil_sentinel) || (node_z->right == Tree->rb_nil_sentinel))
        node_y = node_z;
    else node_y = RB_TREE_SUCCESSOR(Tree, node_z);

    if(node_y->left != Tree->rb_nil_sentinel)
        node_x = node_y->left;
    else node_x = node_y->right;

    node_x->parent = node_y->parent;

    if(node_y->parent == Tree->rb_nil_sentinel)
        Tree->rb_root = node_x;
    else
    {
        temp = node_y->parent;
        if(node_y == temp->left)
            temp->left = node_x;
         else temp->right = node_x;
    }

    if(node_y != node_z)
    {/* copy y's satellite data */
        node_z->customer_num = node_y->customer_num;

        /* update parallel node pointers */
        node_z->parallel_node = node_y->parallel_node;
        os_temp = node_z->parallel_node;
        os_temp->parallel_node = node_z;
    }

    if(node_y->color == BLACK)
        RB_DELETE_FIXUP(Tree, node_x);

    return node_y;
}/***********************************************************************/


