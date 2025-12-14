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
char *categories[] = {
    "Beverages",
    "Snacks",
    "Dairy",
    "Grocery",
    "Fruits",
    "Vegetables",
    "Meat",
    "Fish",
    "Bakery",
    "Toiletries",
    "Cleaning",
    "Frozen Food",
    "Baby Care",
    "Pet Care",
    "Stationery"};
int categoryCount = 15;

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


// product management
void productManagement()
{
    int choice;

    do
    {
        printHeader("PRODUCT MANAGEMENT");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Search Product\n");
        printf("6. Back to Admin Menu\n");
        printf("\nEnter your choice (1-6): ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            viewProducts();
            break;
        case 3:
            updateProduct();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            searchProduct();
            break;
        case 6:
            return;
        default:
            printf("\nInvalid choice! Please try again.\n");
            pressToContinue();
        }
    } while (1);
}

void addProduct()
{
    printHeader("ADD NEW PRODUCT");

    // Check if we need to expand array
    if (productCount >= productCapacity)
    {
        productCapacity *= 2;
        products = (Product *)realloc(products, productCapacity * sizeof(Product));
    }

    Product newProduct;
    newProduct.id = getNextProductId();

    printf("Enter Product Name: ");
    fgets(newProduct.name, 50, stdin);
    newProduct.name[strcspn(newProduct.name, "\n")] = 0;

    // Show categories
    printf("\nAvailable Categories:\n");
    for (int i = 0; i < categoryCount; i++)
    {
        printf("%2d. %s\n", i + 1, categories[i]);
    }

    int categoryChoice;
    do
    {
        printf("\nSelect Category (1-%d): ", categoryCount);
        scanf("%d", &categoryChoice);
        clearInputBuffer();

        if (categoryChoice < 1 || categoryChoice > categoryCount)
        {
            printf("Invalid choice! Please select between 1 and %d.\n", categoryCount);
        }
        else
        {
            strcpy(newProduct.category, categories[categoryChoice - 1]);
            break;
        }
    } while (1);

    printf("Enter Price: ");
    scanf("%f", &newProduct.price);

    printf("Enter Quantity: ");
    scanf("%d", &newProduct.quantity);
    clearInputBuffer();

    printf("Enter Expiry Date (DD/MM/YYYY): ");
    fgets(newProduct.expiryDate, 15, stdin);
    newProduct.expiryDate[strcspn(newProduct.expiryDate, "\n")] = 0;

    // Add to array
    products[productCount] = newProduct;
    productCount++;

    printf("\nProduct added successfully!\n");
    printf("Product ID: %d\n", newProduct.id);
    printf("Product Name: %s\n", newProduct.name);
    printf("Category: %s\n", newProduct.category);
    printf("Price: %.2f | Quantity: %d\n", newProduct.price, newProduct.quantity);

    // Save to file
    saveProductsToFile();
    pressToContinue();
}

void viewProducts()
{
    printHeader("ALL PRODUCTS");

    if (productCount == 0)
    {
        printf("No products available in inventory!\n");
        pressToContinue();
        return;
    }

    printf("===========================================================================================\n");
    printf("ID    | Name                          | Category        | Price    | Qty  | Expiry Date\n");
    printf("===========================================================================================\n");

    for (int i = 0; i < productCount; i++)
    {
        printf("%-5d | %-30s | %-15s | %-8.2f | %-4d | %-15s\n",
               products[i].id,
               products[i].name,
               products[i].category,
               products[i].price,
               products[i].quantity,
               products[i].expiryDate);
    }

    printf("===========================================================================================\n");
    printf("Total Products: %d\n", productCount);

    // Calculate and show inventory value
    float totalValue = 0;
    for (int i = 0; i < productCount; i++)
    {
        totalValue += products[i].price * products[i].quantity;
    }
    printf("Total Inventory Value: %.2f\n", totalValue);

    pressToContinue();
}

