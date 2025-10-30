#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_PRODUCT_ID 1
#define MAX_PRODUCT_ID 10000

#define MAX_NAME_LENGTH 50

#define MIN_PRICE 0
#define MAX_PRICE 100000

#define MIN_QUANTITY 0
#define MAX_QUANTITY 1000000

typedef struct Product
{
    int id;
    char name[MAX_NAME_LENGTH + 1];
    float price;
    int quantity;
} Product;

int getStringLength(const char *stringInput)
{
    int length = 0;
    while (stringInput[length] != '\0')
    {
        length++;
    }
    return length;
}

int upperToLower(int character)
{
    if (character >= 'A' && character <= 'Z')
    {
        return character + ('a' - 'A');
    }
    return character;
}

int findCharPosition(const char *str, char ch)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ch)
            return i;
        i++;
    }
    return i;
}

bool isUniqueID(Product *products, int productCount, int id)
{
    for (int currentIndex = 0; currentIndex < productCount; currentIndex++)
    {
        if (products[currentIndex].id == id)
        {
            return false;
        }
    }
    return true;
}

bool containsSubstring(const char *originalString, const char *substring)
{
    int mainLength = getStringLength(originalString);
    int subLength = getStringLength(substring);

    if (subLength == 0 || subLength > mainLength)
    {
        return false;
    }

    for (int startIndex = 0; startIndex <= mainLength - subLength; startIndex++)
    {
        int subIndex;
        for (subIndex = 0; subIndex < subLength; subIndex++)
        {
            if (upperToLower(originalString[startIndex + subIndex]) != upperToLower(substring[subIndex]))
            {
                break;
            }
        }
        if (subIndex == subLength)
        {
            return true;
        }
    }
    return false;
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void displayProduct(Product product)
{
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           product.id, product.name, product.price, product.quantity);
}

bool isValidPrice(float price)
{
    return (price >= MIN_PRICE && price <= MAX_PRICE);
}

bool isValidQuantity(int quantity)
{
    return (quantity >= MIN_QUANTITY && quantity <= MAX_QUANTITY);
}

bool isValidName(const char *name)
{
    if (name == NULL)
        return false;

    while (*name == ' ')
        name++;

    if (*name == '\0')
        return false;

    if (getStringLength(name) > MAX_NAME_LENGTH)
    {
        return false;
    }

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalnum(name[i]) && name[i] != ' ' && name[i] != '-' && name[i] != '_')
        {
            return false;
        }
    }

    return true;
}

bool isValidProductID(int id, Product *products, int productCount)
{
    if (id < MIN_PRODUCT_ID || id > MAX_PRODUCT_ID)
    {
        printf("ID should be between %d and %d\n", MIN_PRODUCT_ID, MAX_PRODUCT_ID);
        return false;
    }
    if (!isUniqueID(products, productCount, id))
    {
        printf("Product ID already exists. Try again.\n");
        return false;
    }
    return true;
}

void inputProductID(Product *product, Product *products, int productCount)
{
    int id;
    do
    {
        printf("Product ID: ");
        if (scanf("%d", &id) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (isValidProductID(id, products, productCount))
        {
            product->id = id;
            clearInputBuffer();
            break;
        }
        clearInputBuffer();
    } while (1);
}

void inputProductName(Product *product)
{
    do
    {
        printf("Product Name: ");
        fgets(product->name, sizeof(product->name), stdin);
        product->name[findCharPosition(product->name, '\n')] = '\0';

        if (!isValidName(product->name))
        {
            printf("Invalid name. Only alphanumeric characters, spaces, hyphens, and underscores are allowed.\n");
            continue;
        }
        break;
    } while (1);
}

void inputProductPrice(Product *product)
{
    printf("Product Price: ");
    while (scanf("%f", &product->price) != 1 || !isValidPrice(product->price))
    {
        printf("Invalid input. Enter a valid price (%d-%d): ", MIN_PRICE, MAX_PRICE);
        clearInputBuffer();
    }
    clearInputBuffer();
}

void inputProductQuantity(Product *product)
{
    printf("Product Quantity: ");
    while (scanf("%d", &product->quantity) != 1 || !isValidQuantity(product->quantity))
    {
        printf("Invalid input. Enter a valid quantity (%d-%d): ", MIN_QUANTITY, MAX_QUANTITY);
        clearInputBuffer();
    }
    clearInputBuffer();
}

void getProductDetails(Product *product, Product *products, int productCount)
{
    inputProductID(product, products, productCount);
    inputProductName(product);
    inputProductPrice(product);
    inputProductQuantity(product);
}


void addInitialProducts(int initialCount, Product *products, int *productCount)
{
    for (int currentIndex = 0; currentIndex < initialCount; currentIndex++)
    {
        printf("\nEnter details for product %d:\n", currentIndex + 1);
        getProductDetails(&products[currentIndex], products, currentIndex);
        (*productCount)++;
    }
}

void addNewProduct(int *totalProducts, Product **products, int *productCount)
{
    Product *temp = realloc(*products, (*totalProducts + 1) * sizeof(Product));
    if (temp == NULL)
    {
        printf("Memory reallocation failed!\n");
        exit(1);
    }
    *products = temp;

    printf("\nEnter new product details:\n");
    getProductDetails(&(*products)[*productCount], *products, *productCount);

    (*productCount)++;
    (*totalProducts)++;
    printf("Product added successfully!\n");
}

void displayAllProducts(int productCount, Product *products)
{
    if (productCount == 0)
    {
        printf("No products available.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < productCount; i++)
    {
        displayProduct(products[i]);
    }
}

void updateProductQuantity(int productCount, Product *products)
{
    int id;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            printf("Enter new Quantity: ");
            while (scanf("%d", &products[i].quantity) != 1 || !isValidQuantity(products[i].quantity))
            {
                printf("Invalid input. Enter a valid quantity (%d-%d): ", MIN_QUANTITY, MAX_QUANTITY);
                clearInputBuffer();
            }
            clearInputBuffer();
            printf("Quantity updated successfully!\n");
            return;
        }
    }
    printf("No product found with ID %d\n", id);
}

Product *searchProductByID(int productCount, Product *products, int id)
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
        {
            return &products[i];
        }
    }
    return NULL;
}

