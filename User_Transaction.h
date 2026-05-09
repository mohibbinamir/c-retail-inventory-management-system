#ifndef USER_TRANSACTION_H
#define USER_TRANSACTION_H

#include "Common.h"

typedef struct User {
    char id[MAX_ID_LEN];
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
    char role[20];
    struct User* next;
} User;

typedef struct Transaction {
    int id;
    char user_id[MAX_ID_LEN];
    char product_id[MAX_ID_LEN];
    float amount;
    char status[MAX_STATUS_LEN];
    char date[20];
    struct Transaction* next;
} Transaction;

void user_transaction_menu();
void manage_users();
void manage_transactions();

User* load_users();
void save_users(User* head);
void free_users(User* head);
void add_user(User** head);
void view_users(User* head);
void update_user(User* head);
void delete_user(User** head);

Transaction* load_transactions();
void save_transactions(Transaction* head);
void free_transactions(Transaction* head);
void add_transaction(Transaction** head);
void view_transactions(Transaction* head);
void update_transaction(Transaction* head);
void delete_transaction(Transaction** head);

#endif
