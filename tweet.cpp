#include <stdio.h>
#include <stdlib.h>

#define NELEM 10000
#define MAX 1024

typedef struct Tuser {
	long long int id_number;
	char* screen_name;
} Tuser;

void Tuser_init(Tuser* self) {
	self->id_number = 0;
	self->screen_name = NULL;
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

int main() {
	char* number1;
	char* number2;
	char* name;
	char* tweet;
	char* a;
	
	FILE* Friend;
	FILE* User;
	FILE* Word;
	
	Tuser tusers[NELEM];
	Tfriend tfriends[NELEM];
	Tweet tweets[NELEM];
	
	Friend = fopen("friend.txt", "r+");
	User = fopen("user.txt", "r+");
	Word = fopen("word.txt", "r+");
	

	int i1 = 0;//user
	int i2 = 0;//friend
	int i3 = 0;//word

	while ((!feof(User)) && (i1<NELEM)) {
		fgets(number1, MAX, User);
		fgets(a, MAX, User);
		fgets(name, MAX, User);
		fgets(a, MAX, User);	//ºó ÁÙ ¹Þ±â
		Tuser_init(&tusers[i1]);
		tusers[i1].id_number = atoi(number1);
		tusers[i1].screen_name = name;
		i1++;
	}
	while ((!feof(Friend)) && (i2<NELEM)) {
		fgets(number1, MAX, Friend);
		fgets(number2, MAX, Friend);
		fgets(a, MAX, Friend);	//ºó ÁÙ ¹Þ±â
		Tfriend_init(&tfriends[i2]);
		tfriends[i2].user_id = atoi(number1);
		tfriends[i2].friend_id = atoi(number2);
		i2++;
	}
	while ((!feof(Word)) && (i3<NELEM)) {
		fgets(number1, MAX, Word);
		fgets(a, MAX, Word);
		fgets(tweet, MAX, Word);
		fgets(a, MAX, Word);	//ºó ÁÙ ¹Þ±â
		Tweet_init(&tweets[i3]);
		tweets[i3].user_number = atoi(number1);
		tweets[i3].word = tweet;
		i3++;
	}

	fclose(Friend);
	fclose(User);
	fclose(Word);


	return 0;
}