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
    "Beverages", "Snacks", "Dairy", "Grocery", "Fruits",
    "Vegetables", "Meat", "Fish", "Bakery", "Toiletries"};
int categoryCount = 10;

// ============== FUNCTION PROTOTYPES ==============
void initializeSystem();
void printHeader(char *title);
void clearInputBuffer();
void pressToContinue();

void loginMenu();
void adminLogin();
void adminMenu();
void customerLoginMenu();
int customerLogin();
void customerRegister();
void customerMenu();

void productManagement();
void addProduct();
void viewProducts();
void updateProduct();
void deleteProduct();
void searchProduct();
int getNextProductId();
int findProductById(int id);
void saveProductsToFile();
void loadProductsFromFile();

void customerManagement();
int getNextCustomerId();
int findCustomerById(int id);
void saveCustomersToFile();
void loadCustomersFromFile();
void viewCustomerPurchaseHistory();
void viewCustomerProfile();

void generateBill();
int getNextBillNo();
void saveSaleToFile(Sale sale);
void loadSalesFromFile();

void salesReport();
void inventoryReport();

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

    printf("\nThank you for using Shop Management System!\n");
    return 0;
}

// ============== UTILITY FUNCTIONS ==============
void initializeSystem()
{
    products = (Product *)malloc(productCapacity * sizeof(Product));
    customers = (Customer *)malloc(customerCapacity * sizeof(Customer));
    sales = (Sale *)malloc(saleCapacity * sizeof(Sale));

    if (!products || !customers || !sales)
    {
        printf("Memory allocation failed!\n");
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
        printHeader("SHOP MANAGEMENT SYSTEM");
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
            return;
        default:
            printf("\nInvalid choice!\n");
            pressToContinue();
        }
    } while (1);
}

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
        printf("\nIncorrect password!\n");
        pressToContinue();
    }
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
        printf("5. Logout\n");
        printf("\nEnter your choice (1-5): ");
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
            currentUserId = -1;
            return;
        default:
            printf("\nInvalid choice!\n");
            pressToContinue();
        }
    } while (1);
}

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
                customerMenu();
            break;
        case 2:
            customerRegister();
            break;
        case 3:
            return;
        default:
            printf("\nInvalid choice!\n");
            pressToContinue();
        }
    } while (1);
}

int customerLogin()
{
    char phone[15], password[20];
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

    printf("\nLogin failed! Incorrect credentials.\n");
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

    printf("Enter Phone Number: ");
    fgets(newCustomer.phone, 15, stdin);
    newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;

    printf("Enter Password (min 4 chars): ");
    fgets(newCustomer.password, 20, stdin);
    newCustomer.password[strcspn(newCustomer.password, "\n")] = 0;

    customers[customerCount] = newCustomer;
    customerCount++;
    currentUserId = newCustomer.id;

    printf("\nRegistration successful! Your ID: %d\n", newCustomer.id);
    saveCustomersToFile();
    pressToContinue();
}

void customerMenu()
{
    int choice;
    do
    {
        printHeader("CUSTOMER DASHBOARD");
        printf("1. View Products\n");
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
            return;
        default:
            printf("\nInvalid choice!\n");
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
        printf("5. Search Product by Category\n");
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
            printf("\nInvalid choice!\n");
            pressToContinue();
        }
    } while (1);
}

int getNextProductId()
{
    int maxId = 1000;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id > maxId)
            maxId = products[i].id;
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
        printf("%d. %s\n", i + 1, categories[i]);
    }

    int catChoice;
    printf("\nSelect Category (1-%d): ", categoryCount);
    scanf("%d", &catChoice);
    clearInputBuffer();

    if (catChoice < 1 || catChoice > categoryCount)
    {
        strcpy(newProduct.category, "General");
    }
    else
    {
        strcpy(newProduct.category, categories[catChoice - 1]);
    }

    printf("Enter Price: ");
    scanf("%f", &newProduct.price);

    printf("Enter Quantity: ");
    scanf("%d", &newProduct.quantity);
    clearInputBuffer();

    printf("Enter Expiry Date (DD/MM/YYYY): ");
    fgets(newProduct.expiryDate, 15, stdin);
    newProduct.expiryDate[strcspn(newProduct.expiryDate, "\n")] = 0;

    products[productCount] = newProduct;
    productCount++;

    printf("\nProduct added successfully! ID: %d\n", newProduct.id);
    saveProductsToFile();
    pressToContinue();
}

