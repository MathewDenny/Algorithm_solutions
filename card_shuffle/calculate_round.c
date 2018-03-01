/*
 * calculate_round.c
 *
 *  Created on: Feb 28, 2018
 *      Author: denny
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum g_error_codes  {
	E_SUCCESS = 0,
	E_INV_PARAM = -1,
	E_MEM = -2,
	E_UNDERFLOW = -3,

};

void error_handler(int32_t n)
{
	switch (n) {
	case E_INV_PARAM :
		printf("Parameter Error\n\r");
		break;
	case E_MEM :
		printf("Memory allocation error\n\r");
		break;
	}
}

// A utility function to swap to integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// A function to generate a random permutation of arr[]
void randomize( uint32_t arr[], uint32_t n)
{

    srand (2);

    // Start from the last element and swap one by one
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);

        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

/*********** QUEUE IMPLEMENTATION ***************/
struct qNode {
	uint32_t data;
	struct qNode *next;
};

typedef struct queue {
	struct qNode* head;
	struct qNode* tail;

} deck;



int32_t create_new_node(struct queue *Qptr, uint32_t data, struct qNode** node_d_ptr)
{

	if (Qptr == NULL)
		return E_INV_PARAM;
	*node_d_ptr = (struct qNode*) malloc(sizeof(struct qNode));
	struct qNode* node_ptr = *node_d_ptr;

	if (node_ptr == NULL)
		return E_MEM;
	node_ptr->data = data;
	node_ptr->next = NULL;
	return E_SUCCESS;

}

int dequeue(struct queue *Qptr, struct qNode **node_d_ptr)
{
	if (Qptr == NULL)
			return E_INV_PARAM;
	int err = 0;

	if (Qptr->head == NULL) {
		printf("Queue Empty\n\r");
		return E_UNDERFLOW;
	}

	// Get the head node
	*node_d_ptr = Qptr->head;

	// Update the queue pointers
	Qptr->head = Qptr->head->next;

	if (Qptr->head == NULL) {
		Qptr->tail = NULL;
	}
	return E_SUCCESS;
}

int enqueue(struct queue *Qptr, uint32_t data)
{
	if (Qptr == NULL)
			return E_INV_PARAM;
	struct qNode* p_new_node = NULL;
	int err = 0;
	if ((err = create_new_node(Qptr, data, &p_new_node)) < 0) {
		printf("Error creating new node: "); error_handler(err);
		return err;
	}
	if (Qptr->tail == NULL) {
		Qptr->tail = p_new_node;
		Qptr->head = p_new_node;

	} else {
		// Update the queue pointers
		Qptr->tail->next = p_new_node;
		Qptr->tail = p_new_node;
	}
	return E_SUCCESS;
}

// Create a queue and if size is specified create that many nodes and initialize the nodes to random values
int32_t create_queue(struct queue *Qptr, uint32_t size)
{
	if (Qptr == NULL)
	 Qptr = (struct queue*) malloc(sizeof(struct queue));
	if (Qptr == NULL)
		return E_MEM;
	Qptr->head = NULL;
	Qptr->tail = NULL;
	if (size) {
		// If the size is specified create that many nodes using Enqueue operation
		// Randomly assign values
		int* random_nos = malloc(size * sizeof(int32_t));
		int i = 0;

		// Filling the array and shuffling
		while (i < size) {
			random_nos[i]= i + 1;
			i++;
		}
		randomize(random_nos, size);


		i = 0;
		while (i < size) {
			printf("%d ", random_nos[i]);
			enqueue(Qptr, random_nos[i++]);

		}
		printf("\n\r");
	}
	return E_SUCCESS;

}

int32_t delete_queue(struct queue *Qptr)
{
	if (Qptr == NULL)
		return E_INV_PARAM;
	struct qNode *node;
	while (Qptr->head != NULL || Qptr->tail != NULL) {
		dequeue(Qptr, &node);
		free(node);
		//print_queue(Qptr);
	}
	return E_SUCCESS;

}

void print_queue(struct queue *Qptr) {
	struct qNode *curr_node = Qptr->head;

	while (curr_node != NULL ) {
		printf("%d, ", curr_node->data);
		curr_node = curr_node->next;
	}
	printf(" \n\r");
}

/*********** END :QUEUE IMPLEMENTATION **********/



int32_t create_deck(deck* deck_ptr, int32_t n)
{
	int32_t err;

	if ((err = create_queue(deck_ptr, n)) < 0) {
		return err;
	}
	return E_SUCCESS;
}

int32_t print_deck(deck* deck_ptr)
{

	if (deck_ptr == NULL)
			return E_INV_PARAM;
	print_queue(deck_ptr);

	return E_SUCCESS;
}

void main(int argc, char** argv)
{

	uint32_t deck_size = 0;
	int32_t err;
	if (argc < 2) {
		printf("Format is <%s> <number>\n\r", argv[0]);
		exit(0);
	}

	else
		deck_size = atoi(argv[1]);

	if (!deck_size) {
		printf("Invalid Deck Size. Exiting.\n\r");
		exit(0);
	}
	printf("The deck size = %u \n\r", deck_size);

	// Creates a deck with deck_size elements
	// Also randomly initializes the deck.
	deck card_deck;
	if ( (err = create_deck(&card_deck, deck_size)) < 0) {
		printf("Error creating deck: "); error_handler(err);
		exit(0);
	}

	printf("Printing the queue now\n\r");
	fflush(stdout);

	print_queue(&card_deck);

	printf("Deleting the queue now\n\r");
	fflush(stdout);

	delete_queue(&card_deck);



}