void updateProduct()
{
    printHeader("UPDATE PRODUCT");

    if (productCount == 0)
    {
        printf("No products available to update!\n");
        pressToContinue();
        return;
    }

    int id, index;
    printf("Enter Product ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    index = findProductById(id);

    if (index == -1)
    {
        printf("Product with ID %d not found!\n", id);
        pressToContinue();
        return;
    }

    printf("\nCurrent Details of Product ID %d:\n", id);
    printf("1. Name: %s\n", products[index].name);
    printf("2. Category: %s\n", products[index].category);
    printf("3. Price: %.2f\n", products[index].price);
    printf("4. Quantity: %d\n", products[index].quantity);
    printf("5. Expiry Date: %s\n", products[index].expiryDate);

    printf("\nWhich field do you want to update?\n");
    printf("1. Name\n");
    printf("2. Category\n");
    printf("3. Price\n");
    printf("4. Quantity\n");
    printf("5. Expiry Date\n");
    printf("6. Update All Fields\n");
    printf("7. Cancel\n");

    int choice;
    printf("\nEnter your choice (1-7): ");
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice)
    {
    case 1: // Update Name
        printf("Enter new Product Name (current: %s): ", products[index].name);
        fgets(products[index].name, 50, stdin);
        products[index].name[strcspn(products[index].name, "\n")] = 0;
        break;

    case 2: // Update Category
        printf("\nAvailable Categories:\n");
        for (int i = 0; i < categoryCount; i++)
        {
            printf("%2d. %s\n", i + 1, categories[i]);
        }
        int catChoice;
        do
        {
            printf("\nSelect new Category (1-%d, current: %s): ", categoryCount, products[index].category);
            scanf("%d", &catChoice);
            clearInputBuffer();

            if (catChoice < 1 || catChoice > categoryCount)
            {
                printf("Invalid choice! Please select between 1 and %d.\n", categoryCount);
            }
            else
            {
                strcpy(products[index].category, categories[catChoice - 1]);
                break;
            }
        } while (1);
        break;

    case 3: // Update Price
        printf("Enter new Price (current: %.2f): ", products[index].price);
        scanf("%f", &products[index].price);
        clearInputBuffer();
        break;

    case 4: // Update Quantity
        printf("Enter new Quantity (current: %d): ", products[index].quantity);
        scanf("%d", &products[index].quantity);
        clearInputBuffer();
        break;

    case 5: // Update Expiry Date
        printf("Enter new Expiry Date (DD/MM/YYYY) (current: %s): ", products[index].expiryDate);
        fgets(products[index].expiryDate, 15, stdin);
        products[index].expiryDate[strcspn(products[index].expiryDate, "\n")] = 0;
        break;

    case 6: // Update All
        printf("Enter new Product Name (current: %s): ", products[index].name);
        fgets(products[index].name, 50, stdin);
        products[index].name[strcspn(products[index].name, "\n")] = 0;

        printf("\nAvailable Categories:\n");
        for (int i = 0; i < categoryCount; i++)
        {
            printf("%2d. %s\n", i + 1, categories[i]);
        }
        do
        {
            printf("\nSelect new Category (1-%d, current: %s): ", categoryCount, products[index].category);
            scanf("%d", &catChoice);
            clearInputBuffer();

            if (catChoice < 1 || catChoice > categoryCount)
            {
                printf("Invalid choice! Please select between 1 and %d.\n", categoryCount);
            }
            else
            {
                strcpy(products[index].category, categories[catChoice - 1]);
                break;
            }
        } while (1);

        printf("Enter new Price (current: %.2f): ", products[index].price);
        scanf("%f", &products[index].price);
        clearInputBuffer();

        printf("Enter new Quantity (current: %d): ", products[index].quantity);
        scanf("%d", &products[index].quantity);
        clearInputBuffer();

        printf("Enter new Expiry Date (DD/MM/YYYY) (current: %s): ", products[index].expiryDate);
        fgets(products[index].expiryDate, 15, stdin);
        products[index].expiryDate[strcspn(products[index].expiryDate, "\n")] = 0;
        break;

    case 7: // Cancel
        printf("Update cancelled.\n");
        pressToContinue();
        return;

    default:
        printf("Invalid choice! Update cancelled.\n");
        pressToContinue();
        return;
    }

    printf("\nProduct updated successfully!\n");
    saveProductsToFile();
    pressToContinue();
}

void deleteProduct()
{
    printHeader("DELETE PRODUCT");

    if (productCount == 0)
    {
        printf("No products available to delete!\n");
        pressToContinue();
        return;
    }

    int id, index;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    index = findProductById(id);

    if (index == -1)
    {
        printf("Product with ID %d not found!\n", id);
        pressToContinue();
        return;
    }

    printf("\nProduct Details:\n");
    printf("ID: %d\n", products[index].id);
    printf("Name: %s\n", products[index].name);
    printf("Category: %s\n", products[index].category);
    printf("Price: %.2f\n", products[index].price);
    printf("Quantity: %d\n", products[index].quantity);
    printf("Expiry Date: %s\n", products[index].expiryDate);

    char confirm;
    printf("\nAre you sure you want to delete this product? (y/n): ");
    scanf("%c", &confirm);
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y')
    {
        // Shift all products after the deleted one
        for (int i = index; i < productCount - 1; i++)
        {
            products[i] = products[i + 1];
        }
        productCount--;

        printf("\nProduct deleted successfully!\n");
        saveProductsToFile();
    }
    else
    {
        printf("\nDeletion cancelled.\n");
    }

    pressToContinue();
}

