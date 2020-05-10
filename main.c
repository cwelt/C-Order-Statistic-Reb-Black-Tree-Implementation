
/**********************************************************************************************************/
/* 2016A - 20407 : Final Project    ***********************************************************************/
/* @author:   Chanan Welt   *******************************************************************************/
/* @date 20/02/2016     ***********************************************************************************/
/**********************************************************************************************************/
/* This project implements the most efficient data structure for managing a dynamic set of bank customers.*/
/* The managing include supporting all dictionary basic queries - insert, delete, search, and some more   */
/* complex ones: all clients with a negative balance, updating balance (increase/decrease), max and more. */
/* The efficiency of the various variations is valuated as a function of N - the number of accounts in the*/
/* bank, or in other words - the number of records in the dynamic set.                                    */
/* The most efficient data structure known to me by this time for implementing such a task is a balanced  */
/* binary-search-tree. In this project the balanced type of tree which is used is a basic red-black tree  */
/* and an augmented red-black tree, which is a order-statistic tree. basic operations on this data struct'*/
/* operate in time of O(lgn) on the worst case scenario ***************************************************/
/**********************************************************************************************************/

#include "io_utils.h"
#include "rb_tree.h"
#include "os_tree.h"

int main (int argc, char *argv[])
{
    FILE *input_fp; /* input file pointer */
    FILE *output_fp; /* output file pointer */
    time_t current_time;
    char *timestamp;
    unsigned int file_counter = 0;
    char *argument_name; /* name of current command line argument */
    char file_name[FILE_MAX_NAME];
    char output_file_name[FILE_OUT_MAX_NAME];
    unsigned int arg_name_length;
    unsigned int line_counter = 0;
    char current_input_line[MAX_LINE_LENGTH];
    char current_token[MAX_LINE_LENGTH];
    char *line_parser;
    int operation;
    customer customer_record;
    customer *customer_ptr = &customer_record;
    int success_indicator;
    os_tree_header *T_data_structure_ptr;
    os_tree_node *sentinel;
    os_tree_node *min_sentinel;
    os_tree_node *max_sentinel;
    os_tree_node *temp_util_node;
    rb_tree_node *rb_sentinel;
    rb_tree_node *rb_temp_util_node;
    float temp_sum = 0;
    int i, j, k = 0;

    /* get current time stamp for log documentation and output file name */
    current_time = time(NULL);
    timestamp = ctime(&current_time);

    /* create data structure: header data and sentinels  */
    T_data_structure_ptr = (os_tree_header*)calloc(1, sizeof(os_tree_header));
    sentinel = (os_tree_node*)calloc(1, sizeof(os_tree_node));
    min_sentinel = (os_tree_node*)calloc(1, sizeof(os_tree_node));
    max_sentinel = (os_tree_node*)calloc(1, sizeof(os_tree_node));
    rb_sentinel = (rb_tree_node*)calloc(1, sizeof(rb_tree_node));

    if((T_data_structure_ptr == NULL) || (sentinel == NULL) || (min_sentinel == NULL) || (max_sentinel == NULL) || (rb_sentinel == NULL))
    {
        fprintf(output_fp, "%s %s", argv[0], msg_main_memory);
        return EXIT_FAILURE;
    }

    /* first initialization of data structure */
    T_data_structure_ptr->nil_sentinel = sentinel;
    T_data_structure_ptr->rb_nil_sentinel = rb_sentinel;
    T_data_structure_ptr->root = sentinel;
    T_data_structure_ptr->rb_root = rb_sentinel;
    T_data_structure_ptr->max_sentinel = min_sentinel;
    T_data_structure_ptr->max_sentinel = max_sentinel;
    T_data_structure_ptr->min = min_sentinel;
    T_data_structure_ptr->max = max_sentinel;
    min_sentinel->balance = LONG_MAX;
    max_sentinel->balance = LONG_MIN;
    temp_util_node = T_data_structure_ptr->root; /* set temp to point on os-tree root */
    temp_util_node->parent = sentinel;  /* set os-tree root's parent to nil sentinel */
    sentinel->customer_num = NIL_SENTINEL;
    sentinel->parent = sentinel;
    sentinel->left = sentinel;
    sentinel->right = sentinel;
    sentinel->color = BLACK;
    sentinel->size = 0;
    sentinel->parallel_node = rb_sentinel;
    rb_temp_util_node = T_data_structure_ptr->rb_root; /* set temp to point on os-tree root */
    rb_temp_util_node->parent = rb_sentinel;  /* set os-tree root's parent to nil sentinel */
    rb_sentinel->customer_num = NIL_SENTINEL;
    rb_sentinel->parent = sentinel;
    rb_sentinel->left = sentinel;
    rb_sentinel->right = sentinel;
    rb_sentinel->color = BLACK;
    rb_sentinel->parallel_node = sentinel;

    fprintf(stderr, "\n%s\n", timestamp);

    /* check source of input: file or manually  */
    if (argc > 1) /* at least one file */
    {/* validate there are not too many files (security & memory concerns) */
        if (argc > (MAX_FILES+1))
            fprintf(stderr, msg_max_files, argv[0], MAX_FILES);

        while (--argc > 0 && file_counter < MAX_FILES)
        {
            file_counter++;
            argument_name = *++argv;
            arg_name_length = strlen(argument_name);

            if (arg_name_length > FILE_MAX_NAME)
            {
              fprintf(stderr, msg_file_name_length, argv[0], argument_name, FILE_MAX_NAME);
              continue;
            }

            /* try to open input file for reading */
            strcpy(file_name, argument_name);
            if ((input_fp = fopen(file_name, "r")) == NULL)
            { /* if error */
              fprintf(stderr, "File %d: %s \'%s\'.\n", file_counter, msg_file_open_failure, file_name);
              continue;
            }

            /* set output file name */
            strcpy(output_file_name, file_name);
            strcat(output_file_name, "_");
            strncat(output_file_name, timestamp, TIMESTAMP_LENGTH);
            strcat(output_file_name, output_file_name_suffix);

            /* try to open output file for writing */
            if ((output_fp = fopen(output_file_name, "w")) == NULL)
            { /* if error */
              fprintf(stderr, msg_file_creation_err, output_file_name);
              continue;
            }

            fprintf(output_fp, header_line);
            fprintf(output_fp, report_title, file_name);
            fprintf(output_fp, header_line);
            line_counter = 0;

            /* loop at all input lines, one at a time */
            while(fgets(current_input_line, (MAX_LINE_LENGTH+1), input_fp))
              {
                if(file_counter > 0)
                    fprintf(output_fp, "\n\nline %-2d ", ++line_counter); /* update line counter */
                if (strlen(current_input_line) == MAX_LINE_LENGTH)
                {
                    fprintf(output_fp, msg_line_length, MAX_LINE_LENGTH);
                    continue;
                }
                READ_INPUT: /*set label for enabling manual input as well */
                fprintf(output_fp, "input:  %s\toutput:\t", current_input_line);

                /* in case the input is manually via command line interpreter, check if the user wants to quit */
                if(*current_input_line == 'q')
                {
                  fprintf(output_fp, "\n%s", long_header_line);
                  fprintf(output_fp, "Rank|| First name || Last name\t      || Id num         || Customer\t      || Balance          \t|| Color|\n");
                  fprintf(output_fp, "%s", long_header_line);
                  OS_INORDER_TREE_WALK(T_data_structure_ptr, T_data_structure_ptr->root, output_fp); /* if so, print result */
                  return  EXIT_SUCCESS;
                }

                line_parser = strcpy(current_token, current_input_line);
                operation = determine_operation(line_parser); /* check what is the wished operation */
                if((operation == EMPTY_LINE) || (operation == NOT_FOUND))
                {
                    if(operation == NOT_FOUND)
                        fprintf(output_fp,  msg_illegal_op, line_parser);
                    else fprintf(output_fp,  "\n");
                    continue;
                }

                /* map user's input to programs params for integrating between the different modules */
                success_indicator = map_data(operation, line_parser, &customer_record);
                if (success_indicator == EXIT_SUCCESS)
                {
                    switch (operation) /* proceed according to the matching operation requested */
                    {
                        case DEPOSIT_WITHDRAW:
                            {
                                /* search for the customer node in rb-tree */
                                rb_temp_util_node = RB_ITERATIVE_TREE_SEARCH(T_data_structure_ptr, T_data_structure_ptr->rb_root, customer_ptr->customer_number);
                                if(rb_temp_util_node == T_data_structure_ptr->rb_nil_sentinel) /* error if not found */
                                {
                                    fprintf(output_fp, msg_customer_not_found, customer_ptr->customer_number);
                                    continue;
                                }
                                temp_util_node = rb_temp_util_node->parallel_node; /* get parallel node in os-tree */
                                temp_sum = customer_ptr->balance; /* save requested amount for deposit\ withdraw */

                                /* map satellite data from current node to structure for a node with new balance */
                                strcpy(customer_ptr->first_name, temp_util_node->first_name);
                                strcpy(customer_ptr->last_name, temp_util_node->last_name);
                                strcpy(customer_ptr->id_number, temp_util_node->id_number);
                                customer_ptr->balance = (customer_ptr->balance) + (temp_util_node->balance);

                                temp_util_node = OS_INSERT(T_data_structure_ptr, customer_ptr);
                                if(temp_util_node == NULL)
                                {
                                    fprintf(output_fp, "%s", msg_node_memory);
                                    continue;
                                }

                                T_data_structure_ptr->root = temp_util_node; /*update root after insertion of new node*/

                                temp_util_node = rb_temp_util_node->parallel_node;
                                temp_util_node = OS_DELETE(T_data_structure_ptr, temp_util_node);
                                free(temp_util_node);

                                rb_temp_util_node = RB_DELETE(T_data_structure_ptr, rb_temp_util_node);
                                free(rb_temp_util_node);

                                fprintf(output_fp, feedback_balance_upd, (customer_ptr->balance - temp_sum), customer_ptr->balance);
                                continue;
                            }
                        case NEW_CUSTOMER:
                            {
                                rb_temp_util_node = RB_ITERATIVE_TREE_SEARCH(T_data_structure_ptr, T_data_structure_ptr->rb_root, customer_ptr->customer_number);
                                if(rb_temp_util_node != T_data_structure_ptr->rb_nil_sentinel)
                                {
                                    fprintf(output_fp, "%s", msg_customer_already_exsits);
                                    continue;
                                }

                                temp_util_node = OS_INSERT(T_data_structure_ptr, customer_ptr);
                                if(temp_util_node != NULL)
                                {
                                    T_data_structure_ptr->root = temp_util_node; /* return updated root after insertion */
                                    fprintf(output_fp, feedback_new_cust, customer_ptr->first_name, customer_ptr->last_name, customer_ptr->customer_number, customer_ptr->id_number, customer_ptr->balance);
                                    continue;
                                }
                                else
                                {
                                    fprintf(output_fp, "%s", msg_node_memory);
                                    continue;
                                }
                            }
                        case CUSTOMER_LEAVE:
                            {
                                rb_temp_util_node = RB_ITERATIVE_TREE_SEARCH(T_data_structure_ptr, T_data_structure_ptr->rb_root, customer_ptr->customer_number);
                                if(rb_temp_util_node == T_data_structure_ptr->rb_nil_sentinel)
                                {
                                    fprintf(output_fp, msg_customer_not_found, customer_ptr->customer_number);
                                    continue;
                                }

                                temp_util_node = rb_temp_util_node->parallel_node; /* get parallel node from os-tree */

                                /* assure account is balanced to zero before deletion */
                                if(temp_util_node->balance!= 0)
                                {
                                    fprintf(output_fp, msg_balance_not_zero, temp_util_node->balance);
                                    continue;
                                }

                                /* delete parallel node from os-tree */
                                temp_util_node = OS_DELETE(T_data_structure_ptr, temp_util_node); /* delete os-tree node */
                                free(temp_util_node); /*free memory of deleted os-tree node */

                                /* delete node of rb-tree */
                                rb_temp_util_node = RB_DELETE(T_data_structure_ptr, rb_temp_util_node); /*delete rb-node */
                                free(rb_temp_util_node); /*free memory of rb node */

                                /* print feedback */
                                fprintf(output_fp, feedback_delete_cust, customer_ptr->customer_number);
                                continue;
                            }

                        case QUERY_BALANCE:
                            {
                                rb_temp_util_node = RB_ITERATIVE_TREE_SEARCH(T_data_structure_ptr, T_data_structure_ptr->rb_root, customer_ptr->customer_number);
                                if(rb_temp_util_node == T_data_structure_ptr->rb_nil_sentinel)
                                {
                                    fprintf(output_fp, msg_customer_not_found, customer_ptr->customer_number);
                                    continue;
                                }
                                temp_util_node = rb_temp_util_node->parallel_node;
                                fprintf(output_fp, feedback_balance_query, temp_util_node->balance);
                                continue;
                            }

                        case QUERY_MAX:
                            {
                                if(T_data_structure_ptr->max == T_data_structure_ptr->max_sentinel)
                                {   /* if no nodes are found */
                                    fprintf(output_fp, "%s", feedback_max_query_none);
                                    continue;
                                }
                                fprintf(output_fp, "%s", feedback_max_query);
                                print_customer_data(T_data_structure_ptr->max, output_fp);
                                continue;
                            }
                        case QUERY_NEGETIVE_BALANCE:
                            {
                                temp_util_node = T_data_structure_ptr->min;
                                if(temp_util_node->balance >= 0)
                                    fprintf(output_fp, "%s", feedback_negative_not_found);
                                else
                                {
                                    fprintf(output_fp, "%s", negative_query_title);
                                    OS_ENUMERATE(T_data_structure_ptr, T_data_structure_ptr->root, LONG_MIN, 0, output_fp);
                                    continue;
                                }
                            }
                        default: break;
                    }
                    continue;
                }

                else
                {
                    fprintf(output_fp, "%s :\"%s\"", msg_map_data, current_input_line);
                    continue;
                }
              }

              if(file_counter > 0)
                fprintf(stderr, "File %d: \'%s\' processed, output report successfully generated.\n", file_counter, file_name);

              fprintf(output_fp, "\n%s", long_header_line);
              fprintf(output_fp, "Rank|| First name || Last name\t      || Id num         || Customer\t      || Balance          \t|| Color|\n");
              fprintf(output_fp, "%s", long_header_line);
              OS_INORDER_TREE_WALK(T_data_structure_ptr, T_data_structure_ptr->root, output_fp);
        }
    }

    else /* (argc == 1) no arguments (no files) */
    {
        fprintf(stderr, msg_no_files, argv[0]);
        fgets(current_input_line, MAX_LINE_LENGTH+1, stdin);
        input_fp = stdin;
        output_fp = stderr;
        goto READ_INPUT;

        exit (EXIT_SUCCESS);
    }

    exit (EXIT_SUCCESS);
}


