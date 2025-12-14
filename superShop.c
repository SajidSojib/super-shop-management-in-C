#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ============== STRUCTURE DEFINITIONS ==============
typedef struct
{
    int id;
    char name[50];
    char category[30];
    float price;
    int quantity;
    char expiryDate[15]; // Display only, no tracking
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

// ============== GLOBAL VARIABLES ==============
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
int currentUserId = -1;
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

// ============== FUNCTION PROTOTYPES ==============
// Utility Functions
void initializeSystem();
void printHeader(char *title);
void clearInputBuffer();
void pressToContinue();

// Authentication Module
void loginMenu();
void adminLogin();
void changeAdminPassword();
void adminMenu();
void customerLoginMenu();
int customerLogin();
void customerRegister();
void customerMenu();

// Product Management Module
void productManagement();
void addProduct();
void viewProducts();
void updateProduct();
void deleteProduct();
void searchProduct();
void searchByID();
void searchByName();
void searchByCategory();
void searchByPriceRange();
int getNextProductId();
int findProductById(int id);
void saveProductsToFile();
void loadProductsFromFile();

// Customer Management Module
void customerManagement();
void viewCustomers();
int getNextCustomerId();
int findCustomerById(int id);
int findCustomerByPhone(char *phone);
void saveCustomersToFile();
void loadCustomersFromFile();
void viewCustomerPurchaseHistory();
void viewCustomerProfile();
void editCustomerProfile(int index);
void changeCustomerPassword(int index);

// Billing Module
void generateBill();
int getNextBillNo();
void saveSaleToFile(Sale sale);
void loadSalesFromFile();

// Reporting Module
void salesReport();
void inventoryReport();

// Data Management
void saveAllData();
void loadAllData();

// ============== MAIN FUNCTION ==============
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

// ============== UTILITY FUNCTIONS ==============
void initializeSystem()
{
    products = (Product *)malloc(productCapacity * sizeof(Product));
    if (!products)
    {
        printf("Memory allocation failed for products!\n");
        exit(1);
    }

    customers = (Customer *)malloc(customerCapacity * sizeof(Customer));
    if (!customers)
    {
        printf("Memory allocation failed for customers!\n");
        exit(1);
    }

    sales = (Sale *)malloc(saleCapacity * sizeof(Sale));
    if (!sales)
    {
        printf("Memory allocation failed for sales!\n");
        exit(1);
    }
}

void printHeader(char *title)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
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

// ============== AUTHENTICATION MODULE ==============
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
            break;
        case 3:
            printf("\nExiting system...\n");
            return;
        default:
            printf("\nInvalid choice! Please try again.\n");
            pressToContinue();
        }
    } while (1);
}

// ============== ADMIN AUTHENTICATION ==============
void adminLogin()
{
    char password[50];

    printHeader("ADMIN LOGIN");
    printf("Enter Admin Password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = 0;

    if (strcmp(password, adminPassword) == 0)
    {
        currentUserId = 0;
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
        printf("5. Change Admin Password\n"); // Removed "View Data Files" option
        printf("6. Logout\n");
        printf("\nEnter your choice (1-6): "); // Changed from 1-7 to 1-6
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
            changeAdminPassword();
            break;
        case 6: // Changed from case 7
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

// ============== CUSTOMER AUTHENTICATION ==============
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
                customerMenu();
                return;
            }
            break;
        case 2:
            customerRegister();
            if (currentUserId > 0)
            {
                customerMenu();
                return;
            }
            break;
        case 3:
            return;
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

    printHeader("CUSTOMER LOGIN");

    printf("Enter Phone Number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = 0;

    printf("Enter Password: ");
    fgets(password, 20, stdin);
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < customerCount; i++)
    {
        if (strcmp(customers[i].phone, phone) == 0 &&
            strcmp(customers[i].password, password) == 0)
        {
            currentUserId = customers[i].id;
            printf("\nLogin successful! Welcome %s.\n", customers[i].name);
            pressToContinue();
            return 1;
        }
    }

    printf("\nLogin failed! Incorrect phone number or password.\n");
    pressToContinue();
    return 0;
}

void customerRegister()
{
    printHeader("CUSTOMER REGISTRATION");

    if (customerCount >= customerCapacity)
    {
        customerCapacity *= 2;
        customers = (Customer *)realloc(customers, customerCapacity * sizeof(Customer));
    }

    Customer newCustomer;
    newCustomer.id = getNextCustomerId();
    newCustomer.totalSpent = 0;

    printf("Enter Name: ");
    fgets(newCustomer.name, 50, stdin);
    newCustomer.name[strcspn(newCustomer.name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &newCustomer.age);
    clearInputBuffer();

    int phoneExists;
    do
    {
        phoneExists = 0;
        printf("Enter Phone Number: ");
        fgets(newCustomer.phone, 15, stdin);
        newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;

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

    customers[customerCount] = newCustomer;
    customerCount++;
    currentUserId = newCustomer.id;

    printf("\nRegistration successful!\n");
    printf("Your Customer ID: %d\n", newCustomer.id);
    printf("Please remember your phone number and password for login.\n");

    saveCustomersToFile();
    pressToContinue();
}

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

// ============== PRODUCT MANAGEMENT MODULE ==============
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

// ---- ADD PRODUCT FUNCTIONS ----
int getNextProductId()
{
    int maxId = 1000;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id > maxId)
        {
            maxId = products[i].id;
        }
    }
    return maxId + 1;
}

void addProduct()
{
    printHeader("ADD NEW PRODUCT");

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

    printf("Enter Expiry Date (DD/MM/YYYY) [Display Only]: ");
    fgets(newProduct.expiryDate, 15, stdin);
    newProduct.expiryDate[strcspn(newProduct.expiryDate, "\n")] = 0;

    products[productCount] = newProduct;
    productCount++;

    printf("\nProduct added successfully!\n");
    printf("Product ID: %d\n", newProduct.id);
    printf("Product Name: %s\n", newProduct.name);
    printf("Category: %s\n", newProduct.category);
    printf("Price: %.2f | Quantity: %d\n", newProduct.price, newProduct.quantity);

    saveProductsToFile();
    pressToContinue();
}

// ---- VIEW PRODUCTS FUNCTIONS ----
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

    float totalValue = 0;
    for (int i = 0; i < productCount; i++)
    {
        totalValue += products[i].price * products[i].quantity;
    }
    printf("Total Inventory Value: %.2f\n", totalValue);

    pressToContinue();
}

