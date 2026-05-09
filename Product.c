#include "Product.h"

void product_main_menu() {
    Product* products = load_products(PRODUCTS_FILE);
    int choice;

    do {
        printf("\n=== PRODUCT MANAGEMENT ===\n");
        printf("1. Add Product\n2. Update Product\n3. Delete Product\n");
        printf("4. View All Products\n0. Return to Main Menu\n");
        printf("Enter your choice: ");
        choice = get_valid_int("");

        switch (choice) {
            case 1: add_product(&products); break;
            case 2: update_product(products); break;
            case 3: delete_product(&products); break;
            case 4: view_products(products); break;
            case 0: break;
            default: printf("Invalid choice. Please try again.\n");
        }

        if (choice != 0) {
            save_products(PRODUCTS_FILE, products);
            pause_screen();
        }
    } while (choice != 0);

    free_product_list(products);
}

void add_product(Product** head) {
    Product* new_product = malloc(sizeof(Product));
    if (!new_product) return;

    new_product->id = generate_product_id(*head);
    printf("\n--- ADD NEW PRODUCT ---\n");
    printf("Auto-generated Product ID: %d\n", new_product->id);

    get_string_input("Product Name: ", new_product->name, MAX_NAME_LEN);
    new_product->price = get_valid_float("Unit Price: ");
    new_product->quantity = get_valid_int("Initial Quantity: ");
    new_product->category_id = get_valid_int("Category ID (0 if none): ");
    new_product->supplier_id = get_valid_int("Supplier ID (0 if none): ");

    new_product->next = *head;
    *head = new_product;

    printf("\nProduct added successfully!\n");
}

void update_product(Product* head) {
    if (!head) return;

    int product_id = get_valid_int("Enter Product ID to update: ");
    Product* current = head;

    while (current) {
        if (current->id == product_id) {
            printf("\nCurrent Details:\n");
            display_product(current);

            char new_name[MAX_NAME_LEN];
            get_string_input("New Name: ", new_name, MAX_NAME_LEN);
            if (strlen(new_name) > 0) {
                strncpy(current->name, new_name, MAX_NAME_LEN - 1);
                current->name[MAX_NAME_LEN - 1] = '\0';
            }

            char price_input[MAX_INPUT];
            printf("New Price (current: %.2f): ", current->price);
            fgets(price_input, MAX_INPUT, stdin);
            if (strlen(price_input) > 1) {
                current->price = get_valid_float("");
            }

            char qty_input[MAX_INPUT];
            printf("New Quantity (current: %d): ", current->quantity);
            fgets(qty_input, MAX_INPUT, stdin);
            if (strlen(qty_input) > 1) {
                current->quantity = get_valid_int("");
            }

            char cat_input[MAX_INPUT];
            printf("New Category ID (current: %d): ", current->category_id);
            fgets(cat_input, MAX_INPUT, stdin);
            if (strlen(cat_input) > 1) {
                current->category_id = get_valid_int("");
            }

            printf("\nProduct updated successfully!\n");
            return;
        }
        current = current->next;
    }

    printf("Product with ID %d not found.\n", product_id);
}

Product* load_products(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    Product* head = NULL;
    Product* tail = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        Product* new_product = malloc(sizeof(Product));
        if (!new_product) {
            fclose(file);
            return head;
        }

        if (sscanf(line, "%d,%49[^,],%f,%d,%d,%d", &new_product->id, new_product->name,
                   &new_product->price, &new_product->quantity,
                   &new_product->category_id, &new_product->supplier_id) == 6) {
            new_product->next = NULL;

            if (!head) head = new_product;
            else tail->next = new_product;
            tail = new_product;
        } else {
            free(new_product);
        }
    }

    fclose(file);
    return head;
}

void save_products(const char* filename, Product* head) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    Product* current = head;
    while (current) {
        fprintf(file, "%d,%s,%.2f,%d,%d,%d\n", current->id, current->name, current->price,
                current->quantity, current->category_id, current->supplier_id);
        current = current->next;
    }

    fclose(file);
}

int generate_product_id(Product* head) {
    int max_id = 0;
    Product* current = head;
    while (current) {
        if (current->id > max_id) max_id = current->id;
        current = current->next;
    }
    return max_id + 1;
}

void display_product(Product* p) {
    printf("ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n", p->id, p->name, p->price, p->quantity);
    printf("Category ID: %d\nSupplier ID: %d\n", p->category_id, p->supplier_id);
}

void free_product_list(Product* head) {
    Product* current = head;
    while (current) {
        Product* temp = current;
        current = current->next;
        free(temp);
    }
}

void delete_product(Product** head) {
    if (!head || !*head) return;

    int id = get_valid_int("Enter Product ID to delete: ");
    Product* current = *head;
    Product* prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) prev->next = current->next;
            else *head = current->next;

            free(current);
            printf("Product deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Product with ID %d not found.\n", id);
}

void view_products(Product* head) {
    if (!head) {
        printf("No products to display.\n");
        return;
    }

    printf("\nID\tName\t\tPrice\tQty\tCategory\tSupplier\n");
    printf("-------------------------------------------------------------\n");

    Product* current = head;
    while (current) {
        printf("%d\t%-10s\t%.2f\t%d\t%d\t\t%d\n", current->id, current->name,
               current->price, current->quantity,
               current->category_id, current->supplier_id);
        current = current->next;
    }
}

int product_exists(Product* head, int product_id) {
    while (head) {
        if (head->id == product_id) return 1;
        head = head->next;
    }
    return 0;
}
