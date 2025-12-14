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
    int age;
    char phone[15];
    float totalSpent;
    char password[20];
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
int currentUserId = -1; // -1 means not logged in, 0 means admin, >0 means customer ID

// Function Prototypes
void initializeSystem();
void loginMenu();
void adminLogin();
void customerLogin();
void customerRegister();
void adminMenu();
void customerMenu();
void mainMenu(); // Renamed from previous mainMenu to adminMenu
void productManagement();
void billingSystem();
void customerManagement();
void inventoryReport();
void salesReport();
void viewDataFiles();

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

// Billing Functions
void generateBill();
void addToCart(int *cartIds, int *cartQuantities, float *cartPrices, int *itemCount);
void calculateBill(int *cartIds, int *cartQuantities, float *cartPrices, int itemCount, int customerId, char *customerName);
void saveSaleToFile(Sale sale);
void loadSalesFromFile();

// Utility Functions
int getNextProductId();
int getNextCustomerId();
int getNextBillNo();
int findProductById(int id);
int findCustomerById(int id);
int findCustomerByPhone(char *phone);
void clearInputBuffer();
void printHeader(char *title);
void pressToContinue();

// File Operations
void saveAllData();
void loadAllData();

int main()
{
    initializeSystem();
    loadAllData();
    loginMenu();
    saveAllData();

    free(products);
    free(customers);
    free(sales);

    printf("\nThank you for using Super Shop Management System!\n");
    return 0;
}

void initializeSystem()
{
    // Initialize product array
    products = (Product *)malloc(productCapacity * sizeof(Product));
    if (!products)
    {
        printf("Memory allocation failed for products!\n");
        exit(1);
    }

    // Initialize customer array
    customers = (Customer *)malloc(customerCapacity * sizeof(Customer));
    if (!customers)
    {
        printf("Memory allocation failed for customers!\n");
        exit(1);
    }

    // Initialize sales array
    sales = (Sale *)malloc(saleCapacity * sizeof(Sale));
    if (!sales)
    {
        printf("Memory allocation failed for sales!\n");
        exit(1);
    }
}