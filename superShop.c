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

// Global variables
Product *products = NULL;
Customer *customers = NULL;
Sale *sales = NULL;
int productCount = 0;
int customerCount = 0;
int saleCount = 0;
int productCapacity = 10;
int customerCapacity = 10;
int saleCapacity = 10;

// Function Prototypes
void initializeSystem();
void mainMenu();
void productManagement();
void billingSystem();
void customerManagement();
void inventoryReport();
void salesReport();
void viewDataFiles();
void viewProductsFile();
void viewCustomersFile();
void viewSalesFile();

// Product Management Functions
void addProduct();
void viewProducts();
void updateProduct();
void deleteProduct();
void searchProduct();
void saveProductsToFile();
void loadProductsFromFile();

// Customer Management Functions
void addCustomer();
void viewCustomers();
void saveCustomersToFile();
void loadCustomersFromFile();