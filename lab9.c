#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};
struct Node {
	struct RecordType record;
	struct Node *next;
}; 

// Fill out this structure
struct HashType
{
	struct Node ** buckets; // Array of pointers to linked lists
    int size; 
};

// Compute the hash function
int hash(int x, int size)
{
	return x % size; 
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	printf("\nRecords in Hash Table:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct Node *current = pHashArray->buckets[i];
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	// Your hash implementation
	int hashSize = 23; // Hash table size
    struct HashType hashTable;
    hashTable.size = hashSize;
    hashTable.buckets = (struct Node **)malloc(sizeof(struct Node *) * hashSize);
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable.buckets[i] = NULL; // Initialize all buckets to NULL
    }

    // Insert records into the hash table using separate chaining
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id, hashSize);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->record = pRecords[i];
        newNode->next = hashTable.buckets[index];
        hashTable.buckets[index] = newNode;
    }

    // Display records in the hash table
    displayRecordsInHash(&hashTable, hashSize);

    // Free memory
    for (int i = 0; i < hashTable.size; ++i){
		 struct Node *current = hashTable.buckets[i];
        while (current != NULL)
        {
            struct Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable.buckets);
    free(pRecords);

    return 0;
}