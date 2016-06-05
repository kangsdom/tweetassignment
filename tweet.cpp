#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>	//qsort()
#include <string.h> // strcmp()

#define NELEM 10000
#define MAX 1024


//인풋을 받는 구조체
typedef struct Tuser {
	long long int id_number;
	char* screen_name;
	long long int numberOfFriend;
	long long int numberOfTweet;
} Tuser;
void Tuser_init(Tuser* self) {
	self->id_number = 0;
	self->screen_name = NULL;
	self->numberOfFriend = 0;
	self->numberOfTweet = 0;
}

typedef struct Tfriend {
	long long int user_id;
	long long int friend_id;
} Tfriend;
void Tfriend_init(Tfriend* self) {
	self->user_id = 0;
	self->friend_id = 0;
}

typedef struct	Tweet {
	long long int user_number;
	char* word;
} Tweet;
void Tweet_init(Tweet* self) {
	self->user_number = 0;
	self->word = NULL;
}

//word list 링크드리스트로 구현------------
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
			working->num+=1;
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
// word list 구현 끝-----------
int main() {
	//파일받기
	char* number1;
	char* number2;
	char* name;
	char* tweet;
	char* a;
	
	WordList* wordlist;


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
	

	while ((!feof(User)) && (TotalUser<NELEM)) {
		fgets(number1, MAX, User);
		fgets(a, MAX, User);
		fgets(name, MAX, User);
		fgets(a, MAX, User);	//빈 줄 받기
		Tuser_init(&tusers[TotalUser]);
		tusers[TotalUser].id_number = atoll(number1);
		tusers[TotalUser].screen_name = name;
		TotalUser++;
	}
	while ((!feof(Friend)) && (TotalFriend < NELEM)) {
		fgets(number1, MAX, Friend);
		fgets(number2, MAX, Friend);
		fgets(a, MAX, Friend);	//빈 줄 받기
		Tfriend_init(&tfriends[TotalFriend]);
		tfriends[TotalFriend].user_id = atoll(number1);
		tfriends[TotalFriend].friend_id = atoll(number2);
		TotalFriend++;
		for (int i = 0; i < TotalUser; i++) {
			if (tusers[TotalUser].id_number == atoll(number2)) {
				tusers[TotalUser].numberOfFriend += 1;
				break;
			}
		}//포문은 최악의 경우 친구관계가 추가될 때마다 n번 실행해야 하므로 개선 필요
	}
	ANF = TotalFriend / TotalUser;


	InitList();

	while ((!feof(Word)) && (TotalTweet<NELEM)) {
		fgets(number1, MAX, Word);
		fgets(a, MAX, Word);	//날짜 받기
		fgets(tweet, MAX, Word);
		fgets(a, MAX, Word);	//빈 줄 받기
		Tweet_init(&tweets[TotalTweet]);
		tweets[TotalTweet].user_number = atoll(number1);
		tweets[TotalTweet].word = tweet;
		TotalTweet++;
		for (int i = 0; i < TotalUser; i++) {
			if (tusers[i].id_number == atoll(number1)) {
				tusers[i].numberOfTweet += 1;
				break;
			}
		}
		InsertList(tweet);
		//포문은 최악의 경우 트윗이 추가될 때마다 n번 실행해야 하므로 개선 필요2
	}
	ANT = TotalTweet / TotalUser;
	fclose(Friend);
	fclose(User);
	fclose(Word);
	//파일받기 끝

	//qsort((void *)array, ELEMENTS, sizeof(array[0]), comparisonFunctionString);

	while(1){
		int menu = 0;
		{
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
		}//menu 출력
		scanf("%d", menu);

		if (menu == 0) {
			printf("Total users: %lld\n", TotalUser);
			printf("Total friendship records: %lld\n", TotalFriend);
			printf("Total tweets: %lld\n", TotalTweet);
		}
		if (menu == 1) {
			for (int i = 0; i < TotalUser; i++) {
				if (MaxNF < tusers[TotalUser].numberOfFriend) {
					MaxNF = tusers[TotalUser].numberOfFriend;
				}
				if (MinNF > tusers[TotalUser].numberOfFriend) {
					MinNF = tusers[TotalUser].numberOfFriend;
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

			printf("Average number of friends: %lld\n", ANF);
			printf("Minimum friends: %lld\n", MinNF);
			printf("Maximum friends: %lld\n\n", MaxNF);

			printf("Average tweets per user: %lld\n", ANT);
			printf("Minimum tweets per user: %lld\n", MinNT);
			printf("Maximum tweets per user: %lld\n", MaxNT);
		}
		if (menu == 2) {
			;
			
		}
		if (menu == 99)	return 0;
	}


	return 0;
}