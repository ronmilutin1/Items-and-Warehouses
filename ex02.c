#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/**********************************/
/**********YOU MUST REMARK IT BEFORE SUBMISSION************/
//#define DEBUGON
/**********YOU MUST REMARK IT BEFORE SUBMISSION************/
/*********************************/


/* Submitters
Name1: Ido Shwartz     ID: 318732856
Name2:  Ron Milutin    ID: 316389584
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* structures */
typedef struct item {
    char* name;
    int  id;
    struct wlst* warehouses;
} item;

typedef struct warehouse {
    char* location;
    int  code;
    struct itemlst* items;
} warehouse;

typedef struct itemlst {
    item* data;
    struct itemlst* next;
} itemlst;

typedef struct wlst {
    warehouse* data;
    struct wlst* next;
} wlst;

/*************** your's prototypes ***************************/

void new_item(itemlst** items, char* name, int id);
void new_warehouse(wlst** warehouses, char* location, int code);
void assign_item(itemlst** items, wlst** warehouses, int item_id, int warehouse_code);
void unassign_item(itemlst** items, wlst** warehouses, int item_id, int warehouse_code);
void print_items(itemlst* items);
void print_warehouses(wlst* warehouses);
void generate_and_assign(itemlst** items, wlst** warehouses);
void free_items(itemlst* items);
void free_warehouses(wlst* warehouses);
void print_error_message(int errid);
void check_allocation(void* ptr);
// void int_validation(int input);

/*************** your's functions ****************************/
void check_allocation(void* ptr) {
    if (ptr == NULL) {
        print_error_message(0); // Memory allocation failed
        return;
    }
}


void new_item(itemlst** items, char* name, int id) {
    // Input Validation
    if (id < 0) {
        print_error_message(0);
        return;
    }

    // Check if the item name exceeds the maximum length
    if (strlen(name) > 100) {
        print_error_message(3); // Error code for exceeding maximum length
        return;
    }

    // Check if the item with the given id already exists
    itemlst* current_check = *items;
    while (current_check != NULL) {
        if (current_check->data->id == id) {
            // Item with the given id already exists, return without changes
            return;
        }
        current_check = current_check->next;
    }

    // Create a new item
    item* new_item = (item*)malloc(sizeof(item));
    check_allocation(new_item);
    new_item->name = strdup(name);
    new_item->id = id;
    new_item->warehouses = NULL;

    // Create a new itemlst node
    itemlst* new_node = (itemlst*)malloc(sizeof(itemlst));
    new_node->data = new_item;

    // If the list is empty or the new item should be the first item in the list
    if (*items == NULL || (*items)->data->id > id) {
        new_node->next = *items;
        *items = new_node;
        return;
    }

    // Find the position to insert the new item
    itemlst* current = *items;
    while (current->next != NULL && current->next->data->id < id) {
        current = current->next;
    }

    // Insert the new item into the list
    new_node->next = current->next;
    current->next = new_node;
}


void new_warehouse(wlst** warehouses, char* location, int code) {
    // Input Validation
    if (code < 0) {
        print_error_message(0);
        return;
    }

    if (strlen(location) > 100) {
        print_error_message(3); // Error code for exceeding maximum length
        return;
    }

    // Check if the warehouse with the given code already exists
    wlst* current_check = *warehouses;
    while (current_check != NULL) {
        if (current_check->data->code == code) {
            // Warehouse with the given code already exists, return without changes
            return;
        }
        current_check = current_check->next;
    }

    // Create a new warehouse
    warehouse* new_warehouse = (warehouse*)malloc(sizeof(warehouse));
    check_allocation(new_warehouse);
    new_warehouse->location = strdup(location);
    new_warehouse->code = code;
    new_warehouse->items = NULL;

    // Create a new wlst node
    wlst* new_node = (wlst*)malloc(sizeof(wlst));
    new_node->data = new_warehouse;

    // If the list is empty or the new warehouse should be the first warehouse in the list
    if (*warehouses == NULL || (*warehouses)->data->code > code) {
        new_node->next = *warehouses;
        *warehouses = new_node;
        return;
    }

    // Find the position to insert the new warehouse
    wlst* current = *warehouses;
    while (current->next != NULL && current->next->data->code < code) {
        current = current->next;
    }

    // Insert the new warehouse into the list
    new_node->next = current->next;
    current->next = new_node;
}

