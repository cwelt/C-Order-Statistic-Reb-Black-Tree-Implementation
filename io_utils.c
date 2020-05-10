
#include "io_utils.h"

/* constant text element definitions - error messages, headers, etc */
const char msg_main_memory[] = "\nMemory Error: System failure occurred in attempt to initialize main data structure\n";
const char msg_node_memory[] = "\nMemory Error: System failure occurred in attempt to allocate additional memory\n";
const char msg_max_files[] = "%s: Warning: Maximum files allowed is %d. the remaining files won't be taken into account.\n";
const char msg_no_files[] = "\n%s:\n\nNo files found for input.\nPlease enter queries manually, or \'q\' t quit\n\n";
const char msg_file_name_length[] = "%s:Error: File %s name exceeds max length %d, therefore it is not being processed.\n";
const char msg_file_open_failure[] = "failure occurred in attempt to open file";
const char msg_line_length[] = "Line exceeds max length of %d, therefore it is not being processed.\n";
const char msg_illegal_op[] = "error: unknown query: \"%s\"\n";
const char msg_map_data[] = "error: mapping failure due to invalid data arguments\n";
const char msg_balance_not_zero[] = "error: customer has uncleared balance of %-.2f\n";
const char msg_customer_already_exsits[] = "error: customer already exists!\n";
const char msg_customer_not_found[] = "error: requested customer %ld not found\n";
const char msg_file_creation_err[] = "error occurred in attempt to create output file.\n";
const char header_line[] = "=========================\n";
const char long_header_line[] = "==========================================================================\n";
const char report_title[] = "Detailed log report for file \' %s \'\n";
const char negative_query_title[] = "All customers with a negative balance:\n";
const char feedback_negative_not_found[] = "No results: all customers have a balance >= to 0.00\n";
const char feedback_new_cust[] = "Customer was successfully added - \n\tName: %s %s, number: %ld, ID: %s, balance: %-.2f\n";
const char feedback_balance_upd[] = "Balance updated- Old balance: %-.2f, New balance: %-.2f\n";
const char feedback_balance_query[] = "Current Balance: %-.2f\n";
const char feedback_max_query[] = "Customer with highest balance is -\n";
const char feedback_max_query_none[] = "No customers exist.\n";
const char feedback_delete_cust[] = "Customer %ld successfully deleted.\n";
const char space_delimeters[] = "\b\t\n ";
const char max_op[] = "MAX";
const char minus_op[] = "MINUS";
const char output_file_name_suffix[] = "_output.txt";

/******************************************************************/
/* determine_operation: get text from input and checks if it is a */
/* legal command (operation) and if so, it determines which is it */
/******************************************************************/
int determine_operation (char *input_line)
{
    char *util_ptr;
    char token[MAX_LINE_LENGTH];
    strcpy(token, input_line);
    if((util_ptr = strtok(token, space_delimeters)))
    {
        switch (util_ptr[0])
        {
            case '+': return NEW_CUSTOMER;
            case '-': return CUSTOMER_LEAVE;
            case '?':
            {
                strcpy(token, input_line);
                util_ptr = strtok(token, "?\b\t\n "); /*get next token */
                if(util_ptr != NULL)
                {
                    if (isdigit(util_ptr[0]))
                        return QUERY_BALANCE;
                    else if ((strcmp(util_ptr, max_op)) == 0)
                          return QUERY_MAX;
                    else if ((strcmp(util_ptr, minus_op)) == 0)
                        return QUERY_NEGETIVE_BALANCE;
                }
                else
                {
                    strcpy(input_line, util_ptr);
                    return NOT_FOUND;
                }
            }

            default:
                if(isalpha(util_ptr[0]))
                    return DEPOSIT_WITHDRAW;
                else
                {
                    strcpy(input_line, util_ptr);
                    return NOT_FOUND;
                }
        }
    }
    else return EMPTY_LINE; /* input line was empty */
}


/******************************************************************/
/* map_data: get raw data from input and pre-determined command   */
/* and maps each argument to the corresponding parameter.         */
/******************************************************************/
int map_data (int operation, char *input_line,  customer *customer_ptr)
{
    int scan_indicator;
    double sum = 0;
    long customer_num = 0;
    char *util_ptr;
    char current_token[MAX_LINE_LENGTH];
    strcpy(current_token, input_line);
    util_ptr = strtok(current_token, "+-?\b\t\n ");
    switch (operation)
    {
        case QUERY_MAX: case QUERY_NEGETIVE_BALANCE:
             return EXIT_SUCCESS;
        case CUSTOMER_LEAVE: case QUERY_BALANCE:
        {
            scan_indicator = sscanf(util_ptr, "%ld", &customer_num);
            if((scan_indicator != NULL) && (scan_indicator != EOF))
            {
                customer_ptr->customer_number = customer_num;
                return EXIT_SUCCESS;
            }
            return EXIT_FAILURE;
        }
        case DEPOSIT_WITHDRAW: case NEW_CUSTOMER:
        {
            if(!isalpha(util_ptr[0]))
                return EXIT_FAILURE;
            strcpy(customer_ptr->first_name, util_ptr);
            util_ptr = strtok(NULL, space_delimeters);
            if(!isalpha(util_ptr[0]))
                return EXIT_FAILURE;
            strcpy(customer_ptr->last_name, util_ptr);
            util_ptr = strtok(NULL, space_delimeters);
            if(operation == NEW_CUSTOMER)
            {
                if(!isdigit(util_ptr[0]))
                    return EXIT_FAILURE;
                strncpy(customer_ptr->id_number, util_ptr, ID_NUM_LENGTH+1);
                util_ptr = strtok(NULL, space_delimeters);
            }
            if(((sscanf(util_ptr, "%ld", &customer_num)) != NULL) && (isdigit(util_ptr[0])))
                customer_ptr->customer_number = customer_num;
            else return EXIT_FAILURE;
            util_ptr = strtok(NULL, space_delimeters);
            if(util_ptr == NULL)
                return EXIT_FAILURE;
            scan_indicator = sscanf(util_ptr, "%s", &current_token);
            if((scan_indicator != NULL) && (scan_indicator != EOF))
            {
                sum = atof(current_token);
                customer_ptr->balance = sum;
                return EXIT_SUCCESS;
            }
            else return EXIT_FAILURE;
        }
        default: break;
    }
    return EXIT_SUCCESS;
}

