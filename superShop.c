#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


// Structure Definitions
typedef struct
{
    int id;
    char name[50];
    char category[30];
    float price;
    int quantity;
    char expiryDate[15];
} Product;

typedef struct
{
    int id;
    char name[50];
    char phone[15];
    float totalSpent;
} Customer;

typedef struct
{
    int billNo;
    char date[15];
    int customerId;
    char customerName[50];
    float totalAmount;
    int itemCount;
} Sale;