#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>	//qsort()
#include <string.h> // strcmp()

#define NELEM 10000 // 친구관계가 너무 크게 들어오면 받지 못함, 본인의 기술적한계
					// txt파일을 읽어 배열에 넣여야 뭘 할텐데 10000이상은 아이디어가 떠오르지 않음
#define MAX 1024


//인풋을 받는 구조체
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


//top 5 word, user 찾기 위해 heap tree 구현------------
struct TreeNode {
	unsigned long long int id_number;
	long long int idx;//자신의 순서 저장
	long long int num;
	char* word= new char[50];
};
struct Tree {
	long long int idx = 1;
	TreeNode Array[NELEM];
};


Tree* pWordTree = new Tree;	//word tree pointer 선언;
Tree* pUserTree = new Tree;	//user tree pointer 선언;



int Compare_word(Tree* Target_Tree, long long int position)//word tree 삽입 후 비교
{
	long long int idxtemp;
	long long int temp;
	char* tempstr = (char*)malloc(sizeof(char)*MAX);

	if (Target_Tree->Array[position].num < Target_Tree->Array[position / 2].num || position == 1)//부모가 클때
		return 0;
	if (Target_Tree->Array[position].num > Target_Tree->Array[position / 2].num) {// 부모가 작을 때
		strcpy(tempstr, Target_Tree->Array[position].word);										//단어 교체
		strcpy(Target_Tree->Array[position].word, Target_Tree->Array[position / 2].word);		//
		strcpy(Target_Tree->Array[position/2].word, tempstr);										//

		idxtemp = Target_Tree->Array[position].idx;												//순서교체
		Target_Tree->Array[position].idx = Target_Tree->Array[position / 2].idx;				//
		Target_Tree->Array[position / 2].idx=idxtemp;											//

		temp = Target_Tree->Array[position].num;												//횟수교체
		Target_Tree->Array[position].num = Target_Tree->Array[position / 2].num;				//
		Target_Tree->Array[position / 2].num = temp;											//

		return Compare_word(Target_Tree, position / 2);
	}
	return 0;
}

void Add_Tree_word(Tree* Target_Tree, char* word) {			//word tree 삽입
	if (Target_Tree->idx == 1) {							//트리가 비엇을 경우 그냥 넣기
		strcpy(Target_Tree->Array[Target_Tree->idx].word, word);
		Target_Tree->Array[Target_Tree->idx].num = 1;
		Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
		Target_Tree->idx += 1;
		return;
	}
	for (long long int i = 1; i < Target_Tree->idx; i++) {			//먼저 워드가 들어온적 있는지 확인
		if ((strcmp(Target_Tree->Array[i].word, word))==0) {
			Target_Tree->Array[i].num += 1;						//있으면 횟수 추가
			Compare_word(Target_Tree, Target_Tree->Array[i].idx);		//횟수가 늘어나면 순서가 바뀌어야함
			return;
		}
	}
												//없으면 맨 밑에 추가
	strcpy(Target_Tree->Array[Target_Tree->idx].word, word);
	Target_Tree->Array[Target_Tree->idx].num = 1;
	Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
	Target_Tree->idx += 1;
}
int Compare_user(Tree* Target_Tree, long long int position)//user tree 삽입 후 비교
{
	long long int idxtemp;
	long long int temp;
	unsigned long long int tempid;

	if (Target_Tree->Array[position].num < Target_Tree->Array[position / 2].num || position == 1)//부모가 클때
		return 0;
	if (Target_Tree->Array[position].num > Target_Tree->Array[position / 2].num) {// 부모가 작을 때
		tempid = Target_Tree->Array[position].id_number;												//id 교체
		Target_Tree->Array[position].id_number = Target_Tree->Array[position / 2].id_number;			//
		Target_Tree->Array[position/2].id_number= tempid;											//

		idxtemp = Target_Tree->Array[position].idx;												//순서교체
		Target_Tree->Array[position].idx = Target_Tree->Array[position / 2].idx;				//
		Target_Tree->Array[position / 2].idx = idxtemp;											//

		temp = Target_Tree->Array[position].num;												//횟수교체
		Target_Tree->Array[position].num = Target_Tree->Array[position / 2].num;				//
		Target_Tree->Array[position / 2].num = temp;											//

		return Compare_word(Target_Tree, position / 2);
	}
	return 0;
}

void Add_Tree_user(Tree* Target_Tree, unsigned long long int id) {			//user tree 삽입
	if (Target_Tree->idx == 1) {							//트리가 비엇을 경우 그냥 넣기
		Target_Tree->Array[Target_Tree->idx].id_number = id;
		Target_Tree->Array[Target_Tree->idx].num = 1;
		Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
		Target_Tree->idx += 1;
		return;
	}
	for (long long int i = 1; i < Target_Tree->idx; i++) {			//먼저 id가 들어온적 있는지 확인
		if (Target_Tree->Array[i].id_number==id) {
			Target_Tree->Array[i].num += 1;						//있으면 횟수 추가
			Compare_user(Target_Tree, Target_Tree->Array[i].idx);		//횟수가 늘어나면 순서가 바뀌어야함
			return;
		}
	}
	//없으면 맨 밑에 추가
	Target_Tree->Array[Target_Tree->idx].id_number = id;
	Target_Tree->Array[Target_Tree->idx].num = 1;
	Target_Tree->Array[Target_Tree->idx].idx = Target_Tree->idx;
	Target_Tree->idx += 1;
}
//heap tree 구현 끝	=	------------------

