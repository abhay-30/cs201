#include <stdio.h>
#include <stdlib.h>

#define MAXIMUM_TREE_HT 100
struct LL_node // Designing A Linked List to store the formed encoded bits of each corresponding character
{
    int data;             // Single bit either 0 or 1
    struct LL_node *next; // next node pointing to next bit
};
// Insert function to insert data in the node
struct LL_node *insert(struct LL_node *head, int x)
{
    struct LL_node *newnode = (struct LL_node *)malloc(sizeof(struct LL_node)); // dynamically allocating a newnode
    newnode->data = x;                                                          // Updating its data value to x
    newnode->next = NULL;                                                       // And updating its next pointer to NULL
    // If head pointer is NULL then simply eqaute head to newnode and return the head
    if (head == NULL)
    {
        head = newnode;
        return head;
    }
    else // Else we want to add value x to the tail of our linked list
    {
        struct LL_node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next; // Therfore we travel till the the end
        }
        temp->next = newnode; // And there add our newnode
        return head;          // And return the head value
    }
}
// Print Function to print a Linked List
void print_LL(struct LL_node *head)
{
    struct LL_node *temp = head;
    while (temp != NULL) // To traverse the whole list and printing the data
    {
        printf("%d", temp->data);
        temp = temp->next;
    }
    return;
}

// Globalyy Declaring an array of head pointers of size 0-127 (the ascii value of respective
// characters) and each charater is pointing to its encoded bit code
struct LL_node *encoded_code[128] = {0}; // Initiating all pointers to NULL


struct HuffmannTreenode // Basically declaring structure of a Minheap node for huffmann tree
{
    char data; // Charcater in the node

    unsigned freq; // Frequency of the corresponding character

    struct HuffmannTreenode *left, *right; // Leaft and Right Child of the node
};

struct HuffmannTree // Basically it is a collection of all huffmanntree nodes or minheap nodes
{

    unsigned size; // Corresponding to current size value of our collection of nodes

    unsigned capacity; // Corresponding to current capacity limit value of our collection of nodes

    struct HuffmannTreenode **array; // Array of huffmann tree node pointers
};

// Function to create a new huffmann tree node of given character data and its frequency value
struct HuffmannTreenode *newNode(char data, unsigned freq)
{
    struct HuffmannTreenode *temp = (struct HuffmannTreenode *)malloc(
        sizeof(struct HuffmannTreenode)); // Dynamically allocation of temp node of huffmann tree node type

    temp->left = temp->right = NULL; // Initiating its left and right node pointers to avoid garbage values
    temp->data = data;               // Updating temp's data value to given data value
    temp->freq = freq;               // Updating temp's frequency value to given frequency value

    return temp; // Returning the temp node
}

// Function to create the collection of huffmanntree nodes of a given capacity
struct HuffmannTree *createMinHeap(unsigned capacity)
{
    // Dynamically allocation of minheap huffmann tree
    struct HuffmannTree *minHeap = (struct HuffmannTree *)malloc(sizeof(struct HuffmannTree));

    minHeap->size = 0; // Initiating the size to 0

    minHeap->capacity = capacity; // Updating minheap's capacity value to given capacity value

    // Dynamically declaring the array
    minHeap->array = (struct HuffmannTreenode **)malloc(minHeap->capacity * sizeof(struct HuffmannTreenode *));
    return minHeap;
}

// Function to swap two HuffmannTree nodes
void swapHFtreenode(struct HuffmannTreenode **a, struct HuffmannTreenode **b)
{
    // Basic swapping process of two pointers using third pointer
    struct HuffmannTreenode *t = *a;
    *a = *b;
    *b = t;
}

// Function to min heapify
// Result of this function is minHeap modified such that index roots a Min-Heap
void minHeapify(struct HuffmannTree *minHeap, int index)
{

    int smallest = index;
    // Setting the left and right of the node minHeeap[index]
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Checking for the smallest element
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    // Updating the minHeap/Huffmann tree
    if (smallest != index)
    {
        swapHFtreenode(&minHeap->array[smallest], &minHeap->array[index]);
        minHeapify(minHeap, smallest);
    }
}

// Function to check that the size of our hufmann tree is 1 or not
int IsSize(struct HuffmannTree *minHeap)
{
    return (minHeap->size == 1);
}

