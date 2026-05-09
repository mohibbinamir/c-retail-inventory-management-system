#ifndef INVENTORY_H
#define INVENTORY_H

#include "Common.h"

typedef struct InventoryItem {
    int id;
    int product_id;
    int quantity;
    int threshold;
    char location[MAX_NAME_LEN];
    struct InventoryItem* next;
} InventoryItem;

void inventory_menu();
InventoryItem* load_inventory();
void save_inventory(InventoryItem* head);
void free_inventory(InventoryItem* head);
void add_inventory_item(InventoryItem** head);
void update_inventory_item(InventoryItem* head);
void delete_inventory_item(InventoryItem** head);
void view_inventory(InventoryItem* head);
void generate_stock_report(InventoryItem* head);

#endif