// ---- UPDATE PRODUCT FUNCTIONS ----
int findProductById(int id)
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            return i;
        }
    }
    return -1;
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
    case 1:
        printf("Enter new Product Name (current: %s): ", products[index].name);
        fgets(products[index].name, 50, stdin);
        products[index].name[strcspn(products[index].name, "\n")] = 0;
        break;

    case 2:
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

    case 3:
        printf("Enter new Price (current: %.2f): ", products[index].price);
        scanf("%f", &products[index].price);
        clearInputBuffer();
        break;

    case 4:
        printf("Enter new Quantity (current: %d): ", products[index].quantity);
        scanf("%d", &products[index].quantity);
        clearInputBuffer();
        break;

    case 5:
        printf("Enter new Expiry Date (DD/MM/YYYY) (current: %s): ", products[index].expiryDate);
        fgets(products[index].expiryDate, 15, stdin);
        products[index].expiryDate[strcspn(products[index].expiryDate, "\n")] = 0;
        break;

    case 6:
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

    case 7:
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

// ---- DELETE PRODUCT FUNCTIONS ----
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

// ---- SEARCH PRODUCT FUNCTIONS ----
void searchProduct()
{
    printHeader("SEARCH PRODUCT");

    if (productCount == 0)
    {
        printf("No products available to search!\n");
        pressToContinue();
        return;
    }

    int choice;
    printf("Search by:\n");
    printf("1. Product ID\n");
    printf("2. Product Name\n");
    printf("3. Category\n");
    printf("4. Price Range\n");
    printf("5. Back\n");
    printf("\nEnter your choice (1-5): ");
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice)
    {
    case 1:
        searchByID();
        break;
    case 2:
        searchByName();
        break;
    case 3:
        searchByCategory();
        break;
    case 4:
        searchByPriceRange();
        break;
    case 5:
        return;
    default:
        printf("Invalid choice!\n");
        pressToContinue();
        return;
    }

    pressToContinue();
}

void searchByID()
{
    int searchId;
    printf("\nEnter Product ID: ");
    scanf("%d", &searchId);
    clearInputBuffer();

    printf("\nSearch Results:\n");
    printf("===========================================================================================\n");
    printf("ID    | Name                          | Category        | Price    | Qty  | Expiry Date\n");
    printf("===========================================================================================\n");

    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == searchId)
        {
            printf("%-5d | %-30s | %-15s | %-8.2f | %-4d | %-15s\n",
                   products[i].id,
                   products[i].name,
                   products[i].category,
                   products[i].price,
                   products[i].quantity,
                   products[i].expiryDate);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("No product found with ID %d\n", searchId);
    }
    printf("===========================================================================================\n");
}

