#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>

struct {
	char Name[30];
	char Surname[30];
	char University[20]; //Атрибут, отвечающий за ВУЗ, в котором учится человек;
	int Age;
	float Height; //Атрибут, отвечающий за рост человека;
	float GPA;
} Person;

#pragma pack(push, 1)
struct {
	char Name[30];
	char Surname[30];
	int Age;
	int GPA;
} CompressedPerson;
#pragma pack(pop)
int main() {
	printf("alignof Person %ld\n", alignof(Person));
	printf("alignof CompressedPerson %ld\n", alignof(CompressedPerson));

	return 0;
}