//word list 링크드리스트로 구현------------
/*
struct WordList {
	char* word;
	long long int num;
	WordList* next;
};

WordList* head, *tail, *working;//시작, 끝, 작업중

void InitList() {
	head = NULL;
	tail = NULL;
	working = NULL;
}//초기화

char* MostTweeted() {//가장 많이 트윗된 단어 찾기
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
	}//존재한다면 num++
	return MT->word;
}

void InsertList(char* n)
{
	working = head; //
	int state = 0;  //이미 존재하는지 확인

	while (working)
	{
		if (working->word == n)
		{
			working->num += 1;
			state = 1;
			break;
		}
		working = working->next;
	}//존재한다면 num++

	if (state == 0) {//안존재하면 여기로 들어오기
					 // 새로노드를하나만들어서값을대입
		working = (WordList *)malloc(sizeof(WordList));
		working->word = n;
		working->num = 1;

		// 이것이꼬리임
		working->next = NULL;

		// 만약머리가비었으면이것이머리임
		if (head == NULL)
		{
			head = working;
			tail = working;
		}
		else {
			// 머리가아니라면마지막에노드를삽입하고
			tail->next = working;

			// 이것이꼬리임
			tail = working;
		}

	}
}


void DeleteList(char* n)
{
	// 아무런데이타도없으면
	if (head == NULL)
		// 지울일도없다.
		return;

	// 일단머리를가져온다.
	working = head;
	WordList* list;

	// 같은값을찾아서삭제
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
// word list 구현 끝----------- // search가 오래걸려서, 여러가지로 불편해서 폐기

int main() {
	//파일받기
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

	char* ull;//strtoul을 위해 사용된 무의미한 변수

	while ((!feof(User)) && (TotalUser<NELEM)) {
		fgets(number1, MAX, User); //id받기
		fgets(a, MAX, User);	//날짜받기
		fgets(name, MAX, User);	//이름받기
		fgets(a, MAX, User);	//빈 줄 받기
		Tuser_init(&tusers[TotalUser]);
		tusers[TotalUser].id_number = strtoull(number1, &ull, 10);
		strcpy(tusers[TotalUser].screen_name, name);
		tusers[TotalUser].numberOfFriend = 0;
		TotalUser++;
	}
	while ((!feof(Friend)) && (TotalFriend < NELEM)) {
		fgets(number1, MAX, Friend);	//user id
		fgets(number2, MAX, Friend);	//friends id
		fgets(a, MAX, Friend);	//빈 줄 받기
		int state = 0;
		for (int i = 0; i < TotalUser; i++) {
			if (tusers[i].id_number == strtoull(number2, &ull, 10)) {//친구인 유저 존재 확인
				tusers[i].numberOfFriend++;
				state = 1;
				break;
			}
		}
		if (state == 0);//존재하지 않는 유저일 경우 
		else if (state == 1) {//존재하는 유저일 경우
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
		fgets(a, MAX, Word);	//날짜 받기
		fgets(tweet, MAX, Word);
		fgets(a, MAX, Word);	//빈 줄 받기
		Tweet_init(&tweets[TotalTweet]);
		tweets[TotalTweet].user_id = strtoull(number1, &ull, 10);
		strcpy(tweets[TotalTweet].word, tweet);
		TotalTweet++;
		for (int i = 0; i < TotalUser; i++) {//유저별 트윗 횟수 더하기
			if (tusers[i].id_number == strtoull(number1, &ull, 10)) {
				tusers[i].numberOfTweet += 1;
				break;
			}
		}
		Add_Tree_user(pUserTree, strtoull(number1, &ull, 10));
		Add_Tree_word(pWordTree, tweet);
		//InsertList(tweet);
		//포문은 최악의 경우 트윗이 추가될 때마다 n번 실행해야 하므로 개선 필요2
	}
	ANT = TotalTweet / TotalUser;
	fclose(Friend);
	fclose(User);
	fclose(Word);
	//파일받기 끝

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
		//menu 출력
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
			}//포문은 최악의 경우 친구관계가 추가될 때마다 n번 실행해야 하므로 개선 필요
			for (int i = 0; i < TotalUser; i++) {
				if (MaxNT < tusers[i].numberOfTweet) {
					MaxNT = tusers[i].numberOfTweet;
				}
				if (MinNT > tusers[i].numberOfTweet) {
					MinNT = tusers[i].numberOfTweet;
				}
			}//포문은 최악의 경우 트윗이 추가될 때마다 n번 실행해야 하므로 개선 필요22

			printf("\nAverage number of friends: %lld\n", ANF);
			printf("Minimum friends: %lld\n", MinNF);
			printf("Maximum friends: %lld\n\n", MaxNF);

			printf("Average tweets per user: %lld\n", ANT);
			printf("Minimum tweets per user: %lld\n", MinNT);
			printf("Maximum tweets per user: %lld\n\n", MaxNT);
		}
		if (menu == 2) {
			for (int i = 1; i < 32; i++)
				heap[i] = (*pWordTree).Array[i].num;		//상위 5개는 적어도 32개중에 하나이다, 따라서 heap sort를 해줄 필요가 없다.

			for (int j = 0; j < 5; j++) {			//가장 큰 것을 골라내고 그 값을 출력 한 후 그 값을 0으로 반환, 5번 반복
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
				heap[i] = (*pUserTree).Array[i].num;		//상위 5개는 적어도 32개중에 하나이다, 따라서 heap sort를 해줄 필요가 없다.
			}
			for (int j = 0; j < 5; j++) {			//가장 큰 것을 골라내고 그 값을 출력 한 후 그 값을 0으로 반환, 5번 반복
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