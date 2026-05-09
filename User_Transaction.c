#include "User_Transaction.h"

void user_transaction_menu() {
    int choice;
    do {
        printf("\n=== USER/TRANSACTION MANAGEMENT ===\n");
        printf("1. Manage Users\n2. Manage Transactions\n0. Exit\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: manage_users(); break;
            case 2: manage_transactions(); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }
    } while(choice != 0);
}

void manage_users() {
    User* users = load_users();
    int choice;

    do {
        printf("\n=== USERS ===\n");
        printf("1. Add\n2. View\n3. Update\n4. Delete\n0. Back\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: add_user(&users); break;
            case 2: view_users(users); break;
            case 3: update_user(users); break;
            case 4: delete_user(&users); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }

        if(choice != 0) save_users(users);
    } while(choice != 0);

    free_users(users);
}

User* load_users() {
    FILE* file = fopen(USERS_FILE, "r");
    if(!file) return NULL;

    User* head = NULL;
    User* tail = NULL;
    char line[256];

    while(fgets(line, sizeof(line), file)) {
        User* user = malloc(sizeof(User));
        if(!user) break;

        if(sscanf(line, "%19[^,],%49[^,],%49[^,],%19[^\n]",
                 user->id, user->username, user->password, user->role) == 4) {
            user->next = NULL;
            if(!head) head = user;
            else tail->next = user;
            tail = user;
        } else {
            free(user);
        }
    }

    fclose(file);
    return head;
}

void save_users(User* head) {
    FILE* file = fopen(USERS_FILE, "w");
    if(!file) return;

    User* current = head;
    while(current) {
        fprintf(file, "%s,%s,%s,%s\n", current->id, current->username, current->password, current->role);
        current = current->next;
    }

    fclose(file);
}

void add_user(User** head) {
    User* new_user = malloc(sizeof(User));
    if(!new_user) return;

    printf("User ID: ");
    fgets(new_user->id, MAX_ID_LEN, stdin);
    new_user->id[strcspn(new_user->id, "\n")] = '\0';

    get_string_input("Username: ", new_user->username, MAX_NAME_LEN);
    get_string_input("Password: ", new_user->password, MAX_NAME_LEN);
    get_string_input("Role (admin/user): ", new_user->role, 20);

    new_user->next = *head;
    *head = new_user;
}

void view_users(User* head) {
    printf("\nID\tUsername\tRole\n");
    printf("-----------------------------\n");

    while (head) {
        printf("%s\t%s\t\t%s\n", head->id, head->username, head->role);
        head = head->next;
    }
}

void update_user(User* head) {
    char id[MAX_ID_LEN];
    get_string_input("Enter User ID to update: ", id, MAX_ID_LEN);

    while (head) {
        if (strcmp(head->id, id) == 0) {
            get_string_input("New Username: ", head->username, MAX_NAME_LEN);
            get_string_input("New Password: ", head->password, MAX_NAME_LEN);
            get_string_input("New Role: ", head->role, 20);
            printf("User updated successfully.\n");
            return;
        }
        head = head->next;
    }

    printf("User not found.\n");
}

void delete_user(User** head) {
    if (!head || !*head) return;

    char id[MAX_ID_LEN];
    get_string_input("Enter User ID to delete: ", id, MAX_ID_LEN);

    User* current = *head;
    User* prev = NULL;

    while (current) {
        if (strcmp(current->id, id) == 0) {
            if (prev) prev->next = current->next;
            else *head = current->next;
            free(current);
            printf("User deleted.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("User not found.\n");
}

void free_users(User* head) {
    while (head) {
        User* temp = head;
        head = head->next;
        free(temp);
    }
}

void manage_transactions() {
    Transaction* transactions = load_transactions();
    int choice;

    do {
        printf("\n=== TRANSACTIONS ===\n");
        printf("1. Add\n2. View\n3. Update\n4. Delete\n0. Back\nChoice: ");
        choice = get_valid_int("");

        switch(choice) {
            case 1: add_transaction(&transactions); break;
            case 2: view_transactions(transactions); break;
            case 3: update_transaction(transactions); break;
            case 4: delete_transaction(&transactions); break;
            case 0: break;
            default: printf("Invalid choice\n");
        }

        if(choice != 0) save_transactions(transactions);
    } while(choice != 0);

    free_transactions(transactions);
}

Transaction* load_transactions() {
    FILE* file = fopen(TRANSACTIONS_FILE, "r");
    if(!file) return NULL;

    Transaction* head = NULL;
    Transaction* tail = NULL;
    char line[256];

    while(fgets(line, sizeof(line), file)) {
        Transaction* trans = malloc(sizeof(Transaction));
        if(!trans) break;

        if(sscanf(line, "%d,%19[^,],%19[^,],%f,%19[^,],%19[^\n]",
                 &trans->id, trans->user_id, trans->product_id,
                 &trans->amount, trans->status, trans->date) == 6) {
            trans->next = NULL;
            if(!head) head = trans;
            else tail->next = trans;
            tail = trans;
        } else {
            free(trans);
        }
    }

    fclose(file);
    return head;
}

void save_transactions(Transaction* head) {
    FILE* file = fopen(TRANSACTIONS_FILE, "w");
    if(!file) return;

    Transaction* current = head;
    while(current) {
        fprintf(file, "%d,%s,%s,%.2f,%s,%s\n", current->id, current->user_id, current->product_id,
               current->amount, current->status, current->date);
        current = current->next;
    }

    fclose(file);
}

void add_transaction(Transaction** head) {
    Transaction* new_trans = malloc(sizeof(Transaction));
    if (!new_trans) return;

    new_trans->id = 1;
    Transaction* current = *head;
    while (current) {
        if (current->id >= new_trans->id) new_trans->id = current->id + 1;
        current = current->next;
    }

    get_string_input("User ID: ", new_trans->user_id, MAX_ID_LEN);
    get_string_input("Product ID: ", new_trans->product_id, MAX_ID_LEN);
    new_trans->amount = get_valid_float("Amount: ");
    get_string_input("Status: ", new_trans->status, MAX_STATUS_LEN);
    get_string_input("Date (YYYY-MM-DD): ", new_trans->date, 20);

    new_trans->next = *head;
    *head = new_trans;
}

void view_transactions(Transaction* head) {
    printf("\nID\tUser\tProduct\tAmount\tStatus\t\tDate\n");
    printf("-----------------------------------------------------------\n");

    while (head) {
        printf("%d\t%s\t%s\t%.2f\t%-10s\t%s\n", head->id, head->user_id, head->product_id,
               head->amount, head->status, head->date);
        head = head->next;
    }
}

void update_transaction(Transaction* head) {
    int id = get_valid_int("Enter Transaction ID to update: ");

    while (head) {
        if (head->id == id) {
            get_string_input("New Status: ", head->status, MAX_STATUS_LEN);
            get_string_input("New Date: ", head->date, 20);
            printf("Transaction updated.\n");
            return;
        }
        head = head->next;
    }

    printf("Transaction not found.\n");
}

void delete_transaction(Transaction** head) {
    if (!head || !*head) return;

    int id = get_valid_int("Enter Transaction ID to delete: ");
    Transaction* current = *head;
    Transaction* prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) prev->next = current->next;
            else *head = current->next;
            free(current);
            printf("Transaction deleted.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Transaction not found.\n");
}

void free_transactions(Transaction* head) {
    while (head) {
        Transaction* temp = head;
        head = head->next;
        free(temp);
    }
}