void viewProducts()
{
    printHeader("ALL PRODUCTS");

    if (productCount == 0)
    {
        printf("No products available!\n");
        pressToContinue();
        return;
    }

    printf("===========================================================================\n");
    printf("ID    | Name                          | Category  | Price  | Qty  | Expiry\n");
    printf("===========================================================================\n");

    for (int i = 0; i < productCount; i++)
    {
        printf("%-5d | %-30s | %-10s | %-6.2f | %-4d | %-10s\n",
               products[i].id,
               products[i].name,
               products[i].category,
               products[i].price,
               products[i].quantity,
               products[i].expiryDate);
    }

    printf("===========================================================================\n");
    printf("Total Products: %d\n", productCount);

    float totalValue = 0;
    for (int i = 0; i < productCount; i++)
    {
        totalValue += products[i].price * products[i].quantity;
    }
    printf("Total Inventory Value: %.2f\n", totalValue);

    pressToContinue();
}

int findProductById(int id)
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
            return i;
    }
    return -1;
}

void updateProduct()
{
    printHeader("UPDATE PRODUCT");

    if (productCount == 0)
    {
        printf("No products available!\n");
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
        printf("Product not found!\n");
        pressToContinue();
        return;
    }

    printf("\nCurrent Product Details:\n");
    printf("Name: %s\n", products[index].name);
    printf("Category: %s\n", products[index].category);
    printf("Price: %.2f\n", products[index].price);
    printf("Quantity: %d\n", products[index].quantity);
    printf("Expiry Date: %s\n", products[index].expiryDate);

    printf("\nEnter new name (or press Enter to keep): ");
    char newName[50];
    fgets(newName, 50, stdin);
    if (strlen(newName) > 1)
    {
        newName[strcspn(newName, "\n")] = 0;
        if (strlen(newName) > 0)
            strcpy(products[index].name, newName);
    }

    printf("Enter new price (or 0 to keep): ");
    float newPrice;
    scanf("%f", &newPrice);
    if (newPrice > 0)
        products[index].price = newPrice;

    printf("Enter new quantity (or -1 to keep): ");
    int newQty;
    scanf("%d", &newQty);
    if (newQty >= 0)
        products[index].quantity = newQty;
    clearInputBuffer();

    printf("Enter new expiry date (or press Enter to keep): ");
    char newExpiry[15];
    fgets(newExpiry, 15, stdin);
    if (strlen(newExpiry) > 1)
    {
        newExpiry[strcspn(newExpiry, "\n")] = 0;
        if (strlen(newExpiry) > 0)
            strcpy(products[index].expiryDate, newExpiry);
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
        printf("No products available!\n");
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
        printf("Product not found!\n");
        pressToContinue();
        return;
    }

    printf("Delete '%s'? (y/n): ", products[index].name);
    char confirm;
    scanf("%c", &confirm);
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y')
    {
        for (int i = index; i < productCount - 1; i++)
        {
            products[i] = products[i + 1];
        }
        productCount--;
        printf("\nProduct deleted!\n");
        saveProductsToFile();
    }
    else
    {
        printf("\nDeletion cancelled.\n");
    }
    pressToContinue();
}

void searchProduct()
{
    printHeader("SEARCH BY CATEGORY");

    if (productCount == 0)
    {
        printf("No products available!\n");
        pressToContinue();
        return;
    }

    printf("Available Categories:\n");
    for (int i = 0; i < categoryCount; i++)
    {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    int catChoice;
    printf("\nSelect Category (1-%d): ", categoryCount);
    scanf("%d", &catChoice);
    clearInputBuffer();

    if (catChoice < 1 || catChoice > categoryCount)
    {
        printf("Invalid category!\n");
        pressToContinue();
        return;
    }

    char *selectedCategory = categories[catChoice - 1];

    printf("\nProducts in '%s' category:\n", selectedCategory);
    printf("===========================================================================\n");
    printf("ID    | Name                          | Category  | Price  | Qty  | Expiry\n");
    printf("===========================================================================\n");

    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].category, selectedCategory) == 0)
        {
            printf("%-5d | %-30s | %-10s | %-6.2f | %-4d | %-10s\n",
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
        printf("No products found in this category.\n");
    }
    printf("===========================================================================\n");
    pressToContinue();
}

