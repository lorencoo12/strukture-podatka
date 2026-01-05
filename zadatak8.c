#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MALLOC_ERROR -1

// Cvor binarnog stabla pretrazivanja
struct treeNode;
typedef struct treeNode* TreePosition;
typedef struct treeNode {
    int data;
    TreePosition left;
    TreePosition right;
} TreeNode;

// Cvor reda (za level order)
struct queueNode;
typedef struct queueNode* QueuePosition;
typedef struct queueNode {
    TreePosition treeNode;
    QueuePosition next;
} QueueNode;

QueuePosition rear = NULL;

// Prototipovi
TreePosition insert(int x, TreePosition root);
TreePosition search(int x, TreePosition root);
int printPreorder(TreePosition root, int level);
int printInorder(TreePosition root, int level);
int printPostorder(TreePosition root, int level);
int printLevelorder(TreePosition root);
QueuePosition enqueue(TreePosition treeNode);
TreePosition dequeue();
TreePosition findMin(TreePosition root);
TreePosition deleteElement(TreePosition root, int x);
int deleteTree(TreePosition root);

int main()
{
    TreePosition root = NULL;
    int choice = 1, x;

    while (choice != 0) {
        printf("\n--- BINARNO STABLO PRETRAZIVANJA ---\n");
        printf("1 - Unos novog elementa\n");
        printf("2 - Pronadji element\n");
        printf("3 - Ispis inorder\n");
        printf("4 - Ispis preorder\n");
        printf("5 - Ispis postorder\n");
        printf("6 - Ispis levelorder\n");
        printf("7 - Brisanje elementa\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Unesite broj: ");
            scanf("%d", &x);
            root = insert(x, root);
            break;

        case 2:
            printf("Unesite broj: ");
            scanf("%d", &x);
            search(x, root);
            break;

        case 3:
            printf("\nInorder ispis:\n");
            printInorder(root, 0);
            break;

        case 4:
            printf("\nPreorder ispis:\n");
            printPreorder(root, 0);
            break;

        case 5:
            printf("\nPostorder ispis:\n");
            printPostorder(root, 0);
            break;

        case 6:
            printf("\nLevelorder ispis:\n");
            printLevelorder(root);
            break;

        case 7:
            printf("Unesite broj za brisanje: ");
            scanf("%d", &x);
            root = deleteElement(root, x);
            break;

        case 0:
            deleteTree(root);
            printf("Program zavrsen.\n");
            break;

        default:
            printf("Pogresan odabir!\n");
        }
    }

    return 0;
}

// Umetanje novog elementa u BST
TreePosition insert(int x, TreePosition root)
{
    if (root == NULL) {
        root = malloc(sizeof(TreeNode));
        if (!root) return NULL;

        root->data = x;
        root->left = root->right = NULL;
        return root;
    }

    if (x < root->data)
        root->left = insert(x, root->left);
    else if (x > root->data)
        root->right = insert(x, root->right);

    return root;
}

// Pretrazivanje elementa
TreePosition search(int x, TreePosition root)
{
    if (root == NULL) {
        printf("%d nije pronaden.\n", x);
        return NULL;
    }

    if (x < root->data)
        return search(x, root->left);
    else if (x > root->data)
        return search(x, root->right);

    printf("%d je pronaden!\n", x);
    return root;
}

// Preorder ispis
int printPreorder(TreePosition root, int level)
{
    if (root == NULL) return 0;

    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);

    printPreorder(root->left, level + 1);
    printPreorder(root->right, level + 1);

    return 0;
}

// Inorder ispis
int printInorder(TreePosition root, int level)
{
    if (root == NULL) return 0;

    printInorder(root->left, level + 1);

    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);

    printInorder(root->right, level + 1);

    return 0;
}

// Postorder ispis
int printPostorder(TreePosition root, int level)
{
    if (root == NULL) return 0;

    printPostorder(root->left, level + 1);
    printPostorder(root->right, level + 1);

    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);

    return 0;
}

// Enqueue (dodavanje u red)
QueuePosition enqueue(TreePosition treeNode)
{
    QueuePosition q = malloc(sizeof(QueueNode));
    if (!q) return NULL;

    q->treeNode = treeNode;

    if (rear == NULL) {
        q->next = q;
        rear = q;
    }
    else {
        q->next = rear->next;
        rear->next = q;
        rear = q;
    }

    return rear;
}

// Dequeue (uklanjanje iz reda)
TreePosition dequeue()
{
    if (rear == NULL) return NULL;

    QueuePosition front = rear->next;
    TreePosition result = front->treeNode;

    if (front == rear)
        rear = NULL;
    else
        rear->next = front->next;

    free(front);
    return result;
}

// Level order ispis
int printLevelorder(TreePosition root)
{
    if (root == NULL) return 0;

    rear = enqueue(root);

    while (rear != NULL) {
        TreePosition current = dequeue();
        printf("%d ", current->data);

        if (current->left) enqueue(current->left);
        if (current->right) enqueue(current->right);
    }

    printf("\n");
    return 0;
}

// Pronalazak najmanjeg elementa
TreePosition findMin(TreePosition root)
{
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

// Brisanje elementa iz stabla
TreePosition deleteElement(TreePosition root, int x)
{
    if (root == NULL) return NULL;

    if (x < root->data)
        root->left = deleteElement(root->left, x);
    else if (x > root->data)
        root->right = deleteElement(root->right, x);
    else {
        if (root->left && root->right) {
            TreePosition temp = findMin(root->right);
            root->data = temp->data;
            root->right = deleteElement(root->right, temp->data);
        }
        else {
            TreePosition temp = root;
            root = (root->left) ? root->left : root->right;
            free(temp);
        }
    }
    return root;
}

// Brisanje cijelog stabla
int deleteTree(TreePosition root)
{
    if (root == NULL) return 0;

    deleteTree(root->left);
    deleteTree(root->right);
    free(root);

    return 0;
}