void assign_item(itemlst** items, wlst** warehouses, int item_id, int warehouse_code) {
    // Find the item with the given id
    itemlst* currentItem = *items;
    while (currentItem != NULL) {
        if (currentItem->data->id == item_id) {
            // Item found, now find the warehouse with the given code
            wlst* currentWarehouse = *warehouses;
            while (currentWarehouse != NULL) {
                if (currentWarehouse->data->code == warehouse_code) {
                    // Warehouse found, create a new node to store the association
                    itemlst* newItemInWarehouse = (itemlst*)malloc(sizeof(itemlst));
                    check_allocation(newItemInWarehouse);
                    newItemInWarehouse->data = currentItem->data;
                    newItemInWarehouse->next = NULL; // Initialize the next pointer to NULL

                    // Insert the new item into the warehouse's list of items, sorted by ID
                    if (currentWarehouse->data->items == NULL || currentItem->data->id < currentWarehouse->data->items->data->id) {
                        newItemInWarehouse->next = currentWarehouse->data->items;
                        currentWarehouse->data->items = newItemInWarehouse;
                    } else {
                        itemlst* prevItem = currentWarehouse->data->items;
                        while (prevItem->next != NULL && prevItem->next->data->id < currentItem->data->id) {
                            prevItem = prevItem->next;
                        }
                        newItemInWarehouse->next = prevItem->next;
                        prevItem->next = newItemInWarehouse;
                    }

                    // Create a new wlst node to store the association in the item's list of warehouses
                    wlst* newWarehouseForItem = (wlst*)malloc(sizeof(wlst));
                    check_allocation(newWarehouseForItem);
                    newWarehouseForItem->data = currentWarehouse->data;
                    newWarehouseForItem->next = NULL; // Initialize the next pointer to NULL

                    // Insert the new warehouse into the item's list of warehouses, sorted by code
                    if (currentItem->data->warehouses == NULL || currentWarehouse->data->code < currentItem->data->warehouses->data->code) {
                        newWarehouseForItem->next = currentItem->data->warehouses;
                        currentItem->data->warehouses = newWarehouseForItem;
                    } else {
                        wlst* prevWarehouse = currentItem->data->warehouses;
                        while (prevWarehouse->next != NULL && prevWarehouse->next->data->code < currentWarehouse->data->code) {
                            prevWarehouse = prevWarehouse->next;
                        }
                        newWarehouseForItem->next = prevWarehouse->next;
                        prevWarehouse->next = newWarehouseForItem;
                    }

                    return; // Exit function after assignment
                }
                currentWarehouse = currentWarehouse->next;
            }
            // Warehouse with the given code not found
            print_error_message(2);
            return; // Exit function without assignment
        }
        currentItem = currentItem->next;
    }
    // Item with the given id not found
    print_error_message(2);
}


void unassign_item(itemlst** items, wlst** warehouses, int item_id, int warehouse_code) {
    // Find the item with the given id
    itemlst* currentItem = *items;
    while (currentItem != NULL) {
        if (currentItem->data->id == item_id) {
            // Item found, now find the warehouse with the given code
            wlst* currentWarehouse = *warehouses;
            while (currentWarehouse != NULL) {
                if (currentWarehouse->data->code == warehouse_code) {
                    // Warehouse found, check if item is assigned to this warehouse
                    itemlst* currentWarehouseItem = currentWarehouse->data->items;
                    itemlst* prevWarehouseItem = NULL;
                    while (currentWarehouseItem != NULL) {
                        if (currentWarehouseItem->data->id == item_id) {
                            // Item found in warehouse, unassign it
                            if (prevWarehouseItem == NULL) {
                                // Item is the first in warehouse's list
                                currentWarehouse->data->items = currentWarehouseItem->next;
                            } else {
                                prevWarehouseItem->next = currentWarehouseItem->next;
                            }
                            free(currentWarehouseItem);
                            // Update the item's list of warehouses
                            wlst* current = currentItem->data->warehouses;
                            wlst* prev = NULL;
                            while (current != NULL) {
                                if (current->data->code == warehouse_code) {
                                    // Warehouse found in item's list, remove it
                                    if (prev == NULL) {
                                        // Warehouse is the first in item's list
                                        currentItem->data->warehouses = current->next;
                                    } else {
                                        prev->next = current->next;
                                    }
                                    free(current);
                                    return; // Exit function after unassignment
                                }
                                prev = current;
                                current = current->next;
                            }
                            // Warehouse not found in item's list
                            print_error_message(1);
                            return; // Exit function without unassignment
                        }
                        prevWarehouseItem = currentWarehouseItem;
                        currentWarehouseItem = currentWarehouseItem->next;
                    }
                    // Item not found in warehouse
                    print_error_message(1);
                    return; // Exit function without unassignment
                }
                currentWarehouse = currentWarehouse->next;
            }
            // Warehouse with the given code not found
            print_error_message(1);
            return; // Exit function without unassignment
        }
        currentItem = currentItem->next;
    }
    // Item with the given id not found
    print_error_message(1);
}

void print_items(itemlst* items) {
    printf("item LIST:");
    itemlst* currentItem = items;

    if (currentItem==NULL){
        printf(" EMPTY!\n");
    }
    else{
        printf("\n");
    }

    while (currentItem != NULL) {
        printf("%d:%s\n", currentItem->data->id, currentItem->data->name);

        // Print warehouses associated with the current item

        wlst* currentWarehouse = currentItem->data->warehouses;
        if (currentWarehouse != NULL) {
            printf("Item Warehouses: ");
        }

        while (currentWarehouse != NULL) {

            if(currentWarehouse->next == NULL){
                printf("%d-%s",currentWarehouse->data->code ,currentWarehouse->data->location);
                currentWarehouse = currentWarehouse->next;
            }

            else{
                printf("%d-%s, ",currentWarehouse->data->code ,currentWarehouse->data->location);
                currentWarehouse = currentWarehouse->next;
            }

        }
        printf("\n");

        currentItem = currentItem->next;
    }
}

