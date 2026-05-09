#ifndef PRODUCT_H
#define PRODUCT_H

#include "Common.h"

typedef struct Product {
    int id;
    char name[MAX_NAME_LEN];
    float price;
    int quantity;
    int category_id;
    int supplier_id;
    struct Product* next;
} Product;

void product_main_menu();
void add_product(Product** head);
void update_product(Product* head);
void delete_product(Product** head);
void view_products(Product* head);

Product* load_products(const char* filename);
void save_products(const char* filename, Product* head);
void free_product_list(Product* head);

int generate_product_id(Product* head);
int product_exists(Product* head, int product_id);
void display_product(Product* p);

#endif
