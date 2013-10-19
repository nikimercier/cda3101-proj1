/* 
 *
 * Jaime Delgado
 * CDA3101
 * Project 1 - Part 1
 * LC3101 Assembly Language and Assembler
 *
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536
#define MAXLINELENGTH 1000

/* struct to hold lable name and line*/
typedef struct labelStruct{
	char label_name[MAXLINELENGTH];
	int label_address;
} labelType;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int getLabelAddress(char *, labelType list[20], int);


int main(int argc, char *argv[]) {

    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	labelType label_list[200];
	int opbit, regA, regB, destReg, offset, result;
	int label_counter = 0;
	int line_counter = 0;

	/* check argument counter */
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
        exit(1);
    }

	/* set argument file names */
    inFileString = argv[1];
    outFileString = argv[2];

	/* open input file (read only) */
    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
	
	/* open output file (write only) */
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    while( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
		/* check the label for errors*/
		char tmp[2];
		tmp[0] = label[0];

		if(strlen(label) > 6) {
	 		printf("error with label size, must be no more than 6 characters\n");
			exit(1);
		} else if (isNumber(tmp)) {
			printf("error with label, must start with a letter\n");
			exit(1);
		} else if(strcmp(label, "\0")) {
			/* if no errors, save label name and address */
			int i;
			int j = 0;
			for(i = 0; i < 200; i++) {
				if(!strcmp(label_list[i].label_name, label)) {
					printf("error with label on line %d: duplicate label\n", line_counter);
					exit(1);
				}
			}

			strcpy(label_list[label_counter].label_name, label);
			label_list[label_counter].label_address = line_counter;
						
			label_counter++;
			
		} else {
			/*printf("no label on this line\n");*/
		}

		line_counter++;

	   /* reached end of file */
    }

    rewind(inFilePtr);
		
	line_counter = 0;
    while( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {	

    	if (!strcmp(opcode, "add")) {
        	/* opcode "add" */
			opbit = 0;
			opbit = opbit << 22;
			
			if(isNumber(arg0)) 
				regA = atoi(arg0);	
			else
				regA = getLabelAddress(arg0, label_list, line_counter);
				
			regA = regA << 19;		
			
			if(isNumber(arg1)) 
				regB = atoi(arg1);	
			else
				regB = getLabelAddress(arg1, label_list, line_counter);
				
			regB = regB << 16;			
			
			if(isNumber(arg2)) 
				destReg = atoi(arg2);	
			else
				destReg = getLabelAddress(arg2, label_list, line_counter);

			result = opbit | regB | regA | destReg;
			fprintf(outFilePtr, "%d\n", result);
			
    	} else if (!strcmp(opcode, "nand")) {
        	/* opcode "nand" */
			opbit = 1;
			opbit = opbit << 22;
			
			if(isNumber(arg0))
				regA = atoi(arg0);
			else
				regA = getLabelAddress(arg0, label_list, line_counter);

			regA = regA << 19;		

			if(isNumber(arg1)) 
				regB = atoi(arg1);	
			else
				regB = getLabelAddress(arg1, label_list, line_counter);

			regB = regB << 16;			
			
			if(isNumber(arg2)) 
				destReg = atoi(arg2);	
			else
				destReg = getLabelAddress(arg2, label_list, line_counter);
				
			result = opbit | regA | regB | destReg;
			fprintf(outFilePtr, "%d\n", result);	

    	} else if (!strcmp(opcode, "lw")) {
	        /* opcode "lw" */
			opbit = 2;
			opbit = opbit << 22;

			if(isNumber(arg0)) 
				regA = atoi(arg0);	
			else
				regA = getLabelAddress(arg0, label_list, line_counter);

			regA = regA << 19;		
			
			if(isNumber(arg1)) 
				regB = atoi(arg1);	
			else
				regB = getLabelAddress(arg1, label_list, line_counter);

			regB = regB << 16;			
			
			if(isNumber(arg2)) {
				offset = atoi(arg2);
			}	
			else {
				offset = getLabelAddress(arg2, label_list, line_counter);
				offset = offset - line_counter;
			}
			

			if( offset > 32767 || offset < -32768) {
				printf("error: out of range offset\n");
				exit(1);
			}
	
			offset = offset & (NUMMEMORY - 1);

			result = opbit | regA | regB | offset;
			fprintf(outFilePtr, "%d\n", result);
			
    	} else if (!strcmp(opcode, "sw")) {
	        /* opcode "sw" */
			opbit = 3;
			opbit = opbit << 22;

			if(isNumber(arg0)) 
				regA = atoi(arg0);	
			else
				regA = getLabelAddress(arg0, label_list, line_counter);

			regA = regA << 19;		
			
			if(isNumber(arg1)) 
				regB = atoi(arg1);	
			else
				regB = getLabelAddress(arg1, label_list, line_counter);

			regB = regB << 16;			
			
			if(isNumber(arg2)) {
				offset = atoi(arg2);
			}
			else {
				offset = getLabelAddress(arg2, label_list, line_counter);
				offset = offset - line_counter;
			}	

			if( offset > 32767 || offset < -32768) {
				printf("error: out of range offset\n");
				exit(1);
			}
			
			offset = offset & (NUMMEMORY - 1);			

			result = opbit | regA | regB | offset;
			fprintf(outFilePtr, "%d\n", result);
			
	    } else if (!strcmp(opcode, "beq")) {
	        /* opcode "beq" */
			opbit = 4;
			opbit = opbit << 22;

			if(isNumber(arg0)) 
				regA = atoi(arg0);	
			else
				regA = getLabelAddress(arg0, label_list, line_counter);

			regA = regA << 19;		
			
			if(isNumber(arg1)) 
				regB = atoi(arg1);	
			else
				regB = getLabelAddress(arg1, label_list, line_counter);
	
			regB = regB << 16;			
			
			if(isNumber(arg2)) {
				offset = atoi(arg2);
			}	
			else {
				offset = getLabelAddress(arg2, label_list, line_counter);
				offset = offset - line_counter - 1;
			}

			if( offset > 32767 || offset < -32768) {
				printf("error: out of range offset\n");
				exit(1);
			}
			
			offset = offset & (NUMMEMORY - 1);

			result = opbit | regA | regB | offset;
			fprintf(outFilePtr, "%d\n", result);		

	    } else if (!strcmp(opcode, "cmov")) {
	        /* opcode "cmov" */
			opbit = 5;	
			result = opbit = opbit << 22;

			fprintf(outFilePtr, "%d\n", result);			
			
	    } else if (!strcmp(opcode, "halt")) {
	        /* opcode "halt" */
			opbit = 6;	
			result = opbit = opbit << 22;

			fprintf(outFilePtr, "%d\n", result);	
			
	    } else if (!strcmp(opcode, "noop")) {
	        /* opcode "noop" */
			opbit = 7;
			result = opbit = opbit << 22;

			fprintf(outFilePtr, "%d\n", result);
			
		} else if (!strcmp(opcode, ".fill")) {
			/* checking for .fill */
			if(isNumber(arg0)) {
				result = atoi(arg0);
			} else {
				int i;
				int j = 0;
				for(i = 0; i < 200; i++) {
					if(!strcmp(label_list[i].label_name, arg0)) {
						result = label_list[i].label_address;
						j = 1;
						break;
					}
				}
				
				if(j == 0) {
					printf("error with .fill on line %d, not a valid argument\n", line_counter);
					exit(1);
				}
			}

			fprintf(outFilePtr, "%d\n", result);
			
		} else {
			printf("error: invalid opcode on line %d\n", line_counter);
			exit(1);
		}


		line_counter++;

        /* reached end of file */
	}
	
    return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {

    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

int isNumber(char *string) {

    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}

int getLabelAddress(char *arg_label, labelType list[200], int lc) {

	int j = 0;
	int i, la, s;
	for(i = 0; i < 200; i++) {
		//printf("CURRENT: %s | SEARCHING: %s\n", list[i].label_name, arg_label);
		if(!strcmp(list[i].label_name, arg_label)) {
			//printf("FOUND\n");
			la = list[i].label_address;
			j = 1;
			break;
		}
	}
	
	if(j == 0) {
		printf("error on line %d: undefined label\n", lc);
		exit(1);
	}
	
	return la;
}
