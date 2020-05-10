
#include "IO_UTILS.H"
#include "os_tree.h"
#include "rb_tree.h"


/***********************************************************************/
/* OS_INORDER_TREE_WALK: print nodes in sorted order of key values.    */
/* time: O(n)                                                          */
/***********************************************************************/
void OS_INORDER_TREE_WALK (os_tree_header* Tree, os_tree_node *node_x, FILE *fp)
{
    unsigned int rank;
    rank = OS_RANK(Tree, node_x);

    rb_tree_node *parallel_rb_node;

    if(node_x != Tree->nil_sentinel)
    {
        parallel_rb_node = node_x->parallel_node;
        OS_INORDER_TREE_WALK(Tree, node_x->left, fp);
        fprintf(fp,"% -9u %- 20.15s %- 32.13s %- 15.9s %-030.9ld %-+30.2f % 4s\n", rank, node_x->first_name, node_x->last_name, node_x->id_number, node_x->customer_num, node_x->balance, (node_x->color == 0) ? "R" : "B");
        OS_INORDER_TREE_WALK(Tree, node_x->right, fp);
    }
}


/***********************************************************************/
/* OS_ITERATIVE_TREE_SEARCH: iterative version of TREE-SERACH: search  */
/* search for a node with key k. time: O(h)                            */
/***********************************************************************/
os_tree_node *OS_ITERATIVE_TREE_SEARCH (os_tree_header* Tree, os_tree_node *node_x, float key_k)
{
    while ((node_x != Tree->nil_sentinel) && node_x->balance != key_k)
    {
        if(key_k < node_x->balance)
            node_x = node_x->left;
        else node_x = node_x->right;
    }
    return node_x;
}


/***********************************************************************/
/* OS_TREE_MINIMUM: return node with minimum key value. time: O(h)     */
/***********************************************************************/
os_tree_node *OS_TREE_MINIMUM (os_tree_header* Tree, os_tree_node *node_x)
{
    if(node_x != Tree->nil_sentinel)
    {
        while(node_x->left != Tree->nil_sentinel)
            node_x = node_x->left;
        return node_x;
    }
    else return node_x;
}


/***********************************************************************/
/* OS_TREE_MAXIMUM: return node with minimum key value. time: O(h)     */
/***********************************************************************/
os_tree_node *OS_TREE_MAXIMUM (os_tree_header* Tree, os_tree_node *node_x)
{
    if(node_x != Tree->nil_sentinel)
    {
        while(node_x->right != Tree->nil_sentinel)
            node_x = node_x->right;
        return node_x;
    }
    else return node_x;
}


/**************************************************************************/
/* OS_TREE_SUCCESSOR: return node with key value which success the key    */
/* value of given node x. time: O(h)                                      */
/**************************************************************************/
os_tree_node *OS_TREE_SUCCESSOR (os_tree_header* Tree, os_tree_node *node_x)
{
    os_tree_node *parent_y;
    if(node_x->right != Tree->nil_sentinel)
        return OS_TREE_MINIMUM(Tree, node_x->right);
    parent_y = node_x->parent;
    while((parent_y != Tree->nil_sentinel) && (node_x == parent_y->right))
    {
        node_x = parent_y;
        parent_y = parent_y->parent;
    }
    return parent_y;
}


/**************************************************************************/
/* OS_TREE_PREDECESSOR: return node with key value which predecessors the */
/* key value of given node x. time: O(h)                                  */
/**************************************************************************/
os_tree_node *OS_TREE_PREDECESSOR (os_tree_header* Tree, os_tree_node *node_x)
{
    os_tree_node *parent_y;
    if(node_x->left != Tree->nil_sentinel)
        return OS_TREE_MAXIMUM(Tree, node_x->left);
    parent_y = node_x->parent;
    while((parent_y != Tree->nil_sentinel) && (node_x == parent_y->left))
    {
        node_x = parent_y;
        parent_y = parent_y->parent;
    }
    return parent_y;
}


