# Bank-account-management-with-OS-RB-TREE

## Description
The goal of this project is managing a dynamic set of in-memory bank accounts :moneybag: proficiently as possible
(by run-time means, as a factor of set size, i.e., the amount of bank accounts in the set).
Managment of the dynamic set includes supporting the basic dictionary CRUD operations and additional custom queries, 
such as finding maximum account in the set & finding all accounts which contain a negative balance. 

Considering the tradeoffs of different data-structures for the problem in hand, 
those which were found as most appropriate were [Red-Black](https://en.wikipedia.org/wiki/Red-black_tree) :red_circle::black_circle: & [Order-Statistic](https://en.wikipedia.org/wiki/Order_statistic_tree) :bar_chart: balanced :balance_scale: binary-search trees :palm_tree:.
These data-structures are fully implemented in this project from scratch, based on the pseudo-code of [CLRS](https://en.wikipedia.org/wiki/Introduction_to_Algorithms). 

For further details of the considerations taken see: [detailed documentation :bookmark_tabs:](https://github.com/cwelt/Bank-account-management-with-OS-RB-TREE/blob/master/Detaild%20Documentation/%D7%9E%D7%9E%D7%9F%2018%20-%202016%D7%90.pdf)

## Modules 
| Module         | Files                                                | Content & Responsibility                                                                                                       |
|----------------|------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------|
| IO_UTILS       | [io_utils.c](io_utils.c) & [io_utils.h](io_utils.h)  | I/O Interaction, Validations & Global Configurations                                                                           |
| OS_TREE        | [os_tree.c](os_tree.c) & [os_tree.h](os_tree.h)      | Index Records by Account Balance, Support RB & OS Operations (CRUD, Queries, Rotations, Rank,..., Including Enumerating Ranges | 
| RB_TREE        | [rb_tree.c](rb_tree.c) & [rb_tree.h](rb_tree.h)      | Index Records by Account Id, Support RB Operations & Bind to OS Tree for Staying in Sync                                       |                       
| MAIN           | [main.c](main.c)                                     | Initialization, Memory Management, Controller of Program Flow, Output Log                                                      |

## Input/Output Examples 
- **[Example 1](https://github.com/cwelt/Bank-account-management-with-OS-RB-TREE/tree/master/IO%20Examples/Example1)**
    - [File1 *Input*](/IO Examples/Example1/chanan_example.txt)
    - [File1 *Output*](https://github.com/cwelt/Bank-account-management-with-OS-RB-TREE/blob/master/IO%20Examples/Example1/chanan_example.txt_Sun%20Mar%2013_output.txt) 
- **[Example 2](https://github.com/cwelt/Bank-account-management-with-OS-RB-TREE/tree/master/IO%20Examples/Example2)**
    - [File2 *Input*](https://github.com/cwelt/Bank-account-management-with-OS-RB-TREE/blob/master/IO%20Examples/Example2/maman_example.txt)
    - [File2 *Output*](https://github.com/cwelt/Bank-account-management-with-OS-RB-TREE/blob/master/IO%20Examples/Example2/maman_example.txt_Sun%20Mar%2013_output.txt) 

![tree](https://walkccc.github.io/CLRS/img/13.1-1-2.png)