void searchByName()
{
    char searchTerm[50];
    printf("\nEnter Product Name (or part of name): ");
    fgets(searchTerm, 50, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    printf("\nSearch Results:\n");
    printf("===========================================================================================\n");
    printf("ID    | Name                          | Category        | Price    | Qty  | Expiry Date\n");
    printf("===========================================================================================\n");

    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        char productNameLower[50];
        char searchTermLower[50];

        strcpy(productNameLower, products[i].name);
        strcpy(searchTermLower, searchTerm);

        for (int j = 0; productNameLower[j]; j++)
        {
            productNameLower[j] = tolower(productNameLower[j]);
        }
        for (int j = 0; searchTermLower[j]; j++)
        {
            searchTermLower[j] = tolower(searchTermLower[j]);
        }

        if (strstr(productNameLower, searchTermLower) != NULL)
        {
            printf("%-5d | %-30s | %-15s | %-8.2f | %-4d | %-15s\n",
                   products[i].id,
                   products[i].name,
                   products[i].category,
                   products[i].price,
                   products[i].quantity,
                   products[i].expiryDate);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No products found containing '%s'\n", searchTerm);
    }
    printf("===========================================================================================\n");
}

void searchByCategory()
{
    printf("\nAvailable Categories:\n");
    for (int i = 0; i < categoryCount; i++)
    {
        printf("%2d. %s\n", i + 1, categories[i]);
    }

    int catChoice;
    printf("\nSelect Category (1-%d): ", categoryCount);
    scanf("%d", &catChoice);
    clearInputBuffer();

    if (catChoice < 1 || catChoice > categoryCount)
    {
        printf("Invalid category selection!\n");
        return;
    }

    char *selectedCategory = categories[catChoice - 1];

    printf("\nSearch Results for '%s':\n", selectedCategory);
    printf("===========================================================================================\n");
    printf("ID    | Name                          | Category        | Price    | Qty  | Expiry Date\n");
    printf("===========================================================================================\n");

    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].category, selectedCategory) == 0)
        {
            printf("%-5d | %-30s | %-15s | %-8.2f | %-4d | %-15s\n",
                   products[i].id,
                   products[i].name,
                   products[i].category,
                   products[i].price,
                   products[i].quantity,
                   products[i].expiryDate);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No products found in category '%s'\n", selectedCategory);
    }
    printf("===========================================================================================\n");
}

void searchByPriceRange()
{
    float minPrice, maxPrice;
    printf("\nEnter Minimum Price: ");
    scanf("%f", &minPrice);
    printf("Enter Maximum Price: ");
    scanf("%f", &maxPrice);
    clearInputBuffer();

    if (minPrice > maxPrice)
    {
        printf("Minimum price cannot be greater than maximum price!\n");
        return;
    }

    printf("\nSearch Results (Price Range: %.2f - %.2f):\n", minPrice, maxPrice);
    printf("===========================================================================================\n");
    printf("ID    | Name                          | Category        | Price    | Qty  | Expiry Date\n");
    printf("===========================================================================================\n");

    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].price >= minPrice && products[i].price <= maxPrice)
        {
            printf("%-5d | %-30s | %-15s | %-8.2f | %-4d | %-15s\n",
                   products[i].id,
                   products[i].name,
                   products[i].category,
                   products[i].price,
                   products[i].quantity,
                   products[i].expiryDate);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No products found in price range %.2f - %.2f\n", minPrice, maxPrice);
    }
    printf("===========================================================================================\n");
}

// ---- PRODUCT FILE OPERATIONS ----
void saveProductsToFile()
{
    FILE *file = fopen("products.txt", "w");
    if (file == NULL)
    {
        printf("Error saving products to file!\n");
        return;
    }

    fprintf(file, "===========================================================================================\n");
    fprintf(file, "ID    | Name                          | Category        | Price    | Qty  | Expiry Date\n");
    fprintf(file, "===========================================================================================\n");

    for (int i = 0; i < productCount; i++)
    {
        fprintf(file, "%-5d | %-30s | %-15s | %-8.2f | %-4d | %-15s\n",
                products[i].id,
                products[i].name,
                products[i].category,
                products[i].price,
                products[i].quantity,
                products[i].expiryDate);
    }

    fprintf(file, "===========================================================================================\n");
    fprintf(file, "Total Products: %d\n", productCount);

    fclose(file);
}

void loadProductsFromFile()
{
    FILE *file = fopen("products.txt", "r");
    if (file == NULL)
    {
        return;
    }

    productCount = 0;
    char line[200];

    for (int i = 0; i < 3; i++)
    {
        fgets(line, sizeof(line), file);
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "=====") != NULL || strstr(line, "Total Products:") != NULL)
        {
            continue;
        }

        Product p;
        if (sscanf(line, "%d | %49[^|] | %29[^|] | %f | %d | %14[^\n]",
                   &p.id, p.name, p.category, &p.price, &p.quantity, p.expiryDate) >= 5)
        {
            p.name[strcspn(p.name, " ")] = '\0';
            p.category[strcspn(p.category, " ")] = '\0';

            if (productCount >= productCapacity)
            {
                productCapacity *= 2;
                products = (Product *)realloc(products, productCapacity * sizeof(Product));
            }
            products[productCount] = p;
            productCount++;
        }
    }

    fclose(file);
}

