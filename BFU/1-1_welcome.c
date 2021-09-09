#include <stdio.h>


int main(){
	int year, month, day;
	float height;
	char name[21];

	printf("Name: ");
	scanf(" %s", name);

	printf("DoB (yy-mm-dd): ");
	scanf("     %d-%d-%d", &year, &month, &day);

	printf("Längd (m): ");
	scanf("%f", &height);

	printf("\n\n Hej %s, your info:", name);
	printf("\n DoB: %4d-%02d-%02d", year, month, day);
	printf("\n Längd: %4.2f meter", height);

	return 0;


}