/**************************************************************************/
/* print_node_data: prints data of given node for internal use: debug, etc*/
/**************************************************************************/
void print_node_data (os_tree_node *node_x)
{
    fprintf(stdout, "\n\t\tFirst Name: %s, Last name: %s.", node_x->first_name, node_x->last_name);
    fprintf(stdout, "\n\t\tCustomer number: %ld,\b", node_x->customer_num);
    fprintf(stdout, "\n\t\tID number: %s,\b", node_x->id_number);
    fprintf(stderr, "\n\t\tCurrent balance: %.2f\b  $", node_x->balance);
    fprintf(stderr, "\n\t\tColor: %s", (node_x->color == 0) ? "Red\n" : "Black\n");
}


/*********************************************************************************************/
/* print_customer_data: prints data of given input after mapping for internal use: debug, etc*/
/*********************************************************************************************/
void print_customer_data (os_tree_node *x, FILE *fp)
{
    fprintf(fp, "\tName: %s %s, Customer num: %ld, ", x->first_name, x->last_name, x->customer_num);
    fprintf(fp,"ID: %s, Balance: %-.2f ;\n", x->id_number, x->balance);
}


/**************************************************************************/
/* OS_LEFT_ROTATE: change pointers for adjust a different order between   */
/* the node in order to assure it's height is "balanced". time: O(1)      */
/**************************************************************************/
void OS_LEFT_ROTATE (os_tree_header *Tree, os_tree_node *node_x)
{
    os_tree_node *node_y;
    os_tree_node *temp;
    os_tree_node *node_x_left_son;
    os_tree_node *node_x_right_son;

    node_y = node_x->right; /* Set y. */
    node_x->right = node_y->left; /* Turn y's left subtree into x's right subtree. */
    if(node_y->left != Tree->nil_sentinel)
    {
        temp = node_y->left;
        temp->parent = node_x;
    }
    node_y->parent = node_x->parent;
    if(node_x->parent == Tree->nil_sentinel)
        Tree->root = node_y;
    else
    {
        temp = node_x->parent;
        if (node_x == temp->left)
            temp->left = node_y;
        else temp->right = node_y;
    }
    node_y->left = node_x; /* Put x on y's left. */
    node_x->parent = node_y;

    node_x_left_son = node_x->left;
    node_x_right_son = node_x->right;
    node_y->size = node_x->size;
    node_x->size = (node_x_left_son->size) + (node_x_right_son->size) + 1;
}


/**************************************************************************/
/* OS_RIGHT_ROTATE: change pointers for adjust a different order between   */
/* the node in order to assure it's height is "balanced". time: O(1)      */
/**************************************************************************/
void OS_RIGHT_ROTATE (os_tree_header *Tree, os_tree_node *node_y)
{
    os_tree_node *node_x;
    os_tree_node *temp;
    os_tree_node *node_y_left_son;
    os_tree_node *node_y_right_son;

    node_x = node_y->left; /* Set x. */
    node_y->left = node_x->right; /* Turn x's right subtree into y's left subtree. */
    if(node_x->right != Tree->nil_sentinel)
    {
        temp = node_x->right;
        temp->parent = node_y;
    }
    node_x->parent = node_y->parent; /* link y's parent to x. */
    if(node_y->parent == Tree->nil_sentinel)
        Tree->root = node_x;
    else
    {
        temp = node_y->parent;
        if (node_y == temp->right)
            temp->right = node_x;
        else temp->left = node_x;
    }
    node_x->right = node_y; /* Put x on y's right. */
    node_y->parent = node_x;

    node_y_left_son = node_y->left;
    node_y_right_son = node_y->right;
    node_x->size = node_y->size;
    node_y->size = (node_y_left_son->size) + (node_y_right_son->size) + 1;
}


