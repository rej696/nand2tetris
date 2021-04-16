#include <stdio.h>
#include <string.h>

void assemble_lines(FILE *input, FILE *output) {
    char a_flag;
    char a_instruction[255];
    char c_instruction[255];

    char line[255] = {0};

    // loop over characters in file until character is not whitespace
    while (fgets(line, 255, input)) {
    
    } 
    
    // scan for a_instruction
    fscanf(input, "@%s", a_instruction);

    // scan for 
}

char* split(char* input) {
    char delim[] = ".";
    char *str = "";
    strcpy(str, input);
    return strtok(str, delim);
}


int main(int argc, char *argv[]) {

    // Handle file if filename supplied as argument
    if(argc == 2) {

        FILE *input;
        FILE *output;

        // manipulate filename arg for .hack output file
        char *filename = split(argv[1]);

        char *input_name = strcat(filename, ".asm");
        char *output_name = strcat(filename, ".hack");

        // open input and output files
        input = fopen(input_name, "r");
        output = fopen(output_name, "w");

        // parse each line of input and write corresponding output line
        assemble_lines(input, output);

        // close input and output file
    } else {
        // print error if filename not supplied as an argument
        printf("Single filename argument not supplied");
    }
}

    
