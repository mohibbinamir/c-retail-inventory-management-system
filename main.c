#include "Common.h"
#include "Product.h"
#include "Inventory.h"
#include "Category_Supplier.h"
#include "User_Transaction.h"
#include "FileOperations.h"

int main() {
    int choice;
    do {
        printf("\n=== INVENTORY MANAGEMENT SYSTEM ===\n");
        printf("1. Product Management\n2. Inventory Management\n");
        printf("3. Category/Supplier Management\n4. User/Transaction Management\n");
        printf("5. File Operations\n0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch(choice) {
            case 1: product_main_menu(); break;
            case 2: inventory_menu(); break;
            case 3: category_supplier_menu(); break;
            case 4: user_transaction_menu(); break;
            case 5: file_operations_menu(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);

    return 0;
}