/**************************************************************************/
/* OS_INSERT_FIXUP: maintain red-black tree properties after insertion.   */
/**************************************************************************/
void OS_INSERT_FIXUP (os_tree_header *Tree, os_tree_node *node_z)
{
    os_tree_node *parent_of_z = node_z->parent;
    os_tree_node *grandparent_of_z = parent_of_z->parent;
    os_tree_node *y_uncle_of_z;
    os_tree_node *temp;

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
                    OS_LEFT_ROTATE(Tree, node_z);  /* Case 2 */
                    parent_of_z = node_z->parent; /* Case 2 */
                    grandparent_of_z = parent_of_z->parent; /* Case 2 */
                }

                parent_of_z->color = BLACK;           /* Case 3 */
                grandparent_of_z->color = RED;        /* Case 3 */
                OS_RIGHT_ROTATE(Tree, grandparent_of_z); /* Case 3 */
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
                    OS_RIGHT_ROTATE(Tree, node_z);  /* Case 5 */
                    parent_of_z = node_z->parent;  /* Case 5 */
                    grandparent_of_z = parent_of_z->parent;  /* Case 5 */
                }

                parent_of_z->color = BLACK;           /* Case 6 */
                grandparent_of_z->color = RED;        /* Case 6 */
                OS_LEFT_ROTATE(Tree, grandparent_of_z);  /* Case 6 */
            }
        }
        parent_of_z = node_z->parent;
        grandparent_of_z = parent_of_z->parent;
    }
    temp = Tree->root;
    temp->color = BLACK;
}


/**************************************************************************/
/* OS_INSERT: add a new node to order-statistic tree.                           */
/**************************************************************************/
os_tree_node *OS_INSERT (os_tree_header *Tree, customer* data_record)
{
    os_tree_node *new_node_z = NULL;
    os_tree_node *util_node_x = Tree->root;
    os_tree_node *parent_node_y = Tree->nil_sentinel;
    os_tree_node *left = Tree->nil_sentinel;
    os_tree_node *right = Tree->nil_sentinel;
    rb_tree_node *parallel_rb_node;

    new_node_z = (os_tree_node*)calloc(1, sizeof(os_tree_node)); /* allocate dynamic memory */

    if(new_node_z != NULL)
    {
        /* map data from customer data record to tree node record */
        strncpy(new_node_z->first_name, data_record->first_name, strlen(data_record->first_name)+1);
        strncpy(new_node_z->last_name, data_record->last_name, strlen(data_record->last_name)+1);
        strncpy(new_node_z->id_number, data_record->id_number, strlen(data_record->id_number)+1);
        new_node_z->customer_num = data_record->customer_number;
        new_node_z->balance = data_record->balance;

        while (util_node_x != Tree->nil_sentinel)
        {
            util_node_x->size = util_node_x->size+1;
            parent_node_y = util_node_x;
            if (new_node_z->balance < util_node_x->balance)
                util_node_x = util_node_x->left;
            else  util_node_x = util_node_x->right;
        }
        new_node_z->parent = parent_node_y;
        if (parent_node_y == Tree->nil_sentinel)
            Tree->root = new_node_z; /* Tree was empty */
        else if(new_node_z->balance < parent_node_y->balance)
            parent_node_y->left = new_node_z;
        else parent_node_y->right = new_node_z;

        new_node_z->left = Tree->nil_sentinel;
        new_node_z->right = Tree->nil_sentinel;
        new_node_z->color = RED;
        new_node_z->size = 1;
        OS_INSERT_FIXUP(Tree, new_node_z);

        /* update minimum and maximum nodes if necessary (O(1)) */
        util_node_x = Tree->min;
        if((new_node_z->balance) < (util_node_x->balance))
            Tree->min = new_node_z;
        util_node_x = Tree->max;
        if((new_node_z->balance) > (util_node_x->balance))
            Tree->max = new_node_z;

        /* add a new node to corresponding red-black tree  */
        parallel_rb_node = (rb_tree_node*)calloc(1, sizeof(rb_tree_node)); /* allocate dynamic memory */
        if(parallel_rb_node == NULL) /* failure allocating memory */
            return NULL;
        parallel_rb_node->customer_num = new_node_z->customer_num; /* map satellite data */
        parallel_rb_node->parallel_node = new_node_z;
        new_node_z->parallel_node = parallel_rb_node;
        Tree->rb_root = RB_INSERT(Tree, parallel_rb_node);


        return Tree->root;
    }
    return NULL; /* failure allocating memory */
}