void saveProductsToFile()
{
    FILE *file = fopen("products.txt", "w");
    if (file == NULL)
        return;

    fprintf(file, "ID,Name,Category,Price,Quantity,ExpiryDate\n");
    for (int i = 0; i < productCount; i++)
    {
        fprintf(file, "%d,%s,%s,%.2f,%d,%s\n",
                products[i].id,
                products[i].name,
                products[i].category,
                products[i].price,
                products[i].quantity,
                products[i].expiryDate);
    }
    fclose(file);
}

void loadProductsFromFile()
{
    FILE *file = fopen("products.txt", "r");
    if (file == NULL)
        return;

    char line[200];
    fgets(line, sizeof(line), file);

    productCount = 0;
    while (fgets(line, sizeof(line), file))
    {
        Product p;
        if (sscanf(line, "%d,%49[^,],%29[^,],%f,%d,%14[^\n]",
                   &p.id, p.name, p.category, &p.price, &p.quantity, p.expiryDate) == 6)
        {
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
        printf("No customers registered.\n");
        pressToContinue();
        return;
    }

    printf("===================================================================================\n");
    printf("ID    | Name                          | Age | Phone       | Total Spent\n");
    printf("===================================================================================\n");

    for (int i = 0; i < customerCount; i++)
    {
        printf("%-5d | %-30s | %-3d | %-12s | %-11.2f\n",
               customers[i].id,
               customers[i].name,
               customers[i].age,
               customers[i].phone,
               customers[i].totalSpent);
    }

    printf("===================================================================================\n");
    printf("Total Customers: %d\n", customerCount);
    pressToContinue();
}

int getNextCustomerId()
{
    int maxId = 2000;
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id > maxId)
            maxId = customers[i].id;
    }
    return maxId + 1;
}

int findCustomerById(int id)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id == id)
            return i;
    }
    return -1;
}

void viewCustomerPurchaseHistory()
{
    printHeader("MY PURCHASE HISTORY");

    int customerIndex = findCustomerById(currentUserId);
    if (customerIndex == -1)
    {
        printf("Customer not found!\n");
        pressToContinue();
        return;
    }

    printf("Customer: %s (Age: %d)\n", customers[customerIndex].name, customers[customerIndex].age);
    printf("Total Spent: %.2f\n", customers[customerIndex].totalSpent);
    printf("=================================================================\n");
    printf("Bill No | Date         | Items | Amount     \n");
    printf("=================================================================\n");

    int hasPurchases = 0;
    for (int i = 0; i < saleCount; i++)
    {
        if (sales[i].customerId == currentUserId)
        {
            printf("%-7d | %-12s | %-5d | %-11.2f\n",
                   sales[i].billNo, sales[i].date,
                   sales[i].itemCount, sales[i].totalAmount);
            hasPurchases = 1;
        }
    }

    if (!hasPurchases)
        printf("No purchases found.\n");
    printf("=================================================================\n");
    pressToContinue();
}

void viewCustomerProfile()
{
    int customerIndex = findCustomerById(currentUserId);
    if (customerIndex == -1)
    {
        printf("Customer not found!\n");
        pressToContinue();
        return;
    }

    printHeader("MY PROFILE");
    printf("=================================\n");
    printf("Customer ID: %d\n", customers[customerIndex].id);
    printf("Name: %s\n", customers[customerIndex].name);
    printf("Age: %d\n", customers[customerIndex].age);
    printf("Phone: %s\n", customers[customerIndex].phone);
    printf("Total Spent: %.2f\n", customers[customerIndex].totalSpent);
    printf("=================================\n");

    printf("\n1. Edit Profile\n2. Back\nChoice: ");
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1)
    {
        printf("\nEnter new name (or press Enter to keep): ");
        char newName[50];
        fgets(newName, 50, stdin);
        if (strlen(newName) > 1)
        {
            newName[strcspn(newName, "\n")] = 0;
            if (strlen(newName) > 0)
                strcpy(customers[customerIndex].name, newName);
        }

        printf("Enter new age (or 0 to keep): ");
        int newAge;
        scanf("%d", &newAge);
        if (newAge > 0)
            customers[customerIndex].age = newAge;
        clearInputBuffer();

        printf("Enter new phone (or press Enter to keep): ");
        char newPhone[15];
        fgets(newPhone, 15, stdin);
        if (strlen(newPhone) > 1)
        {
            newPhone[strcspn(newPhone, "\n")] = 0;
            if (strlen(newPhone) > 0)
                strcpy(customers[customerIndex].phone, newPhone);
        }

        printf("Enter new password (or press Enter to keep): ");
        char newPass[20];
        fgets(newPass, 20, stdin);
        if (strlen(newPass) > 1)
        {
            newPass[strcspn(newPass, "\n")] = 0;
            if (strlen(newPass) >= 4)
                strcpy(customers[customerIndex].password, newPass);
        }

        saveCustomersToFile();
        printf("\nProfile updated!\n");
    }
    pressToContinue();
}

