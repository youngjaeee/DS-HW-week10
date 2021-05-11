/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>



typedef struct node { // 트리의 노드 구조체
	int key;
	struct node* left;
	struct node* right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20 // MAX_STACK_SIZE를 20으로 설정
Node* stack[MAX_STACK_SIZE]; // MAX_STACK_SIZE 만큼 노드 구조체 포인터 배열을 할당
int top = -1; // top = -1로 초기화

Node* pop(); // stack의 pop()함수 선언
void push(Node* aNode); // stack의 push()함수 선언

/* for queue */
#define MAX_QUEUE_SIZE		20 // MAX_QUEUE_SIZE를 20으로 설정
Node* queue[MAX_QUEUE_SIZE]; // MAX_QUEUE_SIZE만큼 노드 구조체 포인터 배열을 할당
int front = -1; // front = -1로 초기화
int rear = -1; // rear = -1로 초기화

Node* deQueue(); // queue의 deQueue() 함수 선언
void enQueue(Node* aNode); // queue의 enQueue() 함수 선언


int initializeBST(Node** h); // BST 초기화 함수 선언

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
void freeNode(Node* ptr);
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printStack(); // stack을 출력하는 함수 선언



int main()
{
	char command; // 사용자가 선택한 메뉴값 저장하는 변수
	int key; // 사용자가 탐색할 노드의 키값 저장하는 변수
	Node* head = NULL; // BST의 head = NULL로 초기화

	do {
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'z': case 'Z':
			initializeBST(&head); // BST 초기화 함수 호출
			break;
		case 'q': case 'Q':
			freeBST(head); // BST 할당 해제 함수 호출
			break;
		case 'i': case 'I':
			printf("Your Key = "); // 노드 삽입을 선택하였을 경우
			scanf("%d", &key); // 삽입할 key 값을 입력받아
			insert(head, key); // insert() 함수 호출
			break;
		case 'd': case 'D':
			printf("Your Key = "); // 노드 삭제를 선택였을 경우
			scanf("%d", &key); // 삭제할 key 값을 입력받아
			deleteNode(head, key); //deleteNode() 함수 실행
			break;

		case 'r': case 'R':
			recursiveInorder(head->left); // 재귀를 통한 중위순회 함수 호출
			break;
		case 't': case 'T':
			iterativeInorder(head->left); // 반복을 통한 중위순회 함수 호출
			break;

		case 'l': case 'L':
			levelOrder(head->left); // 레벨순서 순회함수 호출
			break;

		case 'p': case 'P':
			printStack(); // stack 출력 함수 호출
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if (*h != NULL) // NULL 트리가 아닌 경우
		freeBST(*h); // BST에 대한 할당 해제 함수 호출

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node)); // head node를 동적할당하여 생성
	(*h)->left = NULL;	/* root */ // *h->left를 NULL로 설정
	(*h)->right = *h; // *h->right를 자기 자신을 가리키게 설정
	(*h)->key = -9999; // key 값을 -9999로 초기화

	top = -1; // 반복을 통한 중위순회 함수에서 쓰일 stack의 top = -1로 초기화

	front = rear = -1; //  레벨순회 함수에서 쓰일 queue의 front, rear = -1로 초기화

	return 1;
}



void recursiveInorder(Node* ptr) // 재귀를 통한 중위순회 함수
{
	if (ptr) { //ptr != NULL 일 때
		recursiveInorder(ptr->left); //recursive로 좌측 자식에 대한 Inorder()함수 호출
		printf(" [%d] ", ptr->key); // ptr의 key 값 출력
		recursiveInorder(ptr->right); // recursive로 우측 자식에 대한 Inorder()함수 호출
	}
}

/**
 * textbook: p 224s
 */
