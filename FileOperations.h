#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "Common.h"
#include "Product.h"
#include "Inventory.h"
#include "Category_Supplier.h"
#include "User_Transaction.h"

void backup_all_data();
void restore_backup(const char* backup_path);
void validate_data_integrity();
void generate_system_report();
void export_to_csv(const char* filename);
void file_operations_menu();

int product_exists(Product* products, int product_id);
int supplier_exists(Supplier* suppliers, int supplier_id);
int category_exists(Category* categories, int category_id);

#endif