// ============== CUSTOMER MANAGEMENT MODULE ==============
void customerManagement()
{
    printHeader("CUSTOMER MANAGEMENT");

    if (customerCount == 0)
    {
        printf("No customers registered yet.\n");
        pressToContinue();
        return;
    }

    printf("==========================================================================================================\n");
    printf("ID    | Name                          | Age | Phone          | Total Spent | Member Since\n");
    printf("==========================================================================================================\n");

    for (int i = 0; i < customerCount; i++)
    {
        int yearsAgo = 2024 - (customers[i].id - 2000);

        printf("%-5d | %-30s | %-3d | %-15s | %-11.2f | %d years\n",
               customers[i].id,
               customers[i].name,
               customers[i].age,
               customers[i].phone,
               customers[i].totalSpent,
               yearsAgo);
    }

    printf("==========================================================================================================\n");
    printf("Total Customers: %d\n", customerCount);

    float totalAllSpent = 0;
    float avgSpent = 0;
    int maxSpentIndex = 0;

    for (int i = 0; i < customerCount; i++)
    {
        totalAllSpent += customers[i].totalSpent;
        if (customers[i].totalSpent > customers[maxSpentIndex].totalSpent)
        {
            maxSpentIndex = i;
        }
    }

    if (customerCount > 0)
    {
        avgSpent = totalAllSpent / customerCount;
    }

    printf("\nCustomer Statistics:\n");
    printf("Total Money Spent by All Customers: %.2f\n", totalAllSpent);
    printf("Average Spending per Customer: %.2f\n", avgSpent);
    printf("Top Spender: %s (ID: %d, Spent: %.2f)\n",
           customers[maxSpentIndex].name,
           customers[maxSpentIndex].id,
           customers[maxSpentIndex].totalSpent);

    pressToContinue();
}

// ---- CUSTOMER UTILITY FUNCTIONS ----
int getNextCustomerId()
{
    int maxId = 2000;
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id > maxId)
        {
            maxId = customers[i].id;
        }
    }
    return maxId + 1;
}

int findCustomerById(int id)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

int findCustomerByPhone(char *phone)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (strcmp(customers[i].phone, phone) == 0)
        {
            return i;
        }
    }
    return -1;
}

