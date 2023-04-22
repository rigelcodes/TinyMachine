#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// integral constates
enum Constants {LOAD = 1, ADD = 2, STORE = 3, SUB = 4, IN = 5, OUT = 6, END = 7, JMP = 8, SKIPZ = 9};

//Structure for Instruction
typedef struct
{
    int opCode;//to store opcode
    int deviceOrAddress;//address of the operation
}Instruction;

// Global Variables:
int CODESIZE = -1;

//Tiny Machine Architecture variables
int pc = 10;    // counter for the program
int ir = 0;     // variable to be used in regard of instruction register
int mar = 0;    // variable to store address of memory
int dataMemory[9]={0,0,0,0,0,0,0,0,0}; // memory for the program
int mdr = 0;    // variable to store data of the memory
int ac = 0;     // variable to hold the accumulator value


//Prints Variables for Tiny Architecture
void printVaribles()
{
    int x;
    printf("\nPC = %d | A = %d | MEM = [", pc, ac);

    // for loop to print the [9] data from memory
    for (x = 0; x < 9; x += 1)
    {
        printf("%d,", dataMemory[x]);
    }
    printf("]\n");
}

// Counts the number of lines in a file
int numOfLineFile(FILE *inputfile) //asmCodefile
{
    // initalize variables
    char Buffer[10];
    int count = 0;

    // check for NULL
    if (inputfile == NULL)
    {
        printf("input file is not opening...");
        exit(0);
    }

    // while loop to track the number of lines and increment for each
    while (fgets(Buffer, 10, inputfile) != NULL)
    {
        count++;
    }
    fclose(inputfile);
    return count;

}

// Parses data (instructions) to the machine
void tinyMachineSimulator(int opCode, int b)
{   
    // switch statement to flip between different scenarios
    switch (opCode)
    {   
    // case that loads value from address
    case LOAD:  
        printf("\n/* Loading from address [%d]... */\n", b);
        ir = b;
        mar = ir;
        mdr = dataMemory[mar];
        ac= mdr;

        //increment the count and call print function
        pc += 1;
        printVaribles();

        printf("\n/* PC <- PC + 1 */\n");
        printf("/* PC <- PC + 1 */\n");
        printf("/* MAR <- IR.ADDR */\n");
        printf("/* MDR <- MEM[MAR] */\n");
        printf("/* A <- MDR */\n");
        break;

    // case that adds values held in ac
    case ADD:

        printf("\n/Adding accumulator and value obtain from address [%d]/\n", b);
        ir = b;
        mar = ir;
        mdr = dataMemory[mar];
        ac += mdr;

        // increment then call print 
        pc += 1;
        printVaribles();
        break;

    // case that stores input (ac) to the machine's memory
    case STORE:
        printf("\n/* storing accumulator to memory location 0 */\n");
        mdr = ac;
        ir = b;
        mar = ir;
        dataMemory[mar] = mdr;

        // increments value and calls print
        pc += 1;
        printVaribles();
        break;

    // case thaat subtracts a value from ac
    case SUB:
        printf("\n/* Subtracting memory address value [%d] from accumulator*/\n", b);
        ir = b;
        mar= ir;
        mdr = dataMemory[mar];
        ac -= mdr;

        // increment then call print
        pc += 1;
        printVaribles();
        break;

    // case regarding user input; needs a value
    case IN:
        printf("\n/* input value */\n");
        scanf("%d", &ac);

        // increment then call print
        pc += 1;
        printVaribles();
        break;

    // case to display the value currently held in the ac
    case OUT:
        printf("\n/* outputting accumulator to screen */\n");
        printf("%d\n", ac);

        // increment then print
        pc += 1;
        printVaribles();
        break;

    // case called upon completion of the program
    case END:
        printf("\nProgram complete ");
        exit(1);

    // case that will jump to address
    case JMP:
        printf("\n/Setting program counter to %d/\n", b);
        pc = b;

        // increment then call print
        printVaribles();
        break;

    // case to skip if ac is 0
    case SKIPZ:
        printf("\n/Skipping the next instruction/\n");
        
        // if ac = 0, skip the upcoming instruction
        if (ac == 0)
        {
            // pc is incremented by 2
            pc += 2;
        }

        else//otherwise
        {
            // pc is incremented by 1
            pc += 1;
        }

        // increment then call on print function
        printVaribles();
        break;

    // base case; if all other cases fail, default will print
    default:
        printf("\n/There was an error parsing that opcode! Exiting program/\n");
        exit(0);
    }

}

int main(int argc, char *argv[])
{
    // variable to hold the result from numOfLineFile
    CODESIZE = numOfLineFile(fopen(argv[1], "r"));

    // array of instructions to be stored
    Instruction programMemory[CODESIZE];

    // opens a file and reads every line
    FILE *inputfile = fopen(argv[1], "r");

    // initialize variables
    char Buffer[10];
    int x = 0;

    // check for NULL
    if (inputfile == NULL)
    {
        printf("input file is not opening...");
        exit(0);
    }

    // ensures each line from input file holds two values
    while (fgets(Buffer, sizeof(CODESIZE), inputfile) != NULL)
    {
        // check for empty space for buffer to skip
        if ((Buffer == " " || Buffer != " ") )
        {
            // checks for if there are double digits AND if there are not double digits
            if( isdigit(Buffer[0]) && (int)Buffer[2] != 0)
            {
                // takes and stores data in the array ProgramMemory, at element x
                programMemory[x].opCode = atoi(&Buffer[0]);
                programMemory[x].deviceOrAddress = atoi(&Buffer[2]);
                x++;
            }
        }
    }
    fclose(inputfile);

    // Display program greeting
    printf("\nTiny Machine Simulator\n");
    printf("Assembling program...\n");
    printf("Program assembled.\n");
    printf("Run.\n");

    // print out the beginning variables
    printVaribles();

    // opens programMemory to gather instructions
    for (x = (pc / 10) - 1; x < sizeof(programMemory); x += 1)
    {
        // Calls upon TMS and passes the programMemory array value to be parsed
        tinyMachineSimulator(programMemory[x].opCode, programMemory[x].deviceOrAddress);
    }
    printf(" Program concluded... ");
    return 0;

}