import sys

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


def check_args(args):
    return (len(args) == 2 and args[1].split('.')[1] == 'asm')

def get_args(args):
    return args[1] if check_args(args) else None

def parse_a_instruction(command):
    value = 0
    first_loop = True
    for char in command:
        if char == "@" and first_loop:
            first_loop = False
            continue
        elif not first_loop and char.isdigit():
            value += int(char)
        else:
            # report error, command doesn't start with @ or contains values that aren't integers
            pass

    fifteen = int('111111111111111', 2)
    if value > fifteen:
        value = fifteen
    return "0" + format(value, 'b').zfill(15)

def get_c_instruction(dest_command, comp_command, jump_command):
    try:
        dest = DEST_COMMANDS[dest_command]
    except KeyError:
        print(f'{dest_command} is invalid syntax for destination command')
        dest = DEST_COMMANDS[""]

    try:
        comp = COMP_COMMANDS[comp_command]
    except KeyError:
        print(f'{comp_command} is invalid syntax for computation command')
        comp = COMP_COMMANDS['1']

    try:
        jump = JUMP_COMMANDS[jump_command]
    except KeyError:
        print(f'{jump_command} is invalid syntax for jump command')
        jump = JUMP_COMMANDS[""]

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

def parse_line(line):
    for command in line.split():
        if command == "//" or "":
            return None
        elif "@" in command:
            return parse_a_instruction(command)
        else:
            return parse_c_instruction(command)

def parse_file(input_file, output_file):
    for line in input_file.readlines():
        # if '//' in line.split():
        #    continue
        output = parse_line(line)
        if output is not None:
            output_file.write(f"\n{output}")


if __name__ == "__main__":
    filename = get_args(sys.argv)
    with open(filename, 'r') as input_file:
        with open(f'{filename.split(".")[0]}.hack', 'w') as output_file:
            parse_file(input_file, output_file)

                  
        