/******************************************************************************************/
/* OS_DELETE_FIXUP: maintain order-statistic tree properties after deletion of black node */
/******************************************************************************************/
void OS_DELETE_FIXUP (os_tree_header *Tree, os_tree_node *node_x)
{
    os_tree_node *parent_of_x = node_x->parent;
    os_tree_node *w_brother_of_x;
    os_tree_node *left_nephew;
    os_tree_node *right_nephew;

    while((node_x != Tree->root) && node_x->color == BLACK)
    {
        if(node_x == parent_of_x->left)
        {
            w_brother_of_x = parent_of_x->right;
            if(w_brother_of_x->color == RED)
            {
                w_brother_of_x->color = BLACK;          /* Case 1 */
                parent_of_x->color = RED;               /* Case 1 */
                OS_LEFT_ROTATE(Tree, parent_of_x);         /* Case 1 */
                parent_of_x = node_x->parent;            /* Case 1 */
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
                    OS_RIGHT_ROTATE(Tree, w_brother_of_x);     /* Case 3 */
                    parent_of_x = node_x->parent;           /* Case 3 */
                    w_brother_of_x = parent_of_x->right;    /* Case 3 */
                }
                parent_of_x = node_x->parent;               /* Case 4 */
                w_brother_of_x->color = parent_of_x->color; /* Case 4 */
                parent_of_x->color = BLACK;                 /* Case 4 */
                right_nephew = w_brother_of_x->right;       /* Case 4 */
                right_nephew->color = BLACK;                /* Case 4 */
                OS_LEFT_ROTATE(Tree, parent_of_x);             /* Case 4 */
                node_x = Tree->root;                        /* Case 4 */
            }
        }
        else
        {
           w_brother_of_x = parent_of_x->left;
           if(w_brother_of_x->color == RED)
            {
                w_brother_of_x->color = BLACK;          /* Case 5 */
                parent_of_x->color = RED;               /* Case 5 */
                OS_RIGHT_ROTATE(Tree, parent_of_x);     /* Case 5 */
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
                    OS_LEFT_ROTATE(Tree, w_brother_of_x);     /* Case 7 */
                    parent_of_x = node_x->parent;          /* Case 7 */
                    w_brother_of_x = parent_of_x->left;    /* Case 7 */
                }
                parent_of_x = node_x->parent;               /* Case 8 */
                w_brother_of_x->color = parent_of_x->color; /* Case 8 */
                parent_of_x->color = BLACK;                 /* Case 8 */
                left_nephew = w_brother_of_x->left;         /* Case 8 */
                left_nephew->color = BLACK;                 /* Case 8 */
                OS_RIGHT_ROTATE(Tree, parent_of_x);            /* Case 8 */
                node_x = Tree->root;                        /* Case 8 */
            }
        }
    }
    node_x->color = BLACK;
}