void iterativeInorder(Node* node) // 반복을 통한 중외순회 함수
{
	int top = -1; // stack의 top = -1로 초기화
	for (;;) // 모든 노드의 탐색을 완료할 때까지 반복
	{
		for (; node; node = node->left) // root부터 반복하여 좌측 자식 노드까지
			push(node); // 노드를 반복하여 push함
		node = pop(); // for문을 돌아 단말 노드까지 push를 마친 경우 node = pop();

		if (!node) break; // node가 NULL일 경우 for문 벗어남

		printf("[%d] ", node->key); // node가 NULL이 아닐 경우 key값 출력
		node = node->right; // node가 우측 자식 노드 가리키게 하여 다시 이중 for문 실행
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	int front = -1;
	int rear = -1; // 레벨 순서 순회함수 동작을 위해 쓰이는 queue 초기화

	if (!ptr) return; // NULL BST일 경우 함수 종료
	enQueue(ptr); //queue에 ptr, 즉 root을 enqueue함

	for (;;) // 모든 노드에 대한 탐색을 마칠 때까지
	{
		ptr = deQueue(); // dequeue한 값을 ptr에 대입
		if (ptr) // ptr이 NULL이 아닐 경우
		{
			printf("[%d] ", ptr->key); // ptr의 key값 출력
			if (ptr->left)
				enQueue(ptr->left); // ptr에 좌측 자식 노드가 있을 경우 enqueue
			if (ptr->right)
				enQueue(ptr->right); // ptr에 우측 자식 노드가 있을 경우 enqueue
		}
		else break;
	}
}


int insert(Node* head, int key) // BST 삽입 함수, 매개변수로 head 포인터와 삽입할 노드의 key값 받음
{
	Node* newNode = (Node*)malloc(sizeof(Node)); // 새로운 노드에 대한 공간 동적할당
	newNode->key = key; // 동적할당한 노드의 key를 매개변수로 받은 key로 설정
	newNode->left = NULL; // 노드의 좌측 자식 노드 NULL 초기화
	newNode->right = NULL; // 노드의 우측 자식 노드 NULL 초기화

	if (head->left == NULL) { // BST가 NULL TREE인 경우
		head->left = newNode; // Root을 새로 할당한 노드로 설정
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left; // 노드 포인터 ptr이 BST의 Root을 가리키게 설정

	Node* parentNode = NULL; // 부모 노드 포인터 선언3
	while (ptr != NULL) {

		/* if there is a node for the key, then just return */
		if (ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if (ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if (parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key) // 입력한 key값과 동일한 값을 가지는 노드를 지우는 함수
{
	Node* ptr = head->left; // 노드 포인터 ptr 선언, root 가리킴
	Node* preptr = head; // ptr의 부모 포인터 preptr 선언 및 초기화

	while (ptr) // ptr != NULL일 동안
	{
		if (key == ptr->key) // 동일한 key값을 가지는 ptr 탐색을 성공했을 경우
			break; // while문 벗어남
		if (key < ptr->key) // 사용자가 입력한 key값이 ptr->key 보다 작은 경우
		{
			preptr = ptr; // preptr을 ptr로 설정하고
			ptr = ptr->left; // ptr을 ptr의 좌측 자식 노드를 가리키게 설정
		}
		else
		{
			preptr = ptr; // 사용자가 입력한 key값이 ptr->key 보다 큰 경우 preptr을 ptr로 설정하고
			ptr = ptr->right; // ptr을 ptr의 우측 자식 노드를 가리키게 설정
		}
	}
	if (ptr == NULL) // ptr이 NULL일 경우, 즉 탐색을 실패한 경우 오류 메시지 출력
	{
		printf("BST에서 입력한 값 탐색을 실패했습니다.\n");
		return 0;
	}

	if (ptr->left == NULL && ptr->right == NULL) // 탐색하여 찾은 ptr이 단말 노드일 경우
	{
		if (preptr->key > ptr->key) // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = NULL; // ptr은 preptr의 좌측 자식노드 이므로 preptr->left = NULL
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = NULL; // ptr은 preptr의 우측 자식노드 이므로 preptr->right = NULL
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
	}
	else if (ptr->left != NULL && ptr->right == NULL) // ptr이 좌측 자식 노드만 가지는 경우
	{
		if (preptr->key > ptr->key)  // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = ptr->left; // ptr은 preptr의 좌측 자식노드이므로 preptr->left = ptr->left
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = ptr->left; // ptr은 preptr의 우측 자식노드이므로 preptr->right = ptr->left
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
	}
	else if (ptr->left == NULL && ptr->right != NULL) // ptr이 우측 자식 노드만 가지는 경우
	{
		if (preptr->key > ptr->key)  // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = ptr->right; // ptr은 preptr의 좌측 자식노드이므로 preptr->left = ptr->right
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = ptr->right; // ptr은 preptr의 우측 자식노드이므로 preptr->right = ptr->right
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
	}

	else if (ptr->left != NULL && ptr->right != NULL) // ptr이 좌측, 우측 모두 자식 노드를 가질 경우
	{
		Node* minrightptr = ptr->right; // ptr의 우측 subtree에서 가장 작은 노드를 탐색하기 위한 노드 포인터 선언
		Node* preminrightptr = ptr; // 우측 subtree 중 최소값 가지는 노드의 부모 노드 포인터 선언

		while (1) // minrightptr이 NULL이 아닐 때까지
		{
			if (minrightptr->left != NULL) // 최소값을 찾기 위해 좌측 자식 노드 탐색 진행
			{
				preminrightptr = minrightptr; // minrightptr 값보다 작은 노드가 존재하는 경우 preminrightptr = minrightptr
				minrightptr = minrightptr->left; // minrightptr이 좌측 자식 노드 가리키게 함
			}
			else break; // 최소값을 찾은 경우 while문 벗어남
		}

		preminrightptr->left = minrightptr->left;
		minrightptr->left = ptr->left; // minrightptr의 좌측, 우측 자식노드를 기존 ptr의 자식노드로 재설정
		minrightptr->right = ptr->right;
		ptr->left = NULL;
		ptr->right = NULL;

		if (preptr->key > ptr->key)  // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = minrightptr; // ptr은 preptr의 좌측 자식노드이므로 preptr->left = minrightptr
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = minrightptr; // ptr은 preptr의 우측 자식노드이므로 preptr->right = minrightptr
			printf("preptr->right 자식노드: %d %d\n", preptr->right->left->key, preptr->right->right->key);
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}

	}


}


void printStack() // stack의 요소를 출력하는 함수
{
	if (top != -1)
	{
		for (int i = 0; i < top; i++)
			printf("[%d] ", stack[i]->key);
	}

}

void freeNode(Node* ptr) // ptr이 가리키는 노드에 대한 할당 해제 함수
{
	if (ptr) {
		freeNode(ptr->left); // 좌측 자식노드 포인터에 대한 할당 해제
		freeNode(ptr->right); // 우측 자식노드 포인터에 대한 할당 해제
		free(ptr); // ptr 할당 해제
	}
}

int freeBST(Node* head) // BST 초기화 함수
{

	if (head->left == head) //  head->left = head, 즉 NULL BST일 경우
	{
		free(head); // head 할당 해제
		return 1;
	}

	Node* p = head->left; // NULL BST가 아닐 경우 p를 BST의 root를 가리키게 설정

	freeNode(p); // p에 대하여 재귀적으로 freeNode()함수 실행

	free(head); // head 할당 해제
	return 1;
}



Node* pop() // stack에서의 pop()함수 구현
{
	if (top < 0)
		return NULL;
	else
	{
		return(stack[top--]);
	}
}

void push(Node* aNode) // stack에서의 push()함수 구현
{
	if (top >= MAX_STACK_SIZE - 1)
	{
		fprintf(stderr, "Stack is full, cannot add element\n"); // 최대 용량을 초과하였을 경우 오류메시지 출력
		exit(EXIT_FAILURE);
	}
	else
	{
		stack[++top] = aNode;
	}
}



Node* deQueue() // Queue의 delete 기능 함수
{
	if (front == rear)
		return NULL;
	return queue[++front];
}

void enQueue(Node* aNode) // Queue의 insert 기능 함수
{
	if (rear == MAX_QUEUE_SIZE - 1)
	{
		fprintf(stderr, "Queue is full, cannot add element\n");
		exit(EXIT_FAILURE);
	}
	queue[++rear] = aNode;

}

