#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <sys/time.h>

//int success = -1;

void init_key(char *key, char c, int s)
{	
	for (int i = 0; i < s - 1; i++)
		key[i] = c;
}

double wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int backtracking(unsigned char *hash, char *alph, char *key, int size, int ind)
{
	int s = strlen(alph);
	
	for (int i = 0; i < s; i++) {
		//if (success == 1)
			//break;
		
		if (key[ind] == alph[i]) {
			if (size > 1 && ind != size - 1)
				backtracking(hash, alph, key, size, ind + 1);
			continue;
		}

		key[ind] = alph[i];
		//printf("%s\n", key);
		unsigned char md5digest[MD5_DIGEST_LENGTH];
		MD5(key, size, md5digest);
		if (strncmp(hash, md5digest, MD5_DIGEST_LENGTH) == 0)
			printf("Found key: %s\n", key);
		
		if (ind == size - 1)
			continue;
		
		backtracking(hash, alph, key, size, ind + 1);
	}
	
	key[ind] = alph[0];
	
	return 0;
}
			
int main(int argc, char **argv) 
{
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <max_length> <hash> <alphabet>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/*if (strlen(argv[2]) < 32) {
		fprintf(stderr, "Length of hash can't be less than 32.\n");
		exit(EXIT_FAILURE);
	}*/
	
	int maxlength;
	char *key;
	
	maxlength = atoi(argv[1]);
	key = malloc(maxlength * sizeof(char));
	
	unsigned char hash[MD5_DIGEST_LENGTH];
	
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sscanf(&argv[2][2 * i], "%02x", &hash[i]);
	
	char *alph = argv[3];
	
	double t = wtime();
	for (int i = 1; i <= maxlength; i++) {
		init_key(key, alph[0], i);
		backtracking(hash, alph, key, i, 0);
	}
	t = wtime() - t;
	printf("t = %.6f\n", t);
	return 0;
}



