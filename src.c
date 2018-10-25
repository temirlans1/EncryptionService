#include <stdio.h>
#include <stdlib.h>

void encrypt(FILE *fin, FILE *fout, char subarr[], char pass[]) {
	int p=0;
	while(!feof(fin)){
        char c=getc(fin);
        if(c==EOF){
            break;
        }
		int sum = (unsigned char) c + (unsigned char) pass[p];
		fprintf(fout, "%c", subarr[sum%254]);
		p++;
        if(p==strlen(pass))p=0;
	}
}

void decrypt(FILE *fin, FILE *fout, char subarr[], char pass[]) {
	int p=0;
	while(!feof(fin)){

		char c=getc(fin);
        if(c==EOF){
            break;
        }
        int dec_char=(unsigned char)subarr[(unsigned char)c]-pass[p];
        if(dec_char<0)dec_char=dec_char+254;
		fprintf(fout, "%c", dec_char);
		p++;
        if(p==strlen(pass))p=0;
	}
}

void read_key(FILE *key, char encode_key[], char decode_key[]) {
    int pos=0;
	while(!feof(key)){
		char c=getc(key);
        if(c==EOF){
            break;
        }
		encode_key[pos]=c;
		decode_key[(unsigned char)c]=pos;
		pos++;
	}
}

int main() {

	char encode_key[1000];
	char password[100];
	char decode_key[1000];

	setvbuf(stdout, NULL, _IONBF, 0);

    FILE *key;
	key = fopen("key.254", "r");
    read_key(key, encode_key, decode_key);

	printf("WELCOME to the encryption service. \n");

	printf("ENTER your password \n");
	scanf("%s", password);
	fflush(stdin);
    char text[100];
	while (text[0] != 'q') {

        printf("MENU: <e>ncode, <d>ecode, <q>uit \n");

		scanf(" %s", text);
		fflush(stdin);

		if (text[0] == 'e') {
			char filename[100];
			FILE *file1, *file2;
			printf("ENTER a file to encrypt \n");
			scanf(" %s", filename);
			fflush(stdin);

			file1 = fopen(filename, "r");

			if (!file1) {
				printf("CANNOT open %s.\n", filename);
				continue;
			}

			printf("ENTER a filename for the encrypted file. \n");
			scanf(" %s", filename);
			fflush(stdin);

			file2 = fopen(filename, "w");

			if (!file2) {
				printf("CANNOT open %s. \n", filename);
				continue;
			}

			encrypt(file1, file2, encode_key, password);

			fclose(file1);
			fclose(file2);
		}

		else if (text[0] == 'd') {

			char filename[100];
			FILE *file1, *file2;
			printf("ENTER a file to decrypt \n");
			scanf(" %s", filename);
			fflush(stdin);

			file1 = fopen(filename, "r");

			if (!file1) {
				printf("CANNOT open %s. \n", filename);
				continue;
			}

			printf("ENTER a filename for the decrypted file. \n");
			scanf("%s", filename);
			fflush(stdin);

			file2 = fopen(filename, "w");

			if (file2 == NULL) {
				printf("CANNOT open %s. \n", filename);
				continue;
			}

			decrypt(file1, file2, decode_key, password);

			fclose(file1);
			fclose(file2);
		}
		else if(text[0]=='q')break;
		else {
			if (text[0] != '/n') {
				printf("UNRECOGNIZED %c \n", text[0]);
			}
		}
	}
    printf("BYE!");
	return 0;
}