void viewCustomerPurchaseHistory()
{
    printHeader("MY PURCHASE HISTORY");

    int customerIndex = findCustomerById(currentUserId);
    if (customerIndex == -1)
    {
        printf("Error: Customer information not found!\n");
        pressToContinue();
        return;
    }

    printf("Customer: %s (ID: %d)\n", customers[customerIndex].name, currentUserId);
    printf("Total Spent: %.2f\n", customers[customerIndex].totalSpent);
    printf("==========================================================\n\n");

    int customerSales[saleCount];
    int customerSaleCount = 0;
    float totalCustomerSpent = 0;
    int totalItemsBought = 0;

    for (int i = 0; i < saleCount; i++)
    {
        if (sales[i].customerId == currentUserId)
        {
            customerSales[customerSaleCount] = i;
            customerSaleCount++;
            totalCustomerSpent += sales[i].totalAmount;
            totalItemsBought += sales[i].itemCount;
        }
    }

    if (customerSaleCount == 0)
    {
        printf("No purchase history found.\n");
        printf("You haven't made any purchases yet.\n");
        pressToContinue();
        return;
    }

    printf("Your Purchase History (%d bills):\n", customerSaleCount);
    printf("==================================================================\n");
    printf("Bill No | Date         | Items | Amount     | Status\n");
    printf("==================================================================\n");

    for (int i = 0; i < customerSaleCount - 1; i++)
    {
        for (int j = 0; j < customerSaleCount - i - 1; j++)
        {
            int day1, month1, year1, day2, month2, year2;
            sscanf(sales[customerSales[j]].date, "%d/%d/%d", &day1, &month1, &year1);
            sscanf(sales[customerSales[j + 1]].date, "%d/%d/%d", &day2, &month2, &year2);

            if (year1 < year2 ||
                (year1 == year2 && month1 < month2) ||
                (year1 == year2 && month1 == month2 && day1 < day2))
            {
                int temp = customerSales[j];
                customerSales[j] = customerSales[j + 1];
                customerSales[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < customerSaleCount; i++)
    {
        int saleIndex = customerSales[i];
        char status[20];

        if (sales[saleIndex].totalAmount < 100)
        {
            strcpy(status, "Small");
        }
        else if (sales[saleIndex].totalAmount < 500)
        {
            strcpy(status, "Medium");
        }
        else
        {
            strcpy(status, "Large");
        }

        printf("%-7d | %-12s | %-5d | %-10.2f | %-10s\n",
               sales[saleIndex].billNo,
               sales[saleIndex].date,
               sales[saleIndex].itemCount,
               sales[saleIndex].totalAmount,
               status);
    }

    printf("==================================================================\n");

    printf("\nYOUR PURCHASE STATISTICS:\n");
    printf("==========================\n");
    printf("Total Bills: %d\n", customerSaleCount);
    printf("Total Amount Spent: %.2f\n", totalCustomerSpent);
    printf("Total Items Purchased: %d\n", totalItemsBought);
    printf("Average Bill Amount: %.2f\n", totalCustomerSpent / customerSaleCount);
    printf("Average Items per Bill: %.1f\n", (float)totalItemsBought / customerSaleCount);

    // Removed monthly spending pattern for simplification

    printf("\nRECENT PURCHASES (Last 5):\n");
    printf("===========================\n");

    int displayCount = (customerSaleCount > 5) ? 5 : customerSaleCount;
    for (int i = 0; i < displayCount; i++)
    {
        int saleIndex = customerSales[i];
        printf("%d. Bill #%d: %.2f (%d items) on %s\n",
               i + 1,
               sales[saleIndex].billNo,
               sales[saleIndex].totalAmount,
               sales[saleIndex].itemCount,
               sales[saleIndex].date);
    }

    printf("\nYOUR LOYALTY STATUS:\n");
    printf("====================\n");

    char loyaltyLevel[20];
    float discount = 0;

    if (totalCustomerSpent >= 5000)
    {
        strcpy(loyaltyLevel, "GOLD");
        discount = 10.0;
    }
    else if (totalCustomerSpent >= 2000)
    {
        strcpy(loyaltyLevel, "SILVER");
        discount = 5.0;
    }
    else if (totalCustomerSpent >= 500)
    {
        strcpy(loyaltyLevel, "BRONZE");
        discount = 2.0;
    }
    else
    {
        strcpy(loyaltyLevel, "NEW");
        discount = 0.0;
    }

    printf("Loyalty Level: %s\n", loyaltyLevel);
    printf("Discount on Next Purchase: %.1f%%\n", discount);

    pressToContinue();
}

void viewCustomerProfile()
{
    int choice;
    int customerIndex = findCustomerById(currentUserId);

    if (customerIndex == -1)
    {
        printf("Error: Customer information not found!\n");
        pressToContinue();
        return;
    }

    do
    {
        printHeader("MY PROFILE");

        printf("PROFILE INFORMATION:\n");
        printf("====================\n");
        printf("Customer ID: %d\n", customers[customerIndex].id);
        printf("Name: %s\n", customers[customerIndex].name);
        printf("Age: %d\n", customers[customerIndex].age);
        printf("Phone: %s [Cannot be changed]\n", customers[customerIndex].phone); // Added note
        printf("Total Spent: %.2f\n\n", customers[customerIndex].totalSpent);

        printf("OPTIONS:\n");
        printf("1. Edit Profile (Name/Age Only)\n"); // Changed to exclude phone
        printf("2. Change Password\n");
        printf("3. Back to Customer Menu\n");
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            editCustomerProfile(customerIndex);
            break;
        case 2:
            changeCustomerPassword(customerIndex);
            break;
        case 3:
            return;
        default:
            printf("Invalid choice!\n");
            pressToContinue();
        }
    } while (choice != 3);
}

void editCustomerProfile(int index)
{
    printHeader("EDIT PROFILE");

    printf("Current Profile:\n");
    printf("Name: %s\n", customers[index].name);
    printf("Age: %d\n", customers[index].age);
    printf("Phone: %s [Cannot be changed]\n\n", customers[index].phone); // Added note

    printf("Enter new details (press Enter to keep current):\n\n");

    printf("Enter new Name (current: %s): ", customers[index].name);
    char newName[50];
    fgets(newName, 50, stdin);
    if (strlen(newName) > 1)
    {
        newName[strcspn(newName, "\n")] = 0;
        if (strlen(newName) > 0)
        {
            strcpy(customers[index].name, newName);
            printf("Name updated.\n");
        }
    }

    printf("\nEnter new Age (current: %d): ", customers[index].age);
    char ageStr[10];
    fgets(ageStr, 10, stdin);
    if (strlen(ageStr) > 1)
    {
        int newAge = atoi(ageStr);
        if (newAge > 0 && newAge < 120)
        {
            customers[index].age = newAge;
            printf("Age updated.\n");
        }
    }

    // Removed phone change functionality as requested

    saveCustomersToFile();
    printf("\nProfile updated successfully!\n");
    pressToContinue();
}

void changeCustomerPassword(int index)
{
    printHeader("CHANGE PASSWORD");

    char currentPass[20], newPass1[20], newPass2[20];

    printf("Enter current password: ");
    fgets(currentPass, 20, stdin);
    currentPass[strcspn(currentPass, "\n")] = 0;

    if (strcmp(currentPass, customers[index].password) != 0)
    {
        printf("Current password is incorrect!\n");
        pressToContinue();
        return;
    }

    printf("Enter new password: ");
    fgets(newPass1, 20, stdin);
    newPass1[strcspn(newPass1, "\n")] = 0;

    printf("Confirm new password: ");
    fgets(newPass2, 20, stdin);
    newPass2[strcspn(newPass2, "\n")] = 0;

    if (strcmp(newPass1, newPass2) != 0)
    {
        printf("Passwords don't match!\n");
    }
    else if (strlen(newPass1) < 4)
    {
        printf("Password must be at least 4 characters!\n");
    }
    else
    {
        strcpy(customers[index].password, newPass1);
        saveCustomersToFile();
        printf("Password changed successfully!\n");
    }

    pressToContinue();
}

// ---- CUSTOMER FILE OPERATIONS ----
void saveCustomersToFile()
{
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL)
    {
        printf("Error saving customers to file!\n");
        return;
    }

    fprintf(file, "==========================================================================================================\n");
    fprintf(file, "ID    | Name                          | Age | Phone          | Total Spent | Password\n");
    fprintf(file, "==========================================================================================================\n");

    for (int i = 0; i < customerCount; i++)
    {
        fprintf(file, "%-5d | %-30s | %-3d | %-15s | %-11.2f | %-20s\n",
                customers[i].id,
                customers[i].name,
                customers[i].age,
                customers[i].phone,
                customers[i].totalSpent,
                customers[i].password);
    }

    fprintf(file, "==========================================================================================================\n");
    fprintf(file, "Total Customers: %d\n", customerCount);

    fclose(file);
}

void loadCustomersFromFile()
{
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL)
    {
        return;
    }

    customerCount = 0;
    char line[200];

    for (int i = 0; i < 3; i++)
    {
        fgets(line, sizeof(line), file);
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "=====") != NULL || strstr(line, "Total Customers:") != NULL)
        {
            continue;
        }

        Customer c;
        if (sscanf(line, "%d | %49[^|] | %d | %14[^|] | %f | %19[^\n]",
                   &c.id, c.name, &c.age, c.phone, &c.totalSpent, c.password) >= 5)
        {
            int len = strlen(c.name);
            while (len > 0 && c.name[len - 1] == ' ')
                c.name[--len] = '\0';

            len = strlen(c.phone);
            while (len > 0 && c.phone[len - 1] == ' ')
                c.phone[--len] = '\0';

            len = strlen(c.password);
            while (len > 0 && c.password[len - 1] == ' ')
                c.password[--len] = '\0';

            if (customerCount >= customerCapacity)
            {
                customerCapacity *= 2;
                customers = (Customer *)realloc(customers, customerCapacity * sizeof(Customer));
            }
            customers[customerCount] = c;
            customerCount++;
        }
    }

    fclose(file);
}

