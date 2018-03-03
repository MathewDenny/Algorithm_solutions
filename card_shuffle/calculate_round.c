/** @file calculate_round.c.c
*
* @brief Emulates a deck of cards using a queue data structure
* 			The code gives support to find the number of rounds required to
* 			bring the deck to the original order on performing a defined shuffling action
* 			The action involves taking out the facing element and putting the next facing
* 			card to the bottom of the deck.
*
*
* @par  deck_size: the size of the deck : example 20
* Author: Denny Mathew; dm4028@nyu.edu
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


/*********** HELPER FUNCTIONS **********************/

/*!
* @brief Print a helper string describing an error
* @param[in] n  The error number.
* @param[in] num2  The second number to be compared.
* @return void
*/
void error_handler(int32_t n)
{
	switch (n) {
	case E_INV_PARAM :
		printf("Parameter Error\n\r");
		break;
	case E_MEM :
		printf("Memory allocation error\n\r");
		break;
	default :
		printf("Error: %d\n\r", n);
	}
}

/*!
* @brief Function to find the gcd of two numbers
* @param[in] num1: First number
* @param[in] num2: Second number
* @return uint8_t: gcd of two numbers
*/
uint8_t gcd(uint32_t num1, uint32_t num2)
{
    if (num2==0)
        return num1;
    return gcd(num2, num1 % num2);
}

/*!
* @brief Function to find the lcm of an array of numbers
* @param[in] arr: Array of uint32_t numbers
* @param[in] n : size of the array
* @return uint64_t: lcm of the numbers
*/
uint64_t findlcm(uint32_t* arr, uint32_t n)
{
	uint64_t lcm = arr[0];
    for (int i=1; i<n; i++)
    	lcm = (((arr[i] * lcm)) / (gcd(arr[i], lcm)));

    return lcm;
}

/*!
* @brief Function to swap two numbers
* @param[in] a: pointer to first element
* @param[in] b : pointer to second element
* @return void
*/
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*!
* @brief Function to generate a random permutation of an array
* @param[in] arr: Input array
* @param[in] size : Size of the array
* @return void
*/
void randomize( uint32_t arr[], uint32_t size)
{
    srand (0);
    int rand_index;
    for (int i = size-1; i > 0; i--)
    {
        // Pick a random index using the mod operator between 0 to i
    	rand_index = rand() % (i+1);

        // Swap arr[i] arr[random index]
        swap(&arr[i], &arr[rand_index]);
    }
}
/*********** END :HELPER FUNCTIONS **************/


/*********** QUEUE IMPLEMENTATION ***************/
struct qNode {
	uint32_t data;
	struct qNode *next;
};

typedef struct queue {
	struct qNode* head;
	struct qNode* tail;

} deck;

/*!
* @brief Function to generate a new node in the array and initialize it with data
* @param[in] Qptr: Pointer to the queue
* @param[in] data : Value to be inserted
* @param[out] node_d_ptr : Pointer to the new node created
* @return int32_t: Error code
*/
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

/*!
* @brief Function to remove the head element from a queue
* @param[in] Qptr: Pointer to the queue
* @param[out] node_d_ptr : Pointer to the node that was dequeued
* @return int32_t: Error code
*/
int32_t dequeue(struct queue *Qptr, struct qNode **node_d_ptr)
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

/*!
* @brief Function to add an element to the tail
* @param[in] Qptr: Pointer to the queue
* @param[in] data : Data value of the new node
* @return int32_t: Error code
*/
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
		// Head and tail points to the new node
		Qptr->tail = p_new_node;
		Qptr->head = p_new_node;

	} else {
		// Update the tail and next pointers
		Qptr->tail->next = p_new_node;
		Qptr->tail = p_new_node;
	}
	return E_SUCCESS;
}


/*!
* @brief Function to Initialize a queue and if size is specified
* 						create that many nodes and initialize the nodes to random values
* @param[in] Qptr: Pointer to the queue
* @param[in] size : number of elements in the queue
* @return int32_t: Error code
*/
int32_t initialize_queue(struct queue *Qptr, uint32_t size)
{

	if (Qptr == NULL)
		return E_MEM;
	Qptr->head = NULL;
	Qptr->tail = NULL;

	if (size) {
		// If the size is specified create that many nodes using Enqueue operation
		// and randomly assign values
		int* random_nos = malloc(size * sizeof(int32_t));
		int i = 0;

		// Getting a random permutation
		while (i < size) {
			random_nos[i]= i + 1;
			i++;
		}
		randomize(random_nos, size);

		// Adding nodes to the queue
		i = 0;
		while (i < size) {
			// printf("%d ", random_nos[i]);
			enqueue(Qptr, random_nos[i++]);
		}
		// printf("\n\r");
	}
	return E_SUCCESS;

}