// Function to extract the minimum value HuffmannTree node from the complete huffmann tree
struct HuffmannTreenode *extractMin(struct HuffmannTree *minHeap)
{

    struct HuffmannTreenode *temp = minHeap->array[0];
    // Copying the last value in the array to the root or first value in array
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;        // Decrease heap's size by 1
    minHeapify(minHeap, 0); // Calling the minheapify such that 0 roots the minheap/huffmann tree

    return temp; // Returning the min value node
}

// Function to insert a new Huffmann tree node to our huffmann tree
void INSERT(struct HuffmannTree *minHeap, struct HuffmannTreenode *minHeapNode)
{

    ++minHeap->size; // Increase heap's size by 1
    int i = minHeap->size - 1;
    // Comparing the added element with its parent
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode; // Assigning the new node to its proper place in the array/heap
}

// Function to build the Huffmann Tree
void BuildHeap(struct HuffmannTree *minHeap)
{
    int n = minHeap->size - 1;
    int i;
    // Perform reverse level order traversal from last non-leaf node and heapify each node
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Function to check a given node is leaf node or not
int IsLeaf(struct HuffmannTreenode *root)
{
    return !(root->left) && !(root->right);
}

// Function to create a Huffmann Tree of capacity equal to size
// and inserts all character of data[] in Huffmann Tree.
// Initially size of Huffmann Tree is equal to capacity
struct HuffmannTree *CreateHFtree(char data[], int freq[], int size)
{
    struct HuffmannTree *minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    BuildHeap(minHeap);

    return minHeap;
}

// Function that builds our tree
struct HuffmannTreenode *buildHuffmanTree(char data[], int freq[], int size)
{
    struct HuffmannTreenode *left, *right;
    struct HuffmannTreenode *node;
    // Create a Huffmann Tree of capacity equal to size
    struct HuffmannTree *minHeap = CreateHFtree(data, freq, size);

    // Iterate till the size of Tree doesn't become 1
    while (!IsSize(minHeap))
    {
        // Get the two minimum frequency HuffmannTreenode from Huffmann Tree
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Create a new internal HuffmannTreenode with frequency equal to the sum of the two nodes frequencies.
        // Make the two extracted node as left and right children of this new node and add this node to the Huffmann Tree

        node = newNode('$', left->freq + right->freq); //'$' is a special value for internal nodes, not used

        node->left = left;
        node->right = right;

        INSERT(minHeap, node);
    }
    // The remaining HuffmannTreenode is the root node and the Huffmann Tree is complete.
    return extractMin(minHeap);
}

// Function assigns huffman codes from the root of Huffman Tree and uses arr[] to store the codes
void assigncode(struct HuffmannTreenode *root, int arr[], int node)
{
    if (root->right) // Assign 1 to right edge
    {

        arr[node] = 1;
        assigncode(root->right, arr, node + 1);
    }

    if (root->left) // Assign 0 to left edge
    {

        arr[node] = 0;
        assigncode(root->left, arr, node + 1);
    }

    // If current node is a leaf node, then it contains one of the input characters and its code from arr[]
    if (IsLeaf(root))
    {
        long long int n = node;

        struct LL_node *head = NULL;
        for (int i = 0; i < n; ++i)
        {
            // Adding the code bit by bit of corresponding character in the head
            head = insert(head, arr[i]);
        }

        int j = (int)root->data;
        //And assigning the head to the corresponding ASCII value position in our hashtable 
        encoded_code[root->data] = head;
    }
}

//Function to print the encoded code of corresponding character from the hash table 
void printCodes(char data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%c: ", data[i]);
        int d = (int)data[i];//Converting the character to its ASCII value

        struct LL_node *temp = encoded_code[d];//Hashtable containing all the codes 

        while (temp != NULL)//To traverse the a head pointer to print down its code 
        {   
            printf("%d", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}
int size_code[128]={0};
void get_size(char data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        
        int d = (int)data[i];//Converting the character to its ASCII value

        struct LL_node *temp = encoded_code[d];//Hashtable containing all the codes 

        while (temp != NULL)//To traverse the a head pointer to print down its code 
        {   
          
            temp = temp->next;
            size_code[d]++;
        }
       
    }
}
//Main Function
int main()
{     printf("\n");
    printf("*******************************************************************************************************************************************************************\t\t\n");
    printf("\n");
     printf("HELLO AND WELCOME!! \n");
     
    printf("\n");  
     printf("ENTER NAME OF FILE WHICH YOU WANT TO COMPRESS    "); 
   
    
   
    char filename[20];
    FILE *input, *output;
    scanf("%s", filename);
    input = fopen(filename, "r");
    output = fopen("output.txt", "w");
    char c;
    int totalsize=0;
    int arr1[128] = {0};
    if (input == NULL)
    {
        printf("this can't be open");
    }
    else
    {
        while ((c = fgetc(input)) != EOF)
        {
            arr1[c]++;
            totalsize++;

        }
    }
    int ct = 0;
    for (int i = 0; i < 128; i++)
    {
        if (arr1[i] != 0)
        {
            ct++;
        }
    }
    char arr2[ct];
    int frequency[ct];
    for (int i = 0, j = 0; i < 128; i++)
    {
        if (arr1[i] != 0)
        {
            arr2[j] = (char)i;
            frequency[j] = arr1[i];
            j++;
        }
    }
    struct HuffmannTreenode *root = buildHuffmanTree(arr2, frequency, ct);
    int arrr[MAXIMUM_TREE_HT];
    int node = 0;

    assigncode(root, arrr, node);
    input = fopen(filename, "r");
    char d;
    while ((d = fgetc(input)) != EOF)
    {
        struct LL_node *temp = encoded_code[d];
        while (temp != NULL)
        {
            fprintf(output, "%d", temp->data);
            temp = temp->next;
        }
    }

    int print_decision;
   
    printf(" PRESS 1 : TO CONTINUE    \n");
    printf(" PRESS 2 : TO PRINT CODE OF EVERY CHARACTER   \n");
    scanf("%d", &print_decision);
    if (print_decision == 2)
    {
        printCodes(arr2, ct);
    }
    printf("OUTPUT.TXT IS YOUR ENCODED FILE  \n");
    fclose(output);

    printf(" PRESS 1 : TO CONTINUE    \n");
    printf(" PRESS 2 : TO DECODE OUTPUT.TXT   \n");
    int decrypt_decision;
    scanf("%d", &decrypt_decision);
    if (decrypt_decision == 2)
    {

        FILE *decode = fopen("output.txt", "r");
        FILE *orignal = fopen("decoded.txt", "w");

        char t;
        struct HuffmannTreenode *curr = root;
        while ((t = fgetc(decode)) != EOF)

        {
            if (t == '0')
                curr = curr->left;
            else if (t == '1')
            {
                curr = curr->right;
            }
            if (curr->left == NULL && curr->right == NULL)

            {
                fprintf(orignal, "%c", curr->data);
                curr = root;
            }
        }
            printf("DECODED.TXT IS YOUR DECODED FILE  \n");
    }
    printf(" PRESS 1 : TO EXIT THE PROGRAM    \n");
    printf(" PRESS 2 : TO KNOW HOW MUCH YOUR FILE COPMRESSED   \n");
    int SUMMARY;
    scanf("%d", &SUMMARY);
    if (SUMMARY == 2)
    {
    get_size(arr2,ct);
    int total_bits_endoded=0;
     
    for (int i = 0; i < ct; i++)
    {int d = (int)arr2[i];
      total_bits_endoded=total_bits_endoded+ size_code[d]*frequency[i];

    }
    float a=totalsize*8;
    float b=(a-total_bits_endoded)*100;

    printf(" TOTAL CHARACTER IN GIVEN FILE IS:    ""%d\n",totalsize);
    printf(" TOTAL NUMBER OF BITS USED:           ""%d\n",totalsize*8);
    printf(" TOTAL CHARACTER IN OUTPUT FILE IS:   ""%d\n",total_bits_endoded);
    printf(" TOTAL NUMBER OF BITS USED:           ""%d\n",total_bits_endoded);
    float percentage1=b/a;
    printf("YOUR FILE IS COMPRESSION BY :         ""%0.2f percent \n",percentage1);
    




    }
    return 0;
}