// ============== BILLING MODULE ==============
int getNextBillNo()
{
    int maxNo = 3000;
    for (int i = 0; i < saleCount; i++)
    {
        if (sales[i].billNo > maxNo)
        {
            maxNo = sales[i].billNo;
        }
    }
    return maxNo + 1;
}

void generateBill()
{
    printHeader("GENERATE BILL");

    if (productCount == 0)
    {
        printf("No products available for billing!\n");
        pressToContinue();
        return;
    }

    int customerIndex = findCustomerById(currentUserId);
    if (customerIndex == -1)
    {
        printf("Error: Customer not found!\n");
        pressToContinue();
        return;
    }

    char customerName[50];
    strcpy(customerName, customers[customerIndex].name);
    int customerId = currentUserId;

    printf("Customer: %s (ID: %d)\n", customerName, customerId);
    printf("========================================\n\n");

    int maxItems = 100;
    int cartIds[maxItems];
    int cartQuantities[maxItems];
    float cartPrices[maxItems];
    int itemCount = 0;

    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y')
    {
        printf("\nAvailable Products:\n");
        printf("----------------------------------------------------------------\n");
        printf("ID    | Name                          | Price    | Stock\n");
        printf("----------------------------------------------------------------\n");

        for (int i = 0; i < productCount; i++)
        {
            if (products[i].quantity > 0)
            {
                printf("%-5d | %-30s | %-8.2f | %-5d\n",
                       products[i].id,
                       products[i].name,
                       products[i].price,
                       products[i].quantity);
            }
        }
        printf("----------------------------------------------------------------\n");

        int productId, quantity;
        printf("\nEnter Product ID to add: ");
        scanf("%d", &productId);
        clearInputBuffer();

        int productIndex = findProductById(productId);
        if (productIndex == -1)
        {
            printf("Product with ID %d not found!\n", productId);
            continue;
        }

        if (products[productIndex].quantity <= 0)
        {
            printf("Product '%s' is out of stock!\n", products[productIndex].name);
            continue;
        }

        printf("Product: %s\n", products[productIndex].name);
        printf("Price: %.2f\n", products[productIndex].price);
        printf("Available Stock: %d\n", products[productIndex].quantity);

        printf("Enter Quantity: ");
        scanf("%d", &quantity);
        clearInputBuffer();

        if (quantity <= 0)
        {
            printf("Invalid quantity!\n");
            continue;
        }

        if (quantity > products[productIndex].quantity)
        {
            printf("Insufficient stock! Only %d available.\n", products[productIndex].quantity);
            continue;
        }

        cartIds[itemCount] = productId;
        cartQuantities[itemCount] = quantity;
        cartPrices[itemCount] = products[productIndex].price;
        itemCount++;

        products[productIndex].quantity -= quantity;

        printf("Added %d x %s to cart\n", quantity, products[productIndex].name);

        if (itemCount < maxItems)
        {
            printf("\nAdd another item? (y/n): ");
            scanf(" %c", &addMore);
            clearInputBuffer();
        }
        else
        {
            printf("\nCart is full!\n");
            addMore = 'n';
        }
    }

    if (itemCount == 0)
    {
        printf("No items in cart. Bill cancelled.\n");
        pressToContinue();
        return;
    }

    printf("\n\n");
    printHeader("BILL RECEIPT");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char dateStr[15];
    sprintf(dateStr, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    int billNo = getNextBillNo();

    printf("Bill No: %d\n", billNo);
    printf("Date: %s\n", dateStr);
    printf("Customer: %s (ID: %d)\n", customerName, customerId);
    printf("================================================\n");
    printf("Item                       Qty   Price   Total\n");
    printf("------------------------------------------------\n");

    float subtotal = 0;

    for (int i = 0; i < itemCount; i++)
    {
        int productIndex = findProductById(cartIds[i]);
        if (productIndex != -1)
        {
            float itemTotal = cartPrices[i] * cartQuantities[i];
            subtotal += itemTotal;

            char displayName[25];
            strncpy(displayName, products[productIndex].name, 24);
            displayName[24] = '\0';

            printf("%-25s %-5d %-7.2f %-7.2f\n",
                   displayName,
                   cartQuantities[i],
                   cartPrices[i],
                   itemTotal);
        }
    }

    printf("------------------------------------------------\n");

    float tax = subtotal * 0.05;
    float total = subtotal + tax;

    printf("Subtotal: %38.2f\n", subtotal);
    printf("Tax (5%%): %38.2f\n", tax);
    printf("Total: %41.2f\n", total);
    printf("================================================\n");

    Sale newSale;
    newSale.billNo = billNo;
    strcpy(newSale.date, dateStr);
    newSale.customerId = customerId;
    strcpy(newSale.customerName, customerName);
    newSale.totalAmount = total;
    newSale.itemCount = itemCount;

    if (saleCount >= saleCapacity)
    {
        saleCapacity *= 2;
        sales = (Sale *)realloc(sales, saleCapacity * sizeof(Sale));
    }
    sales[saleCount] = newSale;
    saleCount++;

    customers[customerIndex].totalSpent += total;

    saveSaleToFile(newSale);
    saveProductsToFile();
    saveCustomersToFile();

    printf("\nThank you for your purchase!\n");
    printf("Bill saved successfully.\n");

    pressToContinue();
}

// ---- SALES FILE OPERATIONS ----
void saveSaleToFile(Sale sale)
{
    FILE *file = fopen("sales.txt", "a");
    if (file == NULL)
    {
        file = fopen("sales.txt", "w");
        if (file == NULL)
        {
            printf("Error creating sales file!\n");
            return;
        }
        fprintf(file, "==================================================================\n");
        fprintf(file, "Bill No | Date         | Customer ID | Customer Name | Items | Amount\n");
        fprintf(file, "==================================================================\n");
        fclose(file);
        file = fopen("sales.txt", "a");
    }
    else
    {
        fseek(file, 0, SEEK_END);
        if (ftell(file) == 0)
        {
            fprintf(file, "==================================================================\n");
            fprintf(file, "Bill No | Date         | Customer ID | Customer Name | Items | Amount\n");
            fprintf(file, "==================================================================\n");
        }
        fclose(file);
        file = fopen("sales.txt", "a");
    }

    fprintf(file, "%-7d | %-12s | %-11d | %-13s | %-5d | %-11.2f\n",
            sale.billNo,
            sale.date,
            sale.customerId,
            sale.customerName,
            sale.itemCount,
            sale.totalAmount);

    fclose(file);
}

void loadSalesFromFile()
{
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL)
    {
        return;
    }

    saleCount = 0;
    char line[200];

    for (int i = 0; i < 3; i++)
    {
        fgets(line, sizeof(line), file);
    }

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "=====") != NULL)
        {
            continue;
        }

        Sale s;
        if (sscanf(line, "%d | %14[^|] | %d | %49[^|] | %d | %f",
                   &s.billNo, s.date, &s.customerId, s.customerName, &s.itemCount, &s.totalAmount) >= 5)
        {
            int len = strlen(s.date);
            while (len > 0 && s.date[len - 1] == ' ')
                s.date[--len] = '\0';

            len = strlen(s.customerName);
            while (len > 0 && s.customerName[len - 1] == ' ')
                s.customerName[--len] = '\0';

            if (saleCount >= saleCapacity)
            {
                saleCapacity *= 2;
                sales = (Sale *)realloc(sales, saleCapacity * sizeof(Sale));
            }
            sales[saleCount] = s;
            saleCount++;
        }
    }

    fclose(file);
}

