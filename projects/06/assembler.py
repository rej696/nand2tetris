import sys

# C Instruction Look Up Tables

COMP_COMMANDS = {
    "0": "0101010",
    "1": "0111111",
    "-1": "0111010",
    "D": "0001100",
    "A": "0110000",
    "M": "1110000",
    "!D": "0001101",
    "!A": "0110001",
    "!M": "1110001",
    "-D": "0001111",
    "-A": "0110011",
    "-M": "1110011",
    "D+1": "0011111",
    "A+1": "0110111",
    "M+1": "1110111",
    "D-1": "0001110",
    "A-1": "0110010",
    "M-1": "1110010",
    "D+A": "0000010",
    "D+M": "1000010",
    "D-A": "0010011",
    "D-M": "1010011",
    "A-D": "0000111",
    "M-D": "1000111",
    "D&A": "0000000",
    "D&M": "1000000",
    "D|A": "0010101",
    "D|M": "1010101"
}

DEST_COMMANDS = {
    "": "000",
    "M": "001",
    "D": "010",
    "MD": "011",
    "A": "100",
    "AM": "101",
    "AD": "110",
    "AMD": "111"
}

JUMP_COMMANDS = {
    "": "000",
    "JGT": "001",
    "JEQ": "010",
    "JGE": "011",
    "JLT": "100",
    "JNE": "101",
    "JLE": "110",
    "JMP": "111"
}


# Define Exceptions
class Error(Exception):
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)

class AssemblerSyntaxError(Error):
    """Incorrect syntax encountered when parsing assembly command"""
    pass

class ArgumentError(Error):
    """Incorrect arguments encountered by the assembler tool"""
    pass


# Define command parsing functions

def get_c_instruction(dest_command, comp_command, jump_command):
    try:
        dest = DEST_COMMANDS[dest_command]
    except KeyError:
        raise AssemblerSyntaxError(f"{dest_command} is invalid syntax for destination command")

    try:
        comp = COMP_COMMANDS[comp_command]
    except KeyError:
        raise AssemblerSyntaxError(f"{comp_command} is invalid syntax for computation command")

    try:
        jump = JUMP_COMMANDS[jump_command]
    except KeyError:
        raise AssemblerSyntaxError(f"{jump_command} is invalid syntax for jump command")

    return f'111{comp}{dest}{jump}'


def parse_c_instruction(command):
    comp_command = ""
    dest_command = ""
    jump_command = ""
    jump_next = False

    for char in command:
        if jump_next:
            jump_command += char
        elif char == "=":
            dest_command = comp_command
            comp_command = ""
        elif char == ";":
            jump_next = True
        else:
            comp_command += char

    return get_c_instruction(dest_command, comp_command, jump_command)


def get_a_instruction_bitstring(value: int) -> str:
    fifteen = int('111111111111111', 2)
    if value > fifteen:
        value = fifteen
    return "0" + format(value, 'b').zfill(15)


def parse_a_instruction(command: str, symbol_table: dict, variable_count: int):
    if command in symbol_table:
        # predefined symbols, labels and existing variable a instructions
        value = symbol_table[command]
    elif command.isdigit():
        # integer a instructions
        value = int(command)
    else:
        # new variable
        value = 16 + variable_count
        symbol_table[command] = value
        variable_count += 1

    return get_a_instruction_bitstring(value), symbol_table, variable_count


def translate_commands(command_list, symbol_table, output_file):
    variable_count = 0
    for command in command_list:
        if command.startswith("@"):
            bitstring, symbol_table, variable_count = parse_a_instruction(
                command.strip("@"), symbol_table, variable_count)
        else:
            bitstring = parse_c_instruction(command)
        
        output_file.write(f"\n{bitstring}")


# Define assembler preprocessing functions

# def resolve_variables(command_list, symbol_table)
#     variable_count = 0
#     for command in command_list:
#         if command.startswith("@"):
#             address = command.strip("@")
#             if not instruction.isdigit() or address not in symbol_table:
#                 # assign an address to the new variable in the symbol table
#                 symbol_table[address] = 16 + variable_count
#                 variable_count += 1
# 
#     return symbol_table


def parse_lines(input_file, symbol_table):
    command_list = []

    for line in input_file.readlines():

        if line.split():
            command = line.split()[0]
        else:
            continue

        if command == "//" or "":
            # Found a comment, don't parse beyond this point.
            pass

        elif command.startswith("@"):
            command_list.append(command)

        elif command.startswith("("):
            symbol = command.strip("()")
            if symbol in symbol_table:
                # Syntax Error, need distinct symbols
                raise AssemblerSyntaxError(f"Label symbol {symbol} has been previously defined")

            # set the value of the symbol to be the current length of the command list
            symbol_table[symbol] = len(command_list)
        
        else:
            command_list.append(command)

    return command_list, symbol_table
            

def initialise_symbol_table():
    # Initialise symbol table with predefined symbols
    symbol_table = {
        "SCREEN": 16384,
        "KBD": 24576,
        "SP": 0,
        "LCL": 1,
        "ARG": 2,
        "THIS":3,
        "THAT":4
    }

    for i in range(16):
        symbol_table[f"R{i}"] = i
    
    return symbol_table


def parse_file(input_file):
    symbol_table = initialise_symbol_table()

    # extract each line in the file
    command_list, symbol_table = parse_lines(input_file, symbol_table)

#     # add variables to symbol table
#     symbol_table = resolve_variables(command_list, symbol_table)

    return command_list, symbol_table


def check_args(args):
    return (len(args) >= 2 and all([arg.split('.')[1] == 'asm' for arg in args[1:]]))


def get_args(args):
    return args[1:] if check_args(args) else None


if __name__ == "__main__":
    files = get_args(sys.argv)
    if files is None:
        # argument exception, incorrect file extention or no file passed
        raise ArgumentError("Invalid file given as argument")

    for filename in files:
        with open(filename, 'r') as input_file:
            command_list, symbol_table = parse_file(input_file)
    
        with open(f'{filename.split(".")[0]}.hack', 'w') as output_file:
            translate_commands(command_list, symbol_table, output_file)
