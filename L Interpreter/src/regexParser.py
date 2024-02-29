class Node:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

syntacticSugars = {
    '\\<': 'LOWERCASE',
    '\\>': 'UPPERCASE',
    '\\$': 'DIGIT'
}

def string_to_binary_tree (regex: list) -> [Node]:
    # if the regex is made from only one character
    if len(regex) == 1:
        return [Node(regex[0])] if regex[0] not in syntacticSugars else [Node(syntacticSugars[regex[0]])]

    # replace paranthesis expressions with nodes
    while '(' in regex:
        opening_paran = regex.index('(')
        closing_paran = get_closing_paran_pos(regex, opening_paran)
        if closing_paran is None:
            print('Error: incorrect regex')

        regex = regex[:opening_paran] + string_to_binary_tree(regex[opening_paran+1:closing_paran]) + regex[closing_paran+1:]

    # remove characters which only require one operand (ex *, +, ?)
    # only concatenations and unions will remain
    regex = remove_one_character_operands(regex)

    # remove the concatenations
    regex = remove_concatenations(regex)

    # remove the unions
    regex = remove_unions(regex)

    return regex

# returns the position of the closing paranthesis corresponding to the open paranthesis
# situated on the given position. It takes into account any paranthesis pair which may
# occur in between
def get_closing_paran_pos(regex: list, position: int) -> int | None:
    # the current open paranthesis must not be included in the search
    position += 1

    stack = []
    while position < len(regex):
        if regex[position] == '(':
            stack.append('(')
        elif regex[position] == ')':
            if not stack:
                return position

            stack.pop()

        position += 1

    return None

def remove_backslashes(regex: str | Node) -> str | Node:
    if isinstance(regex, Node) or len(regex) == 1:
        return regex

    if regex in syntacticSugars:
        return syntacticSugars[regex]

    return regex[1:]

one_character_operands = {
    '*': 'STAR',
    '+': 'PLUS',
    '?': 'QUESTION'
}

def remove_one_character_operands(regex: list) -> list:
    # take care of the one character operands (*, +, ?)
    for operand, codification in one_character_operands.items():
        while operand in regex:
            operand_pos = regex.index(operand)
            operand_target = remove_backslashes(regex[operand_pos - 1])

            node = Node(codification)
            node.left = operand_target if isinstance(operand_target, Node) else Node(operand_target)

            regex = regex[:operand_pos-1] + [node] + regex[operand_pos+1:]

    return regex

def remove_unions(regex: list) -> list:
    union = '|'

    while union in regex:
        union_pos = regex.index(union)
        operand_1 = remove_backslashes(regex[union_pos - 1])
        operand_2 = remove_backslashes(regex[union_pos + 1])

        node = Node('UNION')
        node.left = operand_1 if isinstance(operand_1, Node) else Node(operand_1)
        node.right = operand_2 if isinstance(operand_2, Node) else Node(operand_2)

        regex = regex[:union_pos-1] + [node] + regex[union_pos+2:]

    return regex

def remove_concatenations(regex: list) -> list:
    concatenations_have_been_made = True

    # similar to bubble sort - while in the current step at least one concat has been made,
    # at least one more iteration over the list will be performed
    while concatenations_have_been_made:
        pos = 0
        concatenations_have_been_made = False
        while pos < len(regex) - 1:
            operand_1 = remove_backslashes(regex[pos])
            operand_2 = remove_backslashes(regex[pos + 1])

            if operand_1 == '|' or operand_2 == '|':
                pos += 1
                continue

            node = Node('CONCAT') #concatenation sign
            node.left = operand_1 if isinstance(operand_1, Node) else Node(operand_1)
            node.right = operand_2 if isinstance(operand_2, Node) else Node(operand_2)

            regex = regex[:pos] + [node] + regex[pos + 2:]

            concatenations_have_been_made = True
            pos += 1
    return regex

# removes whitespaces which are not preceded by a '\'
# removes the '\' preceeding a whitespace
def remove_unnecessary_whitespaces(regex: str) -> str:
    pos = 0
    while pos < len(regex):
        if regex[pos] == ' ' and regex[pos - 1] != '\\':
            regex = regex[:pos] + regex[pos + 1:]
            pos -= 1
        elif regex[pos] == ' ' and regex[pos - 1] == '\\':
            regex = regex[:pos - 1] + regex[pos:]
            pos -= 1
        pos += 1

    return regex


def combine_backslashes(regex: list) -> list:
    pos = 0

    while pos < len(regex) - 1:
        if regex[pos] == '\\':
            regex[pos] = regex[pos] + regex[pos + 1]
            regex = regex[:pos + 1] + regex[pos + 2:]

        pos += 1

    return regex
