#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <openssl/md5.h>

int success = -1;

void init_key(char *key, char c, int s)
{	
	for (int i = 0; i < s - 1; i++)
		key[i] = c;
}

int backtracking(unsigned char *hash, char *alph, char *key, int size, int ind, int rank, int commsize)
{
	int s = strlen(alph);
	
	int lb = rank * s / commsize;
	int ub = (rank == commsize - 1) ? s : lb + s / commsize;
	
	for (int i = lb; i < ub; i++) {
		if (success == 1)
			break;
		
		if (key[ind] == alph[i]) {
			if (size > 1 && ind != size - 1)
				backtracking(hash, alph, key, size, ind + 1, j);
			continue;
		}

		key[ind] = alph[i];
		printf("%s\n", key);
		/*unsigned char md5digest[MD5_DIGEST_LENGTH];
		MD5(key, size, md5digest);
		if (strcmp(hash, md5digest) == 0) {
			printf("Found key: %s\n", key);
			success = 1;
			break;
		}*/
		*j += 1;
		
		if (ind == size - 1)
			continue;
		
		backtracking(hash, alph, key, size, ind + 1, j);
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
	int rank, commsize;
	
	MPI_Init(&argc, &argv);
	MPI_Commsize(MPI_COMM_WORLD, &commsize);
	MPI_Commrank(MPI_COMM_WORLD, &rank);
	
	int maxlength;
	char *key;
	
	maxlength = atoi(argv[1]);
	key = malloc(maxlength * sizeof(char));
	
	unsigned char hash[MD5_DIGEST_LENGTH];
	
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sscanf(&argv[2][2 * i], "%02x", &hash[i]);
	
	char *alph = argv[3];
	
	for (int i = 1; i <= maxlength; i++) {
		if (rank == 0)
			init_key(key, alph[0], i);
		backtracking(hash, alph, key, i, 0, rank, commsize);
	}
	
	MPI_Finalize();
	return 0;
}