void saveCustomersToFile()
{
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL)
        return;

    fprintf(file, "ID,Name,Age,Phone,TotalSpent,Password\n");
    for (int i = 0; i < customerCount; i++)
    {
        fprintf(file, "%d,%s,%d,%s,%.2f,%s\n",
                customers[i].id,
                customers[i].name,
                customers[i].age,
                customers[i].phone,
                customers[i].totalSpent,
                customers[i].password);
    }
    fclose(file);
}

void loadCustomersFromFile()
{
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL)
        return;

    char line[200];
    fgets(line, sizeof(line), file);

    customerCount = 0;
    while (fgets(line, sizeof(line), file))
    {
        Customer c;
        if (sscanf(line, "%d,%49[^,],%d,%14[^,],%f,%19[^\n]",
                   &c.id, c.name, &c.age, c.phone, &c.totalSpent, c.password) == 6)
        {
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
            maxNo = sales[i].billNo;
    }
    return maxNo + 1;
}

void generateBill()
{
    printHeader("GENERATE BILL");

    if (productCount == 0)
    {
        printf("No products available!\n");
        pressToContinue();
        return;
    }

    int customerIndex = findCustomerById(currentUserId);
    if (customerIndex == -1)
    {
        printf("Customer not found!\n");
        pressToContinue();
        return;
    }

    int cartIds[100], cartQuantities[100];
    float cartPrices[100];
    int itemCount = 0;
    char addMore = 'y';

    while (addMore == 'y' && itemCount < 100)
    {
        printf("\nAvailable Products:\n");
        printf("===========================================================================\n");
        printf("ID    | Name                          | Category  | Price  | Qty  | Expiry\n");
        printf("===========================================================================\n");

        for (int i = 0; i < productCount; i++)
        {
            if (products[i].quantity > 0)
            {
                printf("%-5d | %-30s | %-10s | %-6.2f | %-4d | %-10s\n",
                       products[i].id, products[i].name,
                       products[i].category, products[i].price,
                       products[i].quantity, products[i].expiryDate);
            }
        }
        printf("===========================================================================\n");

        int productId, quantity;
        printf("\nEnter Product ID: ");
        scanf("%d", &productId);
        clearInputBuffer();

        int productIndex = findProductById(productId);
        if (productIndex == -1)
        {
            printf("Product not found!\n");
            continue;
        }

        if (products[productIndex].quantity <= 0)
        {
            printf("Out of stock!\n");
            continue;
        }

        printf("Enter Quantity: ");
        scanf("%d", &quantity);
        clearInputBuffer();

        if (quantity <= 0 || quantity > products[productIndex].quantity)
        {
            printf("Invalid quantity!\n");
            continue;
        }

        cartIds[itemCount] = productId;
        cartQuantities[itemCount] = quantity;
        cartPrices[itemCount] = products[productIndex].price;
        itemCount++;

        products[productIndex].quantity -= quantity;

        printf("\nAdd another item? (y/n): ");
        scanf(" %c", &addMore);
        clearInputBuffer();
    }

    if (itemCount == 0)
    {
        printf("No items in cart!\n");
        pressToContinue();
        return;
    }

    printHeader("BILL RECEIPT");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char dateStr[15];
    sprintf(dateStr, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    int billNo = getNextBillNo();
    printf("Bill No: %d\n", billNo);
    printf("Date: %s\n", dateStr);
    printf("Customer: %s (Age: %d)\n", customers[customerIndex].name, customers[customerIndex].age);
    printf("================================================\n");
    printf("Item                       Qty   Price   Total\n");
    printf("------------------------------------------------\n");

    float subtotal = 0;
    for (int i = 0; i < itemCount; i++)
    {
        int productIndex = findProductById(cartIds[i]);
        float itemTotal = cartPrices[i] * cartQuantities[i];
        subtotal += itemTotal;

        char displayName[25];
        strncpy(displayName, products[productIndex].name, 24);
        displayName[24] = '\0';

        printf("%-25s %-5d %-7.2f %-7.2f\n",
               displayName, cartQuantities[i], cartPrices[i], itemTotal);
    }

    float tax = subtotal * 0.05;
    float total = subtotal + tax;

    printf("------------------------------------------------\n");
    printf("Subtotal: %38.2f\n", subtotal);
    printf("Tax (5%%): %38.2f\n", tax);
    printf("Total: %41.2f\n", total);
    printf("================================================\n");

    Sale newSale;
    newSale.billNo = billNo;
    strcpy(newSale.date, dateStr);
    newSale.customerId = currentUserId;
    strcpy(newSale.customerName, customers[customerIndex].name);
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
    pressToContinue();
}

void saveSaleToFile(Sale sale)
{
    FILE *file = fopen("sales.txt", "a");
    if (file == NULL)
        return;

    fprintf(file, "%d,%s,%d,%s,%d,%.2f\n",
            sale.billNo, sale.date, sale.customerId,
            sale.customerName, sale.itemCount, sale.totalAmount);
    fclose(file);
}

void loadSalesFromFile()
{
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL)
        return;

    saleCount = 0;
    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        Sale s;
        if (sscanf(line, "%d,%14[^,],%d,%49[^,],%d,%f",
                   &s.billNo, s.date, &s.customerId,
                   s.customerName, &s.itemCount, &s.totalAmount) == 6)
        {
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

// ============== REPORTING MODULE ==============
void salesReport()
{
    printHeader("SALES REPORT");

    if (saleCount == 0)
    {
        printf("No sales recorded.\n");
        pressToContinue();
        return;
    }

    float totalRevenue = 0;
    int totalItems = 0;

    for (int i = 0; i < saleCount; i++)
    {
        totalRevenue += sales[i].totalAmount;
        totalItems += sales[i].itemCount;
    }

    printf("==================================================================\n");
    printf("Total Sales: %d\n", saleCount);
    printf("Total Revenue: %.2f\n", totalRevenue);
    printf("Total Items Sold: %d\n", totalItems);
    printf("Average Sale: %.2f\n", saleCount > 0 ? totalRevenue / saleCount : 0);
    printf("==================================================================\n");

    printf("\nRecent Sales (Last 5):\n");
    printf("==================================================================\n");
    printf("Bill No | Date         | Customer         | Amount\n");
    printf("==================================================================\n");

    int start = saleCount > 5 ? saleCount - 5 : 0;
    for (int i = start; i < saleCount; i++)
    {
        printf("%-7d | %-12s | %-16s | %-11.2f\n",
               sales[i].billNo, sales[i].date,
               sales[i].customerName, sales[i].totalAmount);
    }
    printf("==================================================================\n");

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

    printf("===========================================================================\n");
    printf("ID    | Name                          | Category  | Price  | Qty  | Expiry\n");
    printf("===========================================================================\n");

    float totalValue = 0;
    int totalItems = 0;
    int lowStockCount = 0;

    for (int i = 0; i < productCount; i++)
    {
        printf("%-5d | %-30s | %-10s | %-6.2f | %-4d | %-10s\n",
               products[i].id,
               products[i].name,
               products[i].category,
               products[i].price,
               products[i].quantity,
               products[i].expiryDate);

        totalValue += products[i].price * products[i].quantity;
        totalItems += products[i].quantity;
        if (products[i].quantity < 10)
            lowStockCount++;
    }

    printf("===========================================================================\n");
    printf("\nINVENTORY STATISTICS:\n");
    printf("====================\n");
    printf("Total Products: %d\n", productCount);
    printf("Total Items in Stock: %d\n", totalItems);
    printf("Total Inventory Value: %.2f\n", totalValue);
    printf("Low Stock Items (Qty < 10): %d\n", lowStockCount);

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