void searchAndDisplayByID(int productCount, Product *products)
{
    int id;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();

    Product *foundProduct = searchProductByID(productCount, products, id);
    if (foundProduct != NULL)
    {
        printf("Product Found: ");
        displayProduct(*foundProduct);
    }
    else
    {
        printf("No product found with ID %d\n", id);
    }
}

void searchAndDisplayByName(int productCount, Product *products)
{
    char searchName[100];
    printf("Enter name to search (partial allowed): ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[findCharPosition(searchName, '\n')] = '\0';

    bool found = false;
    printf("Products Found:\n");
    for (int i = 0; i < productCount; i++)
    {
        if (containsSubstring(products[i].name, searchName))
        {
            displayProduct(products[i]);
            found = true;
        }
    }

    if (!found)
    {
        printf("No product found matching \"%s\"\n", searchName);
    }
}

void searchAndDisplayByPriceRange(int productCount, Product *products)
{
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    clearInputBuffer();

    if (minPrice < 0 || maxPrice < 0 || minPrice > maxPrice)
    {
        printf("Invalid price range.\n");
        return;
    }

    bool found = false;
    printf("Products in price range:\n");
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].price >= minPrice && products[i].price <= maxPrice)
        {
            displayProduct(products[i]);
            found = true;
        }
    }

    if (!found)
    {
        printf("No products found in price range %.2f - %.2f\n", minPrice, maxPrice);
    }
}

void deleteProduct(int *totalProducts, Product **products, int *productCount)
{
    int id;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < *productCount; i++)
    {
        if ((*products)[i].id == id)
        {
            for (int j = i; j < *productCount - 1; j++)
            {
                (*products)[j] = (*products)[j + 1];
            }
            (*productCount)--;

            if (*totalProducts > *productCount)
            {
                Product *temp = realloc(*products, (*productCount) * sizeof(Product));
                if (temp != NULL || *productCount == 0)
                {
                    *products = temp;
                    *totalProducts = *productCount;
                }
            }

            printf("Product deleted successfully!\n");
            return;
        }
    }
    printf("No product found with ID %d\n", id);
}

void showMenu(Product **products, int *totalProducts, int *productCount)
{
    int choice;

    do
    {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("==================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addNewProduct(totalProducts, products, productCount);
            break;
        case 2:
            displayAllProducts(*productCount, *products);
            break;
        case 3:
            updateProductQuantity(*productCount, *products);
            break;
        case 4:
            searchAndDisplayByID(*productCount, *products);
            break;
        case 5:
            searchAndDisplayByName(*productCount, *products);
            break;
        case 6:
            searchAndDisplayByPriceRange(*productCount, *products);
            break;
        case 7:
            deleteProduct(totalProducts, products, productCount);
            break;
        case 8:
            printf("Memory released successfully. Exiting program...\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    } while (choice != 8);
}

int main()
{
    int totalProducts;
    int productCount = 0;

    do
    {
        printf("Enter initial number of products: ");
        if (scanf("%d", &totalProducts) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        break;
    } while (1);
    clearInputBuffer();

    Product *products = (Product *)calloc(totalProducts, sizeof(Product));
    if (products == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    addInitialProducts(totalProducts, products, &productCount);

    showMenu(&products, &totalProducts, &productCount);

    free(products);
    return 0;
}
