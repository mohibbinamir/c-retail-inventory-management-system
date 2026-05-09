#include "Inventory.h"

void inventory_menu() {
    InventoryItem* inventory = load_inventory();
    int choice;

    do {
        printf("\n=== INVENTORY MANAGEMENT ===\n");
        printf("1. Add Item\n2. Update Item\n3. Delete Item\n");
        printf("4. View Inventory\n5. Stock Report\n0. Exit\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: add_inventory_item(&inventory); break;
            case 2: update_inventory_item(inventory); break;
            case 3: delete_inventory_item(&inventory); break;
            case 4: view_inventory(inventory); break;
            case 5: generate_stock_report(inventory); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }

        if(choice != 0) {
            save_inventory(inventory);
            pause_screen();
        }
    } while(choice != 0);

    free_inventory(inventory);
}

InventoryItem* load_inventory() {
    FILE* file = fopen(INVENTORY_FILE, "r");
    if(!file) return NULL;

    InventoryItem* head = NULL;
    InventoryItem* tail = NULL;
    char line[256];

    while(fgets(line, sizeof(line), file)) {
        InventoryItem* item = malloc(sizeof(InventoryItem));
        if(!item) break;

        if(sscanf(line, "%d,%d,%d,%d,%49[^\n]", &item->id, &item->product_id, &item->quantity, &item->threshold, item->location) == 5) {
            item->next = NULL;

            if(!head) head = item;
            else tail->next = item;
            tail = item;
        } else {
            free(item);
        }
    }

    fclose(file);
    return head;
}

void save_inventory(InventoryItem* head) {
    FILE* file = fopen(INVENTORY_FILE, "w");
    if(!file) return;

    InventoryItem* current = head;
    while(current) {
        fprintf(file, "%d,%d,%d,%d,%s\n", current->id, current->product_id, current->quantity, current->threshold, current->location);
        current = current->next;
    }

    fclose(file);
}

void free_inventory(InventoryItem* head) {
    InventoryItem* current = head;
    while(current) {
        InventoryItem* temp = current;
        current = current->next;
        free(temp);
    }
}

void add_inventory_item(InventoryItem** head) {
    InventoryItem* item = malloc(sizeof(InventoryItem));
    if(!item) return;

    item->id = 1;
    InventoryItem* current = *head;
    while(current) {
        if(current->id >= item->id) item->id = current->id + 1;
        current = current->next;
    }

    item->product_id = get_valid_int("Product ID: ");
    item->quantity = get_valid_int("Initial Quantity: ");
    item->threshold = get_valid_int("Low Stock Threshold: ");
    get_string_input("Location: ", item->location, MAX_NAME_LEN);

    item->next = *head;
    *head = item;
}

void update_inventory_item(InventoryItem* head) {
    if(!head) return;

    int id = get_valid_int("Enter Inventory ID: ");
    InventoryItem* current = head;

    while(current) {
        if(current->id == id) {
            char input[MAX_INPUT];

            printf("New Quantity (current %d): ", current->quantity);
            fgets(input, MAX_INPUT, stdin);
            if(strlen(input) > 1) current->quantity = get_valid_int("");

            printf("New Threshold (current %d): ", current->threshold);
            fgets(input, MAX_INPUT, stdin);
            if(strlen(input) > 1) current->threshold = get_valid_int("");

            printf("New Location (current %s): ", current->location);
            fgets(input, MAX_INPUT, stdin);
            if(strlen(input) > 1) {
                input[strcspn(input, "\n")] = '\0';
                strncpy(current->location, input, MAX_NAME_LEN - 1);
                current->location[MAX_NAME_LEN - 1] = '\0';
            }

            return;
        }
        current = current->next;
    }

    printf("Item not found\n");
}

void delete_inventory_item(InventoryItem** head) {
    if (!head || !*head) return;

    int id = get_valid_int("Enter Inventory ID to delete: ");
    InventoryItem* current = *head;
    InventoryItem* prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) prev->next = current->next;
            else *head = current->next;

            free(current);
            printf("Item deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Item with ID %d not found.\n", id);
}

void view_inventory(InventoryItem* head) {
    printf("\nID\tProduct\tQty\tThreshold\tLocation\n");
    printf("------------------------------------------------\n");

    InventoryItem* current = head;
    while(current) {
        printf("%d\t%d\t%d\t%d\t\t%s\n", current->id, current->product_id, current->quantity, current->threshold, current->location);
        current = current->next;
    }
}

void generate_stock_report(InventoryItem* head) {
    printf("\n=== LOW STOCK REPORT ===\n");
    printf("ID\tProduct\tQty\tThreshold\n");
    printf("--------------------------------\n");

    InventoryItem* current = head;
    while(current) {
        if(current->quantity <= current->threshold) {
            printf("%d\t%d\t%d\t%d\n", current->id, current->product_id, current->quantity, current->threshold);
        }
        current = current->next;
    }
}