// ============== SIMPLIFIED REPORTING MODULE ==============
void salesReport()
{
    printHeader("SALES REPORT");

    if (saleCount == 0)
    {
        printf("No sales recorded yet.\n");
        pressToContinue();
        return;
    }

    float totalRevenue = 0;
    int totalItemsSold = 0;
    float highestSale = 0;
    int highestSaleIndex = 0;
    float lowestSale = (saleCount > 0) ? sales[0].totalAmount : 0;
    int lowestSaleIndex = 0;

    for (int i = 0; i < saleCount; i++)
    {
        totalRevenue += sales[i].totalAmount;
        totalItemsSold += sales[i].itemCount;

        if (sales[i].totalAmount > highestSale)
        {
            highestSale = sales[i].totalAmount;
            highestSaleIndex = i;
        }
        if (sales[i].totalAmount < lowestSale)
        {
            lowestSale = sales[i].totalAmount;
            lowestSaleIndex = i;
        }
    }

    printf("SALES SUMMARY:\n");
    printf("==============\n");
    printf("Total Sales (Bills): %d\n", saleCount);
    printf("Total Revenue: %.2f\n", totalRevenue);
    printf("Total Items Sold: %d\n", totalItemsSold);
    printf("Average Sale Amount: %.2f\n", saleCount > 0 ? totalRevenue / saleCount : 0);
    printf("Average Items per Sale: %.1f\n", saleCount > 0 ? (float)totalItemsSold / saleCount : 0);

    printf("\nRECORD SALES:\n");
    printf("Highest Sale: Bill #%d - %.2f (Customer: %s)\n",
           sales[highestSaleIndex].billNo,
           highestSale,
           sales[highestSaleIndex].customerName);
    printf("Lowest Sale: Bill #%d - %.2f (Customer: %s)\n",
           sales[lowestSaleIndex].billNo,
           lowestSale,
           sales[lowestSaleIndex].customerName);

    printf("\nTOP CUSTOMERS:\n");
    printf("==============\n");

    float customerSpending[100] = {0};
    char customerNames[100][50];
    int uniqueCustomerCount = 0;

    for (int i = 0; i < saleCount; i++)
    {
        int found = 0;
        for (int j = 0; j < uniqueCustomerCount; j++)
        {
            if (strcmp(customerNames[j], sales[i].customerName) == 0)
            {
                customerSpending[j] += sales[i].totalAmount;
                found = 1;
                break;
            }
        }
        if (!found && uniqueCustomerCount < 100)
        {
            strcpy(customerNames[uniqueCustomerCount], sales[i].customerName);
            customerSpending[uniqueCustomerCount] = sales[i].totalAmount;
            uniqueCustomerCount++;
        }
    }

    int displayCount = (uniqueCustomerCount < 3) ? uniqueCustomerCount : 3;
    for (int i = 0; i < displayCount; i++)
    {
        printf("%d. %s: %.2f\n", i + 1, customerNames[i], customerSpending[i]);
    }

    printf("\nRECENT SALES (Last 5):\n");
    printf("======================\n");
    printf("Bill No | Date         | Customer         | Amount\n");
    printf("--------------------------------------------------\n");

    int start = (saleCount > 5) ? saleCount - 5 : 0;
    for (int i = start; i < saleCount; i++)
    {
        printf("%-7d | %-12s | %-16s | %-11.2f\n",
               sales[i].billNo,
               sales[i].date,
               sales[i].customerName,
               sales[i].totalAmount);
    }

    pressToContinue();
}

