#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

enum file_type {
	DATA,
	EMPTY,
	ASCII,
	ISO,
	UTF
};

const char * const FILE_TYPE_STRINGS[] = {
	"data",
	"empty",
	"ASCII text",
	"ISO-8859 text",
	"Unicode text, UTF-8 text"
};


int print_error(char *path, int errnum) {
	return fprintf(stdout, "%s: cannot determine (%s)\n",path,strerror(errnum));
}

int isEmpty(FILE *file) {
	int c = getc(file);
	if (c == EOF){
		return 1;
	} 
	ungetc(c,file);
	return 0;
}
int isASCII(FILE *file) {
	unsigned char c;
	while (fread(&c, sizeof(char), 1, file) == 1) {
		if ((c <= 6 || c >= 15) && (c != 27) && (c <= 31 || c >= 127)) {
			return 0;
		}	
	}
	return 1;
}
int isISO(FILE *file) {
	fseek(file,0,SEEK_SET);
	unsigned char c;
	while (fread(&c, sizeof(char), 1, file) == 1) {
		if ((c <= 6 || c >= 15) && (c != 27) && (c <= 31 || c >= 127)) {
			if (c <= 159 || c >= 255) {
				return 0;
			}	
		}
	}
	return 1;
}
int isUTF(FILE *file) {
	fseek(file,0,SEEK_SET);
	unsigned char c;
	int checkFollowBytes = 0;
	while (fread(&c, sizeof(char), 1, file) == 1) {	
        if (checkFollowBytes != 0) {
            if ((c >> 6) == 2) { //(10xxxxxx)
                // valid follow byte
                checkFollowBytes--;
				continue;
            } else {
                return 0; // Not valid followbyte: Not UTF-8 	
            }
		} else if ((c >> 7) == 0) { //(0xxxxxxx)
			continue;
        } else if ((c >> 5) == 6) { // (110xxxxx)
            checkFollowBytes = 1;
			continue;
        } else if ((c >> 4) == 14) { // (1110xxxx)
            checkFollowBytes = 2;
			continue;
        } else if ((c >> 3) == 30) { // (11110xxx)
            checkFollowBytes = 3;
			continue;
        } else {
            return 0; // Not UTF-8 
        }
    }
    return checkFollowBytes == 0 ? 1 : 0; // all followbytes should be checked
}
int fileType(FILE *file){
	if (isEmpty(file)){
		return EMPTY;
	}
	if (isASCII(file)) {
		return ASCII;
	}	
	if (isUTF(file)) {
		return UTF;
	}
	if (isISO(file)) {
		return ISO;
	}
	return DATA;
}
 
int main(int argc, char* argv[]) {
	FILE *file = fopen(argv[1],"r");
	if (argc != 2) {
		fprintf(stderr, "Usage: file path\n");
		return EXIT_FAILURE;
	} 

	if (file == NULL) {
		print_error(argv[1],errno);	
    	return EXIT_SUCCESS;
	}
	int ft = fileType(file);
	fprintf(stdout, "%s: %s\n",argv[1],FILE_TYPE_STRINGS[ft]);
	fclose(file);
	return EXIT_SUCCESS;		
}


