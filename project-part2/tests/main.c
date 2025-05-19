#include "../include/queuelist.h"
#include "../include/stack.h"
#include "../include/recursion.h"
#include "../include/bst.h"

int main() {
    int choice;

    do {
        printf("\n========== DICTIONARY MANAGER - MAIN MENU ==========\n");
        printf("1. Test List & Queue Modules (tlist_queue.c)\n");
        printf("2. Test Stack Module (stack.c)\n");
        printf("3. Test Recursion Module (recursion.c)\n");
        printf("4. Test BST Module (bst.c)\n");
        printf("0. Exit Program\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                test_TListQueue();
                break;
            case 2:
                test_stack();
                break;
            case 3:
                test_recursion();
                break;
            case 4:
                test_BST();
                break;
            case 0:
                printf("✅ Exiting. Thank you!\n");
                break;
            default:
                printf("❌ Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}