void inventoryReport()
{
    printHeader("INVENTORY REPORT");

    if (productCount == 0)
    {
        printf("No products in inventory!\n");
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

    float totalValue = 0;
    int totalItems = 0;

    for (int i = 0; i < productCount; i++)
    {
        totalValue += products[i].price * products[i].quantity;
        totalItems += products[i].quantity;
    }

    printf("\nINVENTORY STATISTICS:\n");
    printf("====================\n");
    printf("Total Products: %d\n", productCount);
    printf("Total Items in Stock: %d\n", totalItems);
    printf("Total Inventory Value: %.2f\n", totalValue);
    printf("Average Product Price: %.2f\n", totalItems > 0 ? totalValue / totalItems : 0);

    printf("\nLOW STOCK ALERT (Quantity < 10):\n");
    printf("================================\n");

    int lowStockCount = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].quantity < 10)
        {
            printf("ID: %d, Name: %s, Stock: %d, Category: %s\n",
                   products[i].id,
                   products[i].name,
                   products[i].quantity,
                   products[i].category);
            lowStockCount++;
        }
    }

    if (lowStockCount == 0)
    {
        printf("No low stock items. All products have sufficient quantity.\n");
    }

    pressToContinue();
}

// ============== DATA MANAGEMENT ==============
void saveAllData()
{
    saveProductsToFile();
    saveCustomersToFile();
}

void loadAllData()
{
    loadProductsFromFile();
    loadCustomersFromFile();
    loadSalesFromFile();
}