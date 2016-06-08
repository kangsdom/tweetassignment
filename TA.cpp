#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>	//qsort()
#include <string.h> // strcmp()

#define NELEM 10000 // ģ�����谡 �ʹ� ũ�� ������ ���� ����, ������ ������Ѱ�
					// txt������ �о� �迭�� �ֿ��� �� ���ٵ� 10000�̻��� ���̵� �������� ����
#define MAX 1024


//��ǲ�� �޴� ����ü
typedef struct Tuser {
	unsigned long long int id_number;
	char* screen_name;
	long long int numberOfFriend;
	long long int numberOfTweet;
} Tuser;
void Tuser_init(Tuser* self) {
	self->id_number = 0;
	self->screen_name = (char*)malloc(sizeof(char)*MAX);
	self->numberOfFriend = 0;
	self->numberOfTweet = 0;
}

typedef struct Tfriend {
	unsigned long long int user_id;
	unsigned long long int friend_id;
} Tfriend;
void Tfriend_init(Tfriend* self) {
	self->user_id = 0;
	self->friend_id = 0;
}

typedef struct	Tweet {
	unsigned long long int user_id;
	char* word;
} Tweet;
void Tweet_init(Tweet* self) {
	self->user_id = 0;
	self->word = (char*)malloc(sizeof(char)*MAX);;
}


//top 5 word, user ã�� ���� heap tree ����------------
struct TreeNode {
	unsigned long long int id_number;
	long long int idx;//�ڽ��� ���� ����
	long long int num;
	char* word= new char[50];
};
struct Tree {
	long long int idx = 1;
	TreeNode Array[NELEM];
};


Tree* pWordTree = new Tree;	//word tree pointer ����;
Tree* pUserTree = new Tree;	//user tree pointer ����;



int Compare_word(Tree* Target_Tree, long long int position)//word tree ���� �� ��
{
	long long int idxtemp;
	long long int temp;
	char* tempstr = (char*)malloc(sizeof(char)*MAX);

	if (Target_Tree->Array[position].num < Target_Tree->Array[position / 2].num || position == 1)//�θ� Ŭ��
		return 0;
	if (Target_Tree->Array[position].num > Target_Tree->Array[position / 2].num) {// �θ� ���� ��
		strcpy(tempstr, Target_Tree->Array[position].word);										//�ܾ� ��ü
		strcpy(Target_Tree->Array[position].word, Target_Tree->Array[position / 2].word);		//
		strcpy(Target_Tree->Array[position/2].word, tempstr);										//

		idxtemp = Target_Tree->Array[position].idx;												//������ü
		Target_Tree->Array[position].idx = Target_Tree->Array[position / 2].idx;				//
		Target_Tree->Array[position / 2].idx=idxtemp;											//

		temp = Target_Tree->Array[position].num;												//Ƚ����ü
		Target_Tree->Array[position].num = Target_Tree->Array[position / 2].num;				//
		Target_Tree->Array[position / 2].num = temp;											//

		return Compare_word(Target_Tree, position / 2);
	}
	return 0;
}

void Add_Tree_word(Tree* Target_Tree, char* word) {			//word tree ����
	if (Target_Tree->idx == 1) {							//Ʈ���� ����� ��� �׳� �ֱ�
		strcpy(Target_Tree->Array[Target_Tree->idx].word, word);
		Target_Tree->Array[Target_Tree->idx].num = 1;
		Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
		Target_Tree->idx += 1;
		return;
	}
	for (long long int i = 1; i < Target_Tree->idx; i++) {			//���� ���尡 ������ �ִ��� Ȯ��
		if ((strcmp(Target_Tree->Array[i].word, word))==0) {
			Target_Tree->Array[i].num += 1;						//������ Ƚ�� �߰�
			Compare_word(Target_Tree, Target_Tree->Array[i].idx);		//Ƚ���� �þ�� ������ �ٲ�����
			return;
		}
	}
												//������ �� �ؿ� �߰�
	strcpy(Target_Tree->Array[Target_Tree->idx].word, word);
	Target_Tree->Array[Target_Tree->idx].num = 1;
	Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
	Target_Tree->idx += 1;
}
int Compare_user(Tree* Target_Tree, long long int position)//user tree ���� �� ��
{
	long long int idxtemp;
	long long int temp;
	unsigned long long int tempid;

	if (Target_Tree->Array[position].num < Target_Tree->Array[position / 2].num || position == 1)//�θ� Ŭ��
		return 0;
	if (Target_Tree->Array[position].num > Target_Tree->Array[position / 2].num) {// �θ� ���� ��
		tempid = Target_Tree->Array[position].id_number;												//id ��ü
		Target_Tree->Array[position].id_number = Target_Tree->Array[position / 2].id_number;			//
		Target_Tree->Array[position/2].id_number= tempid;											//

		idxtemp = Target_Tree->Array[position].idx;												//������ü
		Target_Tree->Array[position].idx = Target_Tree->Array[position / 2].idx;				//
		Target_Tree->Array[position / 2].idx = idxtemp;											//

		temp = Target_Tree->Array[position].num;												//Ƚ����ü
		Target_Tree->Array[position].num = Target_Tree->Array[position / 2].num;				//
		Target_Tree->Array[position / 2].num = temp;											//

		return Compare_word(Target_Tree, position / 2);
	}
	return 0;
}

