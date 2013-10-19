/* 
 *
 * Jaime Delgado
 * CDA3101
 * Project 1 - Part 2
 * Behavioral Simulator
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[]) {

    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
	int a, b, c;

    if (argc != 2) {
		printf("error: usage: %s <machine-code file>\n", argv[0]);
		exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
		printf("error: can't open file %s", argv[1]);
		perror("fopen");
		exit(1);
    }

	/* initializing all registers and the program counter to 0*/
	int i;
	for(i = 0; i < NUMREGS; i++) {
		state.reg[i] = 0;
	}

	state.pc = 0;
	printf("\n");
	
    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
		if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
			printf("error in reading address %d\n", state.numMemory);
			exit(1);
		}
		
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
	
	printf("\n");
	
	int opcode, field0, field1, field2;
	for(i = 0; i < state.numMemory; i++) {	
		state.pc = i;
		printState(&state);
		//printf("MEM: %d\n ",state.mem[i] );

		opcode = field0 = field1 = field2 = state.mem[i];

		opcode = opcode >> 22;
		
		switch(opcode) {
		
			/*add*/
			case 0:	
					field0 = field0 << 10;
					field0 = field0 >> 29;
					
					if(field0 < 1 || field0 > 7) {
						printf("invalid registers\n");
						exit(1);
					}					
					
					field1 = field1 << 13;
					field1 = field1 >> 29;
					
					if(field1 < 1 || field1 > 7) {
						printf("invalid registers\n");
						exit(1);
					}			
					
					field2 = field2 << 29;
					field2 = field2 >> 29;

					
					if(field2 < 1 || field2 > 7) {
						printf("invalid registers\n");
						exit(1);
					}
					
					a = state.reg[field0];
					b = state.reg[field1];
					state.reg[field2] = c = a + b;					

					break;
			/*nand*/
			case 1:	
					field0 = field0 << 10;
					field0 = field0 >> 29;

					if(field0 < 1 || field0 > 7) {
						printf("invalid registers\n");
						exit(1);
					}
					
					field1 = field1 << 13;
					field1 = field1 >> 29;
					if(field1 < 1 || field1 > 7) {
						printf("invalid registers\n");
						exit(1);
					}
					
					field2 = field2 << 29;
					field2 = field2 >> 29;

					if(field2 < 1 || field2 > 7) {
						printf("invalid registers\n");
						exit(1);
					}
					
					a = state.reg[field0];
					b = state.reg[field1];
					state.reg[field2] = c = ~(a & b);					
					
					break;
			/*lw*/
			case 2:	
					field0 = field0 << 10;
					field0 = field0 >> 29;
					
					field1 = field1 << 13;
					field1 = field1 >> 29;

					field2 = field2 << 16;
					field2 = field2 >> 16;
					//field2 = convertNum(field2 & (NUMMEMORY-1));
				
					//printf("field0 = %d, field1: %d, field2: %d\n", field0, field1, (i+field2));
					state.reg[field1] = state.mem[state.reg[field0] + (i+field2)];
					//printf("LOADING THIS: %d", state.mem[state.reg[field0] + field2]);					
					
					break;
			/*sw*/
			case 3:	
					field0 = field0 << 10;
					field0 = field0 >> 29;
					
					
					field1 = field1 << 13;
					field1 = field1 >> 29;
					
					field2 = field2 << 16;
					field2 = field2 >> 16;

					state.mem[state.reg[field0] + field2] = state.reg[field1];				
			
					break;
			/*beq*/
			case 4:	
					field0 = field0 << 10;
					field0 = field0 >> 29;

					field1 = field1 << 13;
					field1 = field1 >> 29;

					field2 = field2 << 16;
					field2 = field2 >> 16;

					if(state.reg[field0] == state.reg[field1]) {
							/*branch*/
							i = i + field2;
					} 
					
					break;
			/*cmov*/
			case 5:	
					field0 = field0 << 10;
					field0 = field0 >> 29;
					
					field1 = field1 << 13;
					field1 = field1 >> 29;
					
					field2 = field2 << 29;
					field2 = field2 >> 29;
					
					if(state.reg[field1] != 0)
						state.reg[field2] = field0;

					break;
			/*halt*/
			case 6:	
					break;
			/*noop*/
			case 7:	
					break;

			default:	printf("error - invalid opcode at line %d\n", i);
						break;
		
		}
		
		if(opcode == 6) {
			state.pc++;
			break;
		}
	}

	printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");

	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}

    printf("\tregisters:\n");

	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}

    printf("end state\n");
}

int convertNum(int num) {
	/* convert a 16-bit number into a 32-bit Sun integer */
	if (num & (1<<15) ) {
	    num -= (1<<16);
	}
	return(num);
}