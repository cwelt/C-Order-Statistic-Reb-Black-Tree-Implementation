#ifndef IO_UTILS_H_INCLUDED
#define IO_UTILS_H_INCLUDED

/*********************************/
/* HEADER FILE FOR io_utils.c     */
/*********************************/

/* header files inclusion */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <float.h>

/* constants definitions */
#define FILE_MAX_NAME 30
#define FILE_OUT_MAX_NAME 60
#define TIMESTAMP_LENGTH 10
#define MAX_FILES 10
#define MAX_LINE_LENGTH 80
#define EMPTY_LINE 0
#define ID_NUM_LENGTH 9
#define CUSTOMER_NAME_LENGTH 30
#define NIL_SENTINEL -1
#define NOT_FOUND -1
#define ERROR 0

enum operation {DEPOSIT_WITHDRAW = 1, NEW_CUSTOMER, CUSTOMER_LEAVE, QUERY_BALANCE, QUERY_MAX, QUERY_NEGETIVE_BALANCE};
typedef enum {RED, BLACK} rb_node_color;

/* constant text elements: error messages, notifications, etc */
extern const char msg_main_memory[];
extern const char msg_node_memory[];
extern const char msg_max_files[];
extern const char msg_no_files[];
extern const char msg_file_name_length[];
extern const char msg_file_open_failure[];
extern const char msg_line_length[];
extern const char msg_illegal_op[];
extern const char msg_map_data[];
extern const char msg_customer_not_found[];
extern const char msg_balance_not_zero[];
extern const char msg_customer_already_exsits[];
extern const char msg_file_creation_err[];
extern const char header_line[];
extern const char long_header_line[];
extern const char report_title[];
extern const char negative_query_title[];
extern const char feedback_negative_not_found[];
extern const char feedback_new_cust[];
extern const char feedback_balance_upd[];
extern const char feedback_delete_cust[];
extern const char feedback_balance_query[];
extern const char feedback_max_query[];
extern const char feedback_max_query_none[];
extern const char space_delimeters[];
extern const char max_op[];
extern const char minus_op[];
extern const char output_file_name_suffix[];

/* structure for customer satellite data from input */
typedef struct customer_info_record
{
    char id_number[ID_NUM_LENGTH+1];
    unsigned long customer_number;
    char first_name[CUSTOMER_NAME_LENGTH];
    char last_name[CUSTOMER_NAME_LENGTH];
    double balance;
} customer;

/* structure for red-black tree node */
typedef struct rb_tree_info_record
{
  struct rb_tree_info_record *parent;
  struct rb_tree_info_record *left;
  struct rb_tree_info_record *right;
  struct os_tree_node *parallel_node; /* pointer to corresponding node from os-tree */
  rb_node_color color;
  long customer_num; /* key */
} rb_tree_node;

/* structure for order-statistic tree node */
typedef struct os_tree_info_record
{
  struct os_tree_node *parent;
  struct os_tree_node *left;
  struct os_tree_node *right;
  struct rb_tree_node *parallel_node;
  rb_node_color color;
  unsigned int size;
  char first_name[CUSTOMER_NAME_LENGTH];
  char last_name[CUSTOMER_NAME_LENGTH];
  char id_number[ID_NUM_LENGTH];
  long customer_num;
  double balance;

} os_tree_node;

/* structure for header of the main data structure */
typedef struct os_tree_header_record
{
  os_tree_node *root;
  struct rb_tree_node *rb_root;
  struct os_tree_node *nil_sentinel;
  struct os_tree_node *rb_nil_sentinel;
  struct os_tree_node *max_sentinel;
  struct os_tree_node *min_sentinel;
  struct os_tree_node *max;
  struct os_tree_node *min;
} os_tree_header;


/* function declarations */
int determine_operation (char *input_line);
int map_data (int operation, char *input_line, customer *customer_ptr);


#endif // IO_UTILS_H_INCLUDED