void Add_Tree_user(Tree* Target_Tree, unsigned long long int id) {			//user tree ����
	if (Target_Tree->idx == 1) {							//Ʈ���� ����� ��� �׳� �ֱ�
		Target_Tree->Array[Target_Tree->idx].id_number = id;
		Target_Tree->Array[Target_Tree->idx].num = 1;
		Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
		Target_Tree->idx += 1;
		return;
	}
	for (long long int i = 1; i < Target_Tree->idx; i++) {			//���� id�� ������ �ִ��� Ȯ��
		if (Target_Tree->Array[i].id_number==id) {
			Target_Tree->Array[i].num += 1;						//������ Ƚ�� �߰�
			Compare_user(Target_Tree, Target_Tree->Array[i].idx);		//Ƚ���� �þ�� ������ �ٲ�����
			return;
		}
	}
	//������ �� �ؿ� �߰�
	Target_Tree->Array[Target_Tree->idx].id_number = id;
	Target_Tree->Array[Target_Tree->idx].num = 1;
	Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
	Target_Tree->idx += 1;
}
//heap tree ���� ��	=	------------------

//word list ��ũ�帮��Ʈ�� ����------------
/*
struct WordList {
	char* word;
	long long int num;
	WordList* next;
};

WordList* head, *tail, *working;//����, ��, �۾���

void InitList() {
	head = NULL;
	tail = NULL;
	working = NULL;
}//�ʱ�ȭ

char* MostTweeted() {//���� ���� Ʈ���� �ܾ� ã��
	working = head;
	long long int max;
	WordList* MT;
	max = working->num;
	MT = working;
	while (working)
	{
		if (working->num >max)
		{
			max = working->num;
			MT = working;
		}
		working = working->next;
	}//�����Ѵٸ� num++
	return MT->word;
}

void InsertList(char* n)
{
	working = head; //
	int state = 0;  //�̹� �����ϴ��� Ȯ��

	while (working)
	{
		if (working->word == n)
		{
			working->num += 1;
			state = 1;
			break;
		}
		working = working->next;
	}//�����Ѵٸ� num++

	if (state == 0) {//�������ϸ� ����� ������
					 // ���γ�带�ϳ�������������
		working = (WordList *)malloc(sizeof(WordList));
		working->word = n;
		working->num = 1;

		// �̰��̲�����
		working->next = NULL;

		// ����Ӹ�����������̰��̸Ӹ���
		if (head == NULL)
		{
			head = working;
			tail = working;
		}
		else {
			// �Ӹ����ƴ϶�鸶��������带�����ϰ�
			tail->next = working;

			// �̰��̲�����
			tail = working;
		}

	}
}


void DeleteList(char* n)
{
	// �ƹ�������Ÿ��������
	if (head == NULL)
		// �����ϵ�����.
		return;

	// �ϴܸӸ��������´�.
	working = head;
	WordList* list;

	// ��������ã�Ƽ�����
	while (working)
	{
		if (working->word == n)
		{
			if (working == head)
			{
				head = working->next;
				delete working;
			}
			else
			{
				list->next = working->next;
				delete working;
			}
			break;
		}
		list = working;
		working = working->next;
	}
}
*/
// word list ���� ��----------- // search�� �����ɷ���, ���������� �����ؼ� ���

