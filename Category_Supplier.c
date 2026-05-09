#include "Category_Supplier.h"

void category_supplier_menu() {
    int choice;
    do {
        printf("\n=== CATEGORY/SUPPLIER MANAGEMENT ===\n");
        printf("1. Manage Categories\n2. Manage Suppliers\n0. Exit\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: manage_categories(); break;
            case 2: manage_suppliers(); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }
    } while(choice != 0);
}

void manage_categories() {
    Category* categories = load_categories();
    int choice;

    do {
        printf("\n=== CATEGORIES ===\n");
        printf("1. Add\n2. View\n3. Update\n4. Delete\n0. Back\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: add_category(&categories); break;
            case 2: view_categories(categories); break;
            case 3: update_category(categories); break;
            case 4: delete_category(&categories); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }

        if(choice != 0) save_categories(categories);
    } while(choice != 0);

    free_categories(categories);
}

Category* load_categories() {
    FILE* file = fopen(CATEGORIES_FILE, "r");
    if(!file) return NULL;

    Category* head = NULL;
    Category* tail = NULL;
    char line[256];

    while(fgets(line, sizeof(line), file)) {
        Category* cat = malloc(sizeof(Category));
        if(!cat) break;

        if(sscanf(line, "%d,%49[^\n]", &cat->id, cat->name) == 2) {
            cat->next = NULL;
            if(!head) head = cat;
            else tail->next = cat;
            tail = cat;
        } else {
            free(cat);
        }
    }

    fclose(file);
    return head;
}

void save_categories(Category* head) {
    FILE* file = fopen(CATEGORIES_FILE, "w");
    if(!file) return;

    Category* current = head;
    while(current) {
        fprintf(file, "%d,%s\n", current->id, current->name);
        current = current->next;
    }

    fclose(file);
}

void add_category(Category** head) {
    Category* new_cat = malloc(sizeof(Category));
    if(!new_cat) return;

    new_cat->id = 1;
    Category* current = *head;
    while(current) {
        if(current->id >= new_cat->id) new_cat->id = current->id + 1;
        current = current->next;
    }

    get_string_input("Category Name: ", new_cat->name, MAX_NAME_LEN);
    new_cat->next = *head;
    *head = new_cat;
}

void view_categories(Category* head) {
    printf("\nID\tName\n");
    printf("----------------\n");

    Category* current = head;
    while(current) {
        printf("%d\t%s\n", current->id, current->name);
        current = current->next;
    }
}

void update_category(Category* head) {
    if (!head) return;

    int id = get_valid_int("Enter Category ID to update: ");
    Category* current = head;

    while (current) {
        if (current->id == id) {
            get_string_input("New Name: ", current->name, MAX_NAME_LEN);
            printf("Category updated successfully.\n");
            return;
        }
        current = current->next;
    }

    printf("Category with ID %d not found.\n", id);
}

void delete_category(Category** head) {
    if (!head || !*head) return;

    int id = get_valid_int("Enter Category ID to delete: ");
    Category* current = *head;
    Category* prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) prev->next = current->next;
            else *head = current->next;
            free(current);
            printf("Category deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Category with ID %d not found.\n", id);
}

void free_categories(Category* head) {
    Category* current = head;
    while (current) {
        Category* temp = current;
        current = current->next;
        free(temp);
    }
}

void manage_suppliers() {
    Supplier* suppliers = load_suppliers();
    int choice;

    do {
        printf("\n=== SUPPLIERS ===\n");
        printf("1. Add\n2. View\n3. Update\n4. Delete\n0. Back\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: add_supplier(&suppliers); break;
            case 2: view_suppliers(suppliers); break;
            case 3: update_supplier(suppliers); break;
            case 4: delete_supplier(&suppliers); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }

        if(choice != 0) save_suppliers(suppliers);
    } while(choice != 0);

    free_suppliers(suppliers);
}

Supplier* load_suppliers() {
    FILE* file = fopen(SUPPLIERS_FILE, "r");
    if(!file) return NULL;

    Supplier* head = NULL;
    Supplier* tail = NULL;
    char line[256];

    while(fgets(line, sizeof(line), file)) {
        Supplier* sup = malloc(sizeof(Supplier));
        if(!sup) break;

        if(sscanf(line, "%d,%49[^,],%99[^\n]", &sup->id, sup->name, sup->contact) == 3) {
            sup->next = NULL;
            if(!head) head = sup;
            else tail->next = sup;
            tail = sup;
        } else {
            free(sup);
        }
    }

    fclose(file);
    return head;
}

void save_suppliers(Supplier* head) {
    FILE* file = fopen(SUPPLIERS_FILE, "w");
    if(!file) return;

    Supplier* current = head;
    while(current) {
        fprintf(file, "%d,%s,%s\n", current->id, current->name, current->contact);
        current = current->next;
    }

    fclose(file);
}

void add_supplier(Supplier** head) {
    Supplier* new_sup = malloc(sizeof(Supplier));
    if(!new_sup) return;

    new_sup->id = 1;
    Supplier* current = *head;
    while(current) {
        if(current->id >= new_sup->id) new_sup->id = current->id + 1;
        current = current->next;
    }

    get_string_input("Supplier Name: ", new_sup->name, MAX_NAME_LEN);
    get_string_input("Contact Info: ", new_sup->contact, MAX_CONTACT_LEN);
    new_sup->next = *head;
    *head = new_sup;
}

void view_suppliers(Supplier* head) {
    printf("\nID\tName\t\tContact\n");
    printf("-------------------------------\n");

    Supplier* current = head;
    while(current) {
        printf("%d\t%-10s\t%s\n", current->id, current->name, current->contact);
        current = current->next;
    }
}

void update_supplier(Supplier* head) {
    if (!head) return;

    int id = get_valid_int("Enter Supplier ID to update: ");
    Supplier* current = head;

    while (current) {
        if (current->id == id) {
            get_string_input("New Name: ", current->name, MAX_NAME_LEN);
            get_string_input("New Contact: ", current->contact, MAX_CONTACT_LEN);
            printf("Supplier updated successfully.\n");
            return;
        }
        current = current->next;
    }

    printf("Supplier with ID %d not found.\n", id);
}

void delete_supplier(Supplier** head) {
    if (!head || !*head) return;

    int id = get_valid_int("Enter Supplier ID to delete: ");
    Supplier* current = *head;
    Supplier* prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) prev->next = current->next;
            else *head = current->next;
            free(current);
            printf("Supplier deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Supplier with ID %d not found.\n", id);
}

void free_suppliers(Supplier* head) {
    Supplier* current = head;
    while (current) {
        Supplier* temp = current;
        current = current->next;
        free(temp);
    }
}
