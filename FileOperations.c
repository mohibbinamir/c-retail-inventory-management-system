#include "FileOperations.h"
#include <time.h>

void backup_all_data() {
    time_t now = time(NULL);
    char backup_file[256];
    snprintf(backup_file, sizeof(backup_file), "%sbackup_%ld.txt", BACKUP_DIR, now);

    FILE* backup = fopen(backup_file, "w");
    if (!backup) return;

    const char* files[] = {
        PRODUCTS_FILE, INVENTORY_FILE, CATEGORIES_FILE, SUPPLIERS_FILE,
        USERS_FILE, TRANSACTIONS_FILE, NULL
    };

    for (int i = 0; files[i]; i++) {
        FILE* src = fopen(files[i], "r");
        if (!src) continue;
        fprintf(backup, "=== %s ===\n", files[i]);
        char line[256];
        while (fgets(line, sizeof(line), src)) {
            fputs(line, backup);
        }
        fclose(src);
    }

    fclose(backup);
}

void restore_backup(const char* backup_path) {
    FILE* backup = fopen(backup_path, "r");
    if (!backup) return;

    char current_file[256] = {0};
    FILE* current_target = NULL;
    char line[256];

    while (fgets(line, sizeof(line), backup)) {
        if (strstr(line, "===")) {
            if (current_target) fclose(current_target);
            char* start = strstr(line, "===") + 3;
            char* end = strstr(start, " ===");
            if (end) {
                *end = '\0';
                strcpy(current_file, start);
                current_target = fopen(current_file, "w");
            }
        } else if (current_target) {
            fputs(line, current_target);
        }
    }

    if (current_target) fclose(current_target);
    fclose(backup);
}

void validate_data_integrity() {
    Product* products = load_products(PRODUCTS_FILE);
    InventoryItem* inventory = load_inventory();
    Category* categories = load_categories();
    Supplier* suppliers = load_suppliers();

    int errors = 0;

    InventoryItem* inv = inventory;
    while (inv) {
        if (!product_exists(products, inv->product_id)) {
            printf("Missing product: %d\n", inv->product_id);
            errors++;
        }
        inv = inv->next;
    }

    Product* prod = products;
    while (prod) {
        if (prod->supplier_id > 0 && !supplier_exists(suppliers, prod->supplier_id)) {
            printf("Missing supplier: %d\n", prod->supplier_id);
            errors++;
        }
        if (prod->category_id > 0 && !category_exists(categories, prod->category_id)) {
            printf("Missing category: %d\n", prod->category_id);
            errors++;
        }
        prod = prod->next;
    }

    printf("Validation found %d issues\n", errors);

    free_product_list(products);
    free_inventory(inventory);
    free_categories(categories);
    free_suppliers(suppliers);
}

void generate_system_report() {
    FILE* report = fopen(REPORT_FILE, "w");
    if (!report) return;

    Product* products = load_products(PRODUCTS_FILE);
    InventoryItem* inventory = load_inventory();
    User* users = load_users();
    Transaction* transactions = load_transactions();

    int product_count = 0, low_stock_count = 0, user_count = 0, pending_txn_count = 0;

    for (Product* p = products; p; p = p->next) product_count++;
    for (InventoryItem* i = inventory; i; i = i->next)
        if (i->quantity <= i->threshold) low_stock_count++;
    for (User* u = users; u; u = u->next) user_count++;
    for (Transaction* t = transactions; t; t = t->next)
        if (strcmp(t->status, "pending") == 0) pending_txn_count++;

    fprintf(report,
        "Products: %d\nLow Stock: %d\nUsers: %d\nPending Transactions: %d\n",
        product_count, low_stock_count, user_count, pending_txn_count
    );

    fclose(report);
    free_product_list(products);
    free_inventory(inventory);
    free_users(users);
    free_transactions(transactions);
}

void export_to_csv(const char* filename) {
    FILE* csv = fopen(filename, "w");
    if (!csv) return;

    Product* products = load_products(PRODUCTS_FILE);
    fprintf(csv, "ID,Name,Price,Quantity,CategoryID,SupplierID\n");
    while (products) {
        fprintf(csv, "%d,%s,%.2f,%d,%d,%d\n", products->id, products->name, products->price,
                products->quantity, products->category_id, products->supplier_id);
        products = products->next;
    }
    fclose(csv);
}

void file_operations_menu() {
    int choice;
    do {
        printf("\nFile Operations:\n");
        printf("1. Backup Data\n2. Restore Data\n3. Validate Data\n");
        printf("4. Generate Report\n5. Export Data\n0. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        switch (choice) {
            case 1: backup_all_data(); break;
            case 2: {
                char path[256];
                printf("Backup file path: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = '\0';
                restore_backup(path);
                break;
            }
            case 3: validate_data_integrity(); break;
            case 4: generate_system_report(); break;
            case 5: export_to_csv("Files/export.csv"); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 0);
}

int supplier_exists(Supplier* suppliers, int supplier_id) {
    while (suppliers) {
        if (suppliers->id == supplier_id) return 1;
        suppliers = suppliers->next;
    }
    return 0;
}

int category_exists(Category* categories, int category_id) {
    while (categories) {
        if (categories->id == category_id) return 1;
        categories = categories->next;
    }
    return 0;
}