int main() {
	//���Ϲޱ�
	//working = (WordList *)malloc(sizeof(WordList));
	char* number1 = (char*)malloc(sizeof(char)*MAX);
	char* number2 = (char*)malloc(sizeof(char)*MAX);
	char* name = (char*)malloc(sizeof(char)*MAX);
	char* tweet = (char*)malloc(sizeof(char)*MAX);
	char* a = (char*)malloc(sizeof(char)*MAX);

	//WordList* wordlist;

	Tuser tusers[NELEM];
	Tfriend tfriends[NELEM];
	Tweet tweets[NELEM];

	FILE* Friend;
	FILE* User;
	FILE* Word;
	Friend = fopen("friend.txt", "r+");
	User = fopen("user.txt", "r+");
	Word = fopen("word.txt", "r+");

	long long int TotalUser = 0;//#ofuser
	long long int TotalFriend = 0;//#offriend
	long long int TotalTweet = 0;//#ofword

	long long int ANF = 0;//average number of friend.
	long long int MinNF = 0;//minimum number of friend.
	long long int MaxNF = 0;//maximum number of friend.
	long long int ANT = 0;//average number of tweet per user.
	long long int MinNT = 0;//minimum number of tweet per user.
	long long int MaxNT = 0;//maximum number of tweet per user.

	char* ull;//strtoul�� ���� ���� ���ǹ��� ����

	while ((!feof(User)) && (TotalUser<NELEM)) {
		fgets(number1, MAX, User); //id�ޱ�
		fgets(a, MAX, User);	//��¥�ޱ�
		fgets(name, MAX, User);	//�̸��ޱ�
		fgets(a, MAX, User);	//�� �� �ޱ�
		Tuser_init(&tusers[TotalUser]);
		tusers[TotalUser].id_number = strtoull(number1, &ull, 10);
		strcpy(tusers[TotalUser].screen_name, name);
		tusers[TotalUser].numberOfFriend = 0;
		TotalUser++;
	}
	while ((!feof(Friend)) && (TotalFriend < NELEM)) {
		fgets(number1, MAX, Friend);	//user id
		fgets(number2, MAX, Friend);	//friends id
		fgets(a, MAX, Friend);	//�� �� �ޱ�
		int state = 0;
		for (int i = 0; i < TotalUser; i++) {
			if (tusers[i].id_number == strtoull(number2, &ull, 10)) {//ģ���� ���� ���� Ȯ��
				tusers[i].numberOfFriend++;
				state = 1;
				break;
			}
		}
		if (state == 0);//�������� �ʴ� ������ ��� 
		else if (state == 1) {//�����ϴ� ������ ���
			Tfriend_init(&tfriends[TotalFriend]);
			tfriends[TotalFriend].user_id = strtoull(number1, &ull, 10);
			tfriends[TotalFriend].friend_id = strtoull(number2, &ull, 10);
			TotalFriend++;
		}
	}
	ANF = TotalFriend / TotalUser;

	//InitList();
	
	while ((!feof(Word)) && (TotalTweet<NELEM)) {
		fgets(number1, MAX, Word);
		fgets(a, MAX, Word);	//��¥ �ޱ�
		fgets(tweet, MAX, Word);
		fgets(a, MAX, Word);	//�� �� �ޱ�
		Tweet_init(&tweets[TotalTweet]);
		tweets[TotalTweet].user_id = strtoull(number1, &ull, 10);
		strcpy(tweets[TotalTweet].word, tweet);
		TotalTweet++;
		for (int i = 0; i < TotalUser; i++) {//������ Ʈ�� Ƚ�� ���ϱ�
			if (tusers[i].id_number == strtoull(number1, &ull, 10)) {
				tusers[i].numberOfTweet += 1;
				break;
			}
		}
		Add_Tree_user(pUserTree, strtoull(number1, &ull, 10));
		Add_Tree_word(pWordTree, tweet);
		//InsertList(tweet);
		//������ �־��� ��� Ʈ���� �߰��� ������ n�� �����ؾ� �ϹǷ� ���� �ʿ�2
	}
	ANT = TotalTweet / TotalUser;
	fclose(Friend);
	fclose(User);
	fclose(Word);
	//���Ϲޱ� ��

	//qsort((void *)array, ELEMENTS, sizeof(array[0]), comparisonFunctionString);

	while (1) {
		int menu = 0;
		printf("0. Read data files\n");
		printf("1. display statistics\n");
		printf("2. Top 5 most tweeted words\n");
		printf("3. Top 5 most tweeted users\n");
		printf("4. Find users who tweeted a word\n");
		printf("5. Find all people who are friends of the above users\n");
		printf("6. Delete all mentions of a word\n");
		printf("7. Delete all users who mentioned a word\n");
		printf("8. Find strongly connected components\n");
		printf("9. Find shortest path from a given user\n");
		printf("99. Quit\n");
		printf("Select Menu:\n");
		//menu ���
		scanf("%d", &menu);
		unsigned long long int heap[32];
		int UserListIdx=0;
		unsigned long long int userList[NELEM];

		if (menu == 0) {
			printf("\nTotal users: %lld\n", TotalUser);
			printf("Total friendship records: %lld\n", TotalFriend);
			printf("Total tweets: %lld\n\n", TotalTweet);
		}
		if (menu == 1) {
			MaxNF = 0;
			MinNF = ANF;
			for (int i = 0; i < TotalUser; i++) {
				if (MaxNF < tusers[i].numberOfFriend) {
					MaxNF = tusers[i].numberOfFriend;
				}
				if (MinNF > tusers[i].numberOfFriend) {
					MinNF = tusers[i].numberOfFriend;
				}
			}//������ �־��� ��� ģ�����谡 �߰��� ������ n�� �����ؾ� �ϹǷ� ���� �ʿ�
			for (int i = 0; i < TotalUser; i++) {
				if (MaxNT < tusers[i].numberOfTweet) {
					MaxNT = tusers[i].numberOfTweet;
				}
				if (MinNT > tusers[i].numberOfTweet) {
					MinNT = tusers[i].numberOfTweet;
				}
			}//������ �־��� ��� Ʈ���� �߰��� ������ n�� �����ؾ� �ϹǷ� ���� �ʿ�22

			printf("\nAverage number of friends: %lld\n", ANF);
			printf("Minimum friends: %lld\n", MinNF);
			printf("Maximum friends: %lld\n\n", MaxNF);

			printf("Average tweets per user: %lld\n", ANT);
			printf("Minimum tweets per user: %lld\n", MinNT);
			printf("Maximum tweets per user: %lld\n\n", MaxNT);
		}
		if (menu == 2) {
			for (int i = 1; i < 32; i++)
				heap[i] = (*pWordTree).Array[i].num;		//���� 5���� ��� 32���߿� �ϳ��̴�, ���� heap sort�� ���� �ʿ䰡 ����.

			for (int j = 0; j < 5; j++) {			//���� ū ���� ��󳻰� �� ���� ��� �� �� �� ���� 0���� ��ȯ, 5�� �ݺ�
				long long int max = 0;
				int maxidx = 0;
				for (int i = 1; i < 32; i++) {
					if (max < heap[i]) {
						max = heap[i];
						maxidx = i;
					}
				}
				printf("word: %stweeted %lld times\n\n", (*pWordTree).Array[maxidx].word, (*pWordTree).Array[maxidx].num);
				heap[maxidx] = 0;
			}
		}
		if (menu == 3) {
			for (int i = 1; i < 32; i++) {
				heap[i] = (*pUserTree).Array[i].num;		//���� 5���� ��� 32���߿� �ϳ��̴�, ���� heap sort�� ���� �ʿ䰡 ����.
			}
			for (int j = 0; j < 5; j++) {			//���� ū ���� ��󳻰� �� ���� ��� �� �� �� ���� 0���� ��ȯ, 5�� �ݺ�
				long long int max = 0;
				int maxidx = 0;
				for (int i = 1; i < 32; i++) {
					if (max < heap[i]) {
						max = heap[i];
						maxidx = i;
					}
				}
				printf("user ID: %llu  tweeted %lld times\n\n", (*pUserTree).Array[maxidx].id_number, (*pUserTree).Array[maxidx].num);
				heap[maxidx] = 0;
			}
		}
		if (menu == 4) {
			UserListIdx = 0;
			char a[] = "\n";
			char temp[128];
			printf("Type a word: ");
			scanf("%s", &temp);
			strcat(temp, a);
			printf("\n");
			for (int i = 0; i < TotalTweet; i++) {
				if (strcmp(tweets[i].word, temp)==0) {
					userList[UserListIdx] = tweets[i].user_id;
					UserListIdx += 1;
					printf("User id \"%llu\" tweeted\n\n", tweets[i].user_id);
				}
			}
			if (UserListIdx == 0) {
				printf("No user tweeted this word\n\n");
			}
		}
		if (menu == 5) {
			for (int i = 0; i < UserListIdx; i++) {
				userList[i];
				for (int j = 0; j < TotalFriend; j++) {
					;
				}
			}
		}
		if (menu == 99)	break;
	}


	return 0;
}