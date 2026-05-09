#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define MAX_INPUT 100
#define MAX_CONTACT_LEN 100
#define MAX_STATUS_LEN 20
#define DEFAULT_LOW_STOCK_THRESHOLD 5

#define PRODUCTS_FILE "Files/products.txt"
#define INVENTORY_FILE "Files/inventory.txt"
#define CATEGORIES_FILE "Files/categories.txt"
#define SUPPLIERS_FILE "Files/suppliers.txt"
#define USERS_FILE "Files/users.txt"
#define TRANSACTIONS_FILE "Files/transactions.txt"
#define ACTIVITY_LOG "Files/activity.log"
#define BACKUP_DIR "Files/backups/"
#define REPORT_FILE "Files/system_report.txt"

void clear_input_buffer();
int get_valid_int(const char* prompt);
float get_valid_float(const char* prompt);
void get_string_input(const char* prompt, char* buffer, int size);
int is_valid_id(const char* id);
void pause_screen();

#endif
