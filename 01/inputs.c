#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main() {
	int n_listeners;
	int n_antennas;
	int n_places;
	int x, y, r, c;

	FILE* fp = fopen("input.txt", "a");

	printf("Insira quantos listeners vai querer: ");
	scanf("%d", &n_listeners);

	printf("Insira quantos lugares para colocar antenas vai querer: ");
	scanf("%d", &n_places);

	printf("Insira quantos tipos de antenas diferentes vai querer: ");
	scanf("%d", &n_antennas);

	fprintf(fp, "%d\n", n_listeners);
	for (int i = 0; i < n_listeners; i++) {
		x = (rand() % (100));
		y = (rand() % (100));
		fprintf(fp, "%d %d\n", x, y);
	}
	fprintf(fp, "%d\n", n_places);
	for (int i = 0; i < n_places; i++) {
		x = (rand() % (100));
		y = (rand() % (100));
		fprintf(fp, "%d %d\n", x, y);
	}
	fprintf(fp, "%d\n", n_antennas);
	for (int i = 0; i < n_antennas; i++) {
		r = (rand() % (100)) + 1;
		c = (rand() % (1000)) + 1;
		fprintf(fp, "%d %d\n", r, c);
	}

	fclose(fp);

	

}