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
char adminPassword[50] = "admin123";
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
void changeAdminPassword();
void adminMenu();
int customerLogin();
void customerLoginMenu();
void customerRegister();
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

void printHeader(char *title)
{
// Clear screen command for different operating systems
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Mac/Linux
#endif

    printf("\n========================================\n");
    printf("   %s\n", title);
    printf("========================================\n\n");
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pressToContinue()
{
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar();
}

void loginMenu()
{
    int choice;

    do
    {
        printHeader("SUPER SHOP MANAGEMENT SYSTEM");
        printf("1. Admin Login\n");
        printf("2. Customer Login\n");
        printf("3. Exit System\n");
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            adminLogin();
            break;
        case 2:
            customerLoginMenu();
            break; // Changed to customerLoginMenu
        case 3:
            printf("\nExiting system...\n");
            return;
        default:
            printf("\nInvalid choice! Please try again.\n");
            pressToContinue();
        }
    } while (1); // Infinite loop until exit
}

// customer login
void customerLoginMenu()
{
    int choice;

    do
    {
        printHeader("CUSTOMER LOGIN");
        printf("1. Login with Phone & Password\n");
        printf("2. Register New Account\n");
        printf("3. Back to Main Login\n");
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            if (customerLogin() == 1)
            {
                customerMenu(); // Successful login
                return;         // Return after customer menu
            }
            // If login failed, show this menu again
            break;
        case 2:
            customerRegister();
            // After registration, go to customer menu
            if (currentUserId > 0)
            {
                customerMenu();
                return;
            }
            break;
        case 3:
            return; // Back to main login
        default:
            printf("\nInvalid choice! Please try again.\n");
            pressToContinue();
        }
    } while (1);
}

int customerLogin()
{
    char phone[15];
    char password[20];
    int found = 0;

    printHeader("CUSTOMER LOGIN");

    printf("Enter Phone Number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = 0;

    printf("Enter Password: ");
    fgets(password, 20, stdin);
    password[strcspn(password, "\n")] = 0;

    // Search for customer with matching phone and password
    for (int i = 0; i < customerCount; i++)
    {
        if (strcmp(customers[i].phone, phone) == 0 &&
            strcmp(customers[i].password, password) == 0)
        {

            currentUserId = customers[i].id;
            printf("\nLogin successful! Welcome %s.\n", customers[i].name);
            pressToContinue();
            found = 1;
            return 1; // Success
        }
    }

    if (!found)
    {
        printf("\nLogin failed! Incorrect phone number or password.\n");
        pressToContinue();
    }

    return 0; // Failed
}

void customerRegister()
{
    printHeader("CUSTOMER REGISTRATION");

    // Check if we need to expand array
    if (customerCount >= customerCapacity)
    {
        customerCapacity *= 2;
        customers = (Customer *)realloc(customers, customerCapacity * sizeof(Customer));
    }

    Customer newCustomer;
    newCustomer.id = getNextCustomerId();
    newCustomer.totalSpent = 0;

    // Get customer details
    printf("Enter Name: ");
    fgets(newCustomer.name, 50, stdin);
    newCustomer.name[strcspn(newCustomer.name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &newCustomer.age);
    clearInputBuffer();

    // Check if phone already exists
    int phoneExists;
    do
    {
        phoneExists = 0;
        printf("Enter Phone Number: ");
        fgets(newCustomer.phone, 15, stdin);
        newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;

        // Check if phone already registered
        for (int i = 0; i < customerCount; i++)
        {
            if (strcmp(customers[i].phone, newCustomer.phone) == 0)
            {
                printf("Phone number already registered! Please use a different number.\n");
                phoneExists = 1;
                break;
            }
        }
    } while (phoneExists);

    // Get password
    char password1[20], password2[20];
    do
    {
        printf("Enter Password (min 4 characters): ");
        fgets(password1, 20, stdin);
        password1[strcspn(password1, "\n")] = 0;

        printf("Confirm Password: ");
        fgets(password2, 20, stdin);
        password2[strcspn(password2, "\n")] = 0;

        if (strcmp(password1, password2) != 0)
        {
            printf("Passwords don't match! Try again.\n");
        }
        else if (strlen(password1) < 4)
        {
            printf("Password must be at least 4 characters long!\n");
        }
        else
        {
            strcpy(newCustomer.password, password1);
            break;
        }
    } while (1);

    // Add customer to array
    customers[customerCount] = newCustomer;
    customerCount++;

    // Set as logged in
    currentUserId = newCustomer.id;

    printf("\nRegistration successful!\n");
    printf("Your Customer ID: %d\n", newCustomer.id);
    printf("Please remember your phone number and password for login.\n");

    saveCustomersToFile();
    pressToContinue();
}

// admin login
void adminLogin()
{
    char password[50];

    printHeader("ADMIN LOGIN");
    printf("Enter Admin Password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = 0; // Remove newline

    // Check password
    if (strcmp(password, adminPassword) == 0)
    {
        currentUserId = 0; // 0 means admin
        printf("\nLogin successful! Welcome Admin.\n");
        pressToContinue();
        adminMenu();
    }
    else
    {
        printf("\nIncorrect password! Returning to login menu.\n");
        pressToContinue();
    }
}

void changeAdminPassword()
{
    printf("Enter new admin password: ");
    fgets(adminPassword, 50, stdin);
    adminPassword[strcspn(adminPassword, "\n")] = 0;
    printf("Password changed successfully!\n");
}


// admin menu
void adminMenu()
{
    int choice;

    do
    {
        printHeader("ADMIN DASHBOARD");
        printf("1. Product Management\n");
        printf("2. Customer Management\n");
        printf("3. View Sales Report\n");
        printf("4. View Inventory Report\n");
        printf("5. View Data Files\n");
        printf("6. Change Admin Password\n");
        printf("7. Logout\n");
        printf("\nEnter your choice (1-7): ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            productManagement();
            break;
        case 2:
            customerManagement();
            break;
        case 3:
            salesReport();
            break;
        case 4:
            inventoryReport();
            break;
        case 5:
            viewDataFiles();
            break;
        case 6:
            changeAdminPassword();
            break;
        case 7:
            currentUserId = -1;
            printf("\nLogged out successfully!\n");
            pressToContinue();
            return;
        default:
            printf("\nInvalid choice! Please try again.\n");
            pressToContinue();
        }
    } while (1);
}


// customer menu
void customerMenu()
{
    int choice;
    int customerIndex = findCustomerById(currentUserId);
    char customerName[50] = "Customer";

    if (customerIndex != -1)
    {
        strcpy(customerName, customers[customerIndex].name);
    }

    do
    {
        printHeader("CUSTOMER DASHBOARD");
        printf("Welcome, %s!\n", customerName);
        printf("Customer ID: %d\n", currentUserId);

        if (customerIndex != -1)
        {
            printf("Total Spent: %.2f\n", customers[customerIndex].totalSpent);
        }

        printf("\n1. View Products\n");
        printf("2. Generate Bill\n");
        printf("3. View My Purchase History\n");
        printf("4. View My Profile\n");
        printf("5. Logout\n");
        printf("\nEnter your choice (1-5): ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            viewProducts();
            break;
        case 2:
            generateBill();
            break;
        case 3:
            viewCustomerPurchaseHistory();
            break;
        case 4:
            viewCustomerProfile();
            break;
        case 5:
            currentUserId = -1;
            printf("\nLogged out successfully!\n");
            pressToContinue();
            return;
        default:
            printf("\nInvalid choice! Please try again.\n");
            pressToContinue();
        }
    } while (1);
}