/************************************************************************************/
/* OS_DELETE: delete a given node z.                                                */
/************************************************************************************/
os_tree_node *OS_DELETE(os_tree_header *Tree, os_tree_node *node_z)
{
    os_tree_node *node_x;
    os_tree_node *node_y;
    os_tree_node *temp;
    rb_tree_node *rb_temp;

    if((node_z->left == Tree->nil_sentinel) || (node_z->right == Tree->nil_sentinel))
        node_y = node_z;
    else node_y = OS_TREE_SUCCESSOR(Tree, node_z);

    if(node_y->left != Tree->nil_sentinel)
        node_x = node_y->left;
    else node_x = node_y->right;

    node_x->parent = node_y->parent;

    if(node_y->parent == Tree->nil_sentinel)
        Tree->root = node_x;
    else
    {
        temp = node_y->parent;
        if(node_y == temp->left)
            temp->left = node_x;
         else temp->right = node_x;
    }

    if(node_y != node_z)
    {/* copy y's satellite data */
        strncpy(node_z->first_name, node_y->first_name, strlen(node_y->first_name+1));
        strncpy(node_z->last_name, node_y->last_name, strlen(node_y->last_name+1));
        strncpy(node_z->id_number, node_y->id_number, strlen(node_y->id_number+1));
        node_z->customer_num = node_y->customer_num;
        node_z->balance = node_y->balance;

        /* update parallel node */
        node_z->parallel_node = node_y->parallel_node;
        rb_temp = node_z->parallel_node;
        rb_temp->parallel_node = node_z;
    }

    /* decrement size of each node all the way up from y to the root */
    temp = node_y;
    while(temp != Tree->root && temp != Tree->nil_sentinel)
    {
        temp = temp->parent;
        temp->size--;
    }

    if(node_y->color == BLACK)
        OS_DELETE_FIXUP(Tree, node_x);

    /* update minimum if necessary O(lgn) */
    if(node_y == Tree->min)
    {
        Tree->min = OS_TREE_MINIMUM(Tree, Tree->root);
        if ((Tree->min) == (Tree->nil_sentinel))
            Tree->min = Tree->min_sentinel;
    }

    /* update maximum if necessary O(lgn) */
    if(node_y == Tree->max)
    {
        Tree->max = OS_TREE_MAXIMUM(Tree, Tree->root);
        if ((Tree->max) == (Tree->nil_sentinel))
            Tree->max = Tree->max_sentinel;
    }

    return node_y;
}


/************************************************************************************/
/* OS_SELECT: given a order statistic i, this function returns a node which its key */
/* value is the positioned in the i'th rank, considering sored order.               */
/************************************************************************************/
os_tree_node *OS_SELECT(os_tree_node *node_x, unsigned int i)
{
    os_tree_node *util_temp_node = node_x->left;
    unsigned int r = util_temp_node->size + 1;
    if(i == r)
        return node_x;
    else if(i < r)
        return OS_SELECT(node_x->left, i);
    else return OS_SELECT(node_x->right, i-r);
}


/*************************************************************************************/
/* OS_RANK: given a node x, this function returns the rank of the key of x among the */
/* the set of record in the tree, considering sorted order.                          */
/*************************************************************************************/
unsigned int OS_RANK(os_tree_header *Tree, os_tree_node *node_x)
{
    os_tree_node *util_node_y;
    os_tree_node *sentinel_node = Tree->nil_sentinel;
    os_tree_node *util_temp_node = node_x->left;
    unsigned int r = util_temp_node->size + 1;
    util_node_y = node_x;

    while ((util_node_y != Tree->root) && (util_node_y != sentinel_node))
    {
        util_temp_node = util_node_y->parent;
        if(util_node_y == util_temp_node->right)
        {
            util_temp_node = util_temp_node->left;
            r += (util_temp_node->size + 1);
        }
        util_node_y = util_node_y->parent;
    }
    return r;
}


/*******************************************************************************************/
/* OS_ENUMERATE: given a node x, and low and high limits of range, this function prints    */
/* all of the nodes rooted in x, which their key value is in the given range of low & high */
/* it is meant for use in this program to print all nodes with a negative balance.         */
/*******************************************************************************************/
void OS_ENUMERATE(os_tree_header *Tree, os_tree_node *x, float low, float high, FILE *fp)
{
    if((x->balance >= low) && (x != Tree->nil_sentinel))
        OS_ENUMERATE(Tree, x->left, low, high, fp);
    if((x->balance >= low) && (x->balance < high) && (x != Tree->nil_sentinel))
        print_customer_data(x, fp);
    if((x->balance < high) && (x != Tree->nil_sentinel))
        OS_ENUMERATE(Tree, x->right, low, high, fp);
}