void print_warehouses(wlst* warehouses) {
    printf("warehouse LIST:");
    wlst* currentWarehouse = warehouses;

    if (currentWarehouse==NULL){
        printf(" EMPTY!\n");
    }
    else{
        printf("\n");
    }

    while (currentWarehouse != NULL) {
        printf("Warehouse code %d, Warehouse name: %s\n", currentWarehouse->data->code, currentWarehouse->data->location);

        // Print items associated with the current warehouse
        itemlst* currentItem = currentWarehouse->data->items;
        if (currentItem!=NULL){
            printf("items: ");
        }
        while (currentItem != NULL) {
            printf("ID %d Name %s | ",currentItem->data->id, currentItem->data->name);
            currentItem = currentItem->next;
        }
        printf("\n");

        currentWarehouse = currentWarehouse->next;
    }
}

void generate_and_assign(itemlst** items, wlst** warehouses) {
    srand(1948); // Lock the random number generation

    // Generate 100 items with IDs from 0 to 99
    for (int i = 0; i < 100; ++i) {
        char item_name[20];
        sprintf(item_name, "Item%d", i);
        new_item(items, item_name, i);
    }

    // Generate 10 warehouses with codes from 0 to 9
    for (int i = 0; i < 10; ++i) {
        char warehouse_location[20];
        sprintf(warehouse_location, "Warehouse%d", i);
        new_warehouse(warehouses, warehouse_location, i);
    }

    // Assign each item to a random warehouse
    itemlst* current_item = *items;
    while (current_item != NULL) {
        int random_code = rand() % 10;
        assign_item(items, warehouses, current_item->data->id, random_code);
        current_item = current_item->next;
    }
}

void free_items(itemlst* items) {
    while (items != NULL) {
        itemlst* temp = items;
        items = items->next;
        free(temp->data->name);
        free(temp->data);
        free(temp);
    }
}

void free_warehouses(wlst* warehouses) {
    while (warehouses != NULL) {
        wlst* temp = warehouses;
        warehouses = warehouses->next;
        free(temp->data->location);
        free(temp->data);
        free(temp);
    }
}

/**************Generate And Assign Items And Warehouses*******************/

/*DO NOT TOUCH THIS FUNCTION */
void getstring(char* buf, int length) {
    int len;
    scanf("%s", buf);
    len = (int)strlen(buf);
    if (buf[len - 1] == '\n')
        buf[len - 1] = '\0';
}

/*DO NOT TOUCH THIS FUNCTION */
void print_error_message(int errid) {
#ifdef DEBUGON
    fprintf(stderr, "BGU ERROR %d\n", errid);
#else
    fprintf(stderr, "BGU ERROR\n");
#endif
    exit(-1);
}

/*DO NOT TOUCH THIS MAIN TEMPLATE - ONLY ADD YOUR FUNCTIONS CALLS */
int main() {
    itemlst* items = NULL;
    wlst* warehouses = NULL;
    char  c;
    char  buf[100];
    int   id, num;

    do {
        printf("Choose:\n"
               "    i - new item\n"
               "    w - new warehouse\n"
               "    a - assign an item to a warehouse\n"
               "    u - unassign an item from a warehouse(not delete!)\n"
               "    p - print status\n"
               "    g - generating and assigning 100 items to 10 warehouses\n"
               "    q - quit\n");

        while ((c = (char)getchar()) == '\n');
        getchar();

        switch (c) {
            case 'i':
                printf("Adding new item.\n");

                printf("item name: ");
                getstring(buf, 100);

                printf("item ID: ");
                scanf("%d", &id);

                printf("\n Add new item: name %s item id: %d",buf, id);

                new_item(&items, buf, id);

                break;

            case 'w':
                printf("Adding new warehouse.\n");

                printf("\n warehouse name: ");
                getstring(buf, 100);

                printf("\n warehouse code: ");
                scanf("%d", &num);

                printf("\n Add new warehouse: name %s warehouse code: %d",buf, num);

                new_warehouse(&warehouses, buf, num);
                break;

            case 'a':
                printf("Assign an item to a warehouse.\n");

                printf("item ID: ");
                scanf("%d", &id);

                printf("warehouse code: ");
                scanf("%d", &num);

                assign_item(&items, &warehouses, id, num);
                break;

            case 'u':
                printf("Remove an item from a warehouse.\n");

                printf("item ID: ");
                scanf("%d", &id);

                printf("warehouse code: ");
                scanf("%d", &num);

                unassign_item(&items, &warehouses, id, num);
                break;

            case 'p':
                printf("Printing status.\n");

                print_items(items);
                print_warehouses(warehouses);
                break;

            case 'g':
                printf("Generating and assigning items to warehouses\n");

                generate_and_assign(&items, &warehouses);
                break;

            case 'q':
                printf("Quitting...\n");
                break;
        }

        if (c != 'q')
            printf("\n");
    } while (c != 'q');

    // Free dynamically allocated memory
    free_items(items);
    free_warehouses(warehouses);

    exit(0);
}