/*!
* @brief Function to empty a queue
* @param[in] Qptr: Pointer to the queue
* @return int32_t: Error code
*/
int32_t empty_queue(struct queue *Qptr)
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

/*!
* @brief Function to print the elements of a queue
* @param[in] Qptr: Pointer to the queue
* @return void
*/
void print_queue(struct queue *Qptr) {
	struct qNode *curr_node = Qptr->head;

	while (curr_node != NULL ) {
		printf("%d, ", curr_node->data);
		curr_node = curr_node->next;
	}
	printf(" \n\r");
}
/*!
* @brief Function to get the nth element value in the queue
* @param[in] Qptr: Pointer to the queue
* @param[in] pos: position value
* @param[out] p_data: Data value found out
* @return data
*/
int32_t get_element(struct queue *Qptr, uint32_t pos, uint32_t* p_data) {
	if (Qptr == NULL || p_data == NULL)
		return E_INV_PARAM;
	struct qNode *curr_node = Qptr->head;
	int i = 0;
	while (curr_node != NULL) {
		if(i == pos)
			*p_data = curr_node->data;
		else
			curr_node = curr_node->next;
		i++;
	}
	if (curr_node == NULL)
		// size of the queue is lesser than position
		return E_INV_PARAM;
	else
		return E_SUCCESS;
}

/*!
* @brief Helper function to check queue empty condition
* @param[in] Qptr: Pointer to the queue
* @return : 1 on empty , 0 on not empty
*/
uint8_t is_empty(struct queue *Qptr)
{

	if (Qptr->head == NULL && Qptr->tail == NULL)
		return 1;
	else
		return 0;
}

/*********** END :QUEUE IMPLEMENTATION **********/

/*!
* @brief Interface function to initialize a deck
* @param[in] deck_ptr: Pointer to the deck
* @param[in] n : number of elements in the deck
* @return int32_t: Error code
*/
int32_t initialize_deck(deck* deck_ptr, int32_t n)
{
	int32_t err;

	if ((err = initialize_queue(deck_ptr, n)) < 0) {
		return err;
	}
	return E_SUCCESS;
}

/*!
* @brief Interface function to print a deck
* @param[in] deck_ptr: Pointer to the deck
* @return int32_t: Error code
*/
int32_t print_deck(deck* deck_ptr)
{

	if (deck_ptr == NULL)
			return E_INV_PARAM;
	print_queue(deck_ptr);

	return E_SUCCESS;
}

/*!
* @brief Make an exact copy of a deck
* 			- Function deletes the elements if the second deck is not empty
* @param[in] deck_ptr: Pointer to the main deck
* @param[in] new_deck_ptr: Pointer to the (empty) deck to be copied to
* @return int32_t: Error code
*/
int32_t copy_deck(deck* deck_ptr, deck* new_deck_ptr)
{
	if (deck_ptr == NULL || new_deck_ptr == NULL)
		return E_INV_PARAM;

	struct qNode *curr_node = deck_ptr->head;

	if(!is_empty(new_deck_ptr))
		empty_queue(new_deck_ptr);

	while (curr_node != NULL ) {
		enqueue(new_deck_ptr, curr_node->data);
		curr_node = curr_node->next;
	}
	return E_SUCCESS;
}

/*!
* @brief Perform the defined reordering of the deck till the deck is empty
* @param[in] deck_ptr: Pointer to the main deck
* @param[in] table_ptr: Pointer to the deck on table that will contain the reordered cards
* @param[in] size : number of elements in the queue
* @return int32_t: Error code
*/
int32_t do_a_round(deck* deck_ptr, deck* table_ptr, uint32_t size)
{
	if (deck_ptr == NULL || table_ptr == NULL)
		return E_INV_PARAM;
	struct qNode *node;

	// Sanity check. If the table deck is not empty before the round
	if(!is_empty(table_ptr))
			empty_queue(table_ptr);

	// Perform the series of enqueue and dequeue operations till its empty
	while(!is_empty(deck_ptr)) {
		dequeue(deck_ptr, &node);
		enqueue(table_ptr, node->data);
		free(node);

		if (!is_empty(deck_ptr)) {
			dequeue(deck_ptr, &node);
			enqueue(deck_ptr, node->data);
			free(node);
		}
	}
	// printf("INFO: A round finished\n\r");
	return E_SUCCESS;
}

/*!
* @brief Find the number of rounds it will take to put the deck back in order
* @param[in] new_deck: Pointer to the reordered deck
* @param[in] original_deck: Pointer to the original deck
* @param[in] size : number of elements in the queue
* @param[out] p_no_of_rounds : calculated number of rounds required
* @return int32_t: Error code
*/
int32_t rounds_calculate(deck* new_deck, deck* original_deck,
		uint32_t size,
		uint64_t* p_no_of_rounds)
{
	/*
	 * Code to find the order of each element : the number of rounds after which an element returns to
	 * 		its original position.
	 *		For that trace the path each node takes from root position to new position in new array
	 *		Then repeat for the node at that new position in original array
	 *		Repeat till we arrive at root position. Number of nodes traversed gives the order of the element.
	 */

	if (new_deck == NULL || original_deck == NULL || p_no_of_rounds == NULL)
		return E_INV_PARAM;

	struct qNode *orig_node = original_deck->head;
	struct qNode *curr_node = new_deck->head;

	//	// To speed up the lookup . Not used since arrays are discouraged
	//	int *temp_array = malloc(size * sizeof(uint32_t));

	// Array to store the order of each element
	int *counts = malloc(size * sizeof(uint32_t));

	int i = 0, j = 0, pos = 0, data = orig_node->data, root_data;

	//	while (i < size) {
	//		temp_array[i] = orig_node->data;
	//		orig_node = orig_node->next;
	//		i++;
	//	}

	orig_node = original_deck->head;
	for (i = 0; i < size; i++) {
		// for each element find the order
		counts[i] = 0;
		data = orig_node->data;
		root_data = data;
		curr_node = new_deck->head;

		// search in the new deck
		for (j = 0; j < size ; j++) {
			if(data == curr_node->data) {
				counts[i] = counts[i] + 1;
				// do a reverse of the position since we havent reversed the deck
				pos = size  - 1 - j;

				// Find the pos element in original deck
				// data = temp_array[pos];
				get_element(original_deck, pos, &data);

				if (data == root_data) {
					// Thats all we need. Proceed to next element
					break;
				}else {
					// search again
					j = -1;
					curr_node = new_deck->head;
				}
			}
			else {
				curr_node = curr_node->next;
			}
		}
		// Get the next node to find
		orig_node = orig_node->next;
	}

	printf("INFO: Finding Order finished\n\r");

	// // print the counts
	// i = 0;
	// while (i < size) {
	// 	printf("%d ", counts[i]);
	//	i++;
	// }

	*p_no_of_rounds = findlcm(counts, size);
	return E_SUCCESS;
}


void main(int argc, char** argv)
{

	uint32_t deck_size = 0;
	int32_t err;
	uint64_t rounds;

	if (argc < 2) {
		printf("Format is <%s> <number>\n\r", argv[0]);
		exit(0);
	} else {
		deck_size = atoi(argv[1]);
	}

	if (!deck_size) {
		printf("Invalid Deck Size. Exiting.\n\r");
		exit(0);
	}
	printf("The deck size = %u \n\r", deck_size);

	// Creates a deck with deck_size elements
	// Also randomly initializes the deck.
	deck card_deck, orig_deck;
	if ((err = initialize_deck(&card_deck, deck_size)) < 0) {
		printf("Error initializing deck: "); error_handler(err);
		exit(0);
	}

	printf("Printing the original deck\n\r");
	//print_deck(&card_deck);

	// Creating a copy of the original
	if ((err = initialize_deck(&orig_deck, 0)) < 0) {
			printf("Error initializing deck: "); error_handler(err);
			exit(0);
	}
	if ((err = copy_deck(&card_deck, &orig_deck)) < 0) {
		printf("Error copying deck: "); error_handler(err);
		exit(0);
	}

	// Creating the deck to store the data after a round
	deck table_deck;
	if ( (err = initialize_deck(&table_deck, 0)) < 0) {
		printf("Error creating deck: "); error_handler(err);
		exit(0);
	}

	// Now do a round of the defined reordering
	do_a_round(&card_deck, &table_deck, deck_size);

	printf("Printing the deck after a round(Reverse Order)\n\r");
	//print_deck(&table_deck);


	// Now calculate the number of rounds required for the deck
	if ( (err = rounds_calculate(&table_deck, &orig_deck, deck_size, &rounds)) < 0) {
		printf("Error Finding the number of rounds: "); error_handler(err);
		exit(0);
	}
	printf ("\n\rNumber of rounds required = %lu\n\r", rounds);

}
