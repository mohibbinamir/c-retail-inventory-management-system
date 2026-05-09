#ifndef CATEGORY_SUPPLIER_H
#define CATEGORY_SUPPLIER_H

#include "Common.h"

typedef struct Category {
    int id;
    char name[MAX_NAME_LEN];
    struct Category* next;
} Category;

typedef struct Supplier {
    int id;
    char name[MAX_NAME_LEN];
    char contact[MAX_CONTACT_LEN];
    struct Supplier* next;
} Supplier;

void category_supplier_menu();
void manage_categories();
void manage_suppliers();

Category* load_categories();
void save_categories(Category* head);
void free_categories(Category* head);
void add_category(Category** head);
void view_categories(Category* head);
void update_category(Category* head);
void delete_category(Category** head);

Supplier* load_suppliers();
void save_suppliers(Supplier* head);
void free_suppliers(Supplier* head);
void add_supplier(Supplier** head);
void view_suppliers(Supplier* head);
void update_supplier(Supplier* head);
void delete_supplier(Supplier** head);

#endif
