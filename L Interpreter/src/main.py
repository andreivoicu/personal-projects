from sys import argv
from src.Lexer import Lexer

WORD = "([a-z] | [A-Z])+"
NUMBER = "(1|2|3|4|5|6|7|8|9)[0-9]*(\ )*"
LAMBDA = f"lambda\ ([a-z] | [A-Z])+:\ (\ | : | {NUMBER} | [a-z] | [A-Z] | \+ | \+\+ | \( | \))+"
SUM = "\+"
CONCAT = "\+\+"
LAMBDA_BODY = f"{NUMBER} | {WORD} | ((\ )* \( ( \ | {NUMBER} | {WORD} | (({SUM} | {CONCAT})? (\ )*\( ({WORD} | {NUMBER} | \ )+ \)) )+\) (\ ) )"


spec = [
	("number", NUMBER),
	("empty_list", "\(\)(\ )*"),
	("lambda", LAMBDA),
	("function_invocation", f"(\( | \ )+ (\ )* ({LAMBDA} | {SUM} | {CONCAT}) (\ )* ( : | {SUM} | {NUMBER} | \( | \) | [a-z] | [A-Z] | \ )+ (\) | \ )+"),
	("list", f"\(( {NUMBER} | \( ({NUMBER} | \(\) | \ )* \) | \ )+\)(\ )*")
]

spec_lambda = [
	("open_paran", "(\ )*\((\ )*"),
	("closed_paran", "(\ )*\)(\ )*"),
	("lambda", f"((\ )*lambda\ {WORD}:\ )+ (\ )*({LAMBDA_BODY}*)"),
	("replacement", f"(\ )*{NUMBER}(\ )* | (\ )*{WORD}(\ )* | (\ )*\( ( {NUMBER} | {WORD} | \ )+ \)(\ )*"),
	("sum | concat", f"(\ )* \(? ({SUM} | {CONCAT} | \ )+ \(( {WORD} | {NUMBER} | \(\) | \ )+\)")
]

lexer = Lexer(spec)
lexer_lambda = Lexer(spec_lambda)

result_list = ""
result_sum = 0

operation_stack = []

def main():
	if len(argv) != 2:
		return
	
	filename = argv[1]
	file_content = ""

	with open(filename, 'r') as file:
		file_content = file.read()

	# replace newlines with whitespaces
	file_content = file_content.replace('\n', ' ')

	file_content = format_input(file_content)

	evaluate(file_content, 'print_list')

	global result_list
	result_list = result_list.rstrip()
	result_list = ' '.join(result_list.split())
	print(result_list)

def evaluate(input: str, action: str):
	global lexer, result_list, result_sum
	tokens = lexer.lex(input)

	pos = 0

	for token in tokens:
		if token[0] == "number":
			if action == 'print_list' or action == 'concat':
				result_list += f'{token[1]}'
			elif action == 'sum':
				result_sum += get_number(token[1])

		elif token[0] == "empty_list":
			if action == 'print_list':
				result_list += f'{token[1]}'
			elif action == 'sum':
				continue

		elif token[0] == 'list':
			if action == 'concat':
				result_list += format_list(token[1]) + ' '
				continue

			if action == 'print_list':
				result_list += '( '
			evaluate(format_list(token[1]), action)
			if action == 'print_list':
				result_list += ' ) '

		elif token[0] == 'function_invocation' and get_function(token[1]) == 'sum':
			result_sum = 0
			operation_stack.append('sum')
			evaluate(format_function(token[1]), 'sum')
			result_list += f' {result_sum}'

		elif token[0] == 'function_invocation' and get_function(token[1]) == 'concat':
			result_list += '( '
			operation_stack.append('concat')
			evaluate(format_function(token[1]), 'concat')
			result_list += ' ) '

			operation_stack.pop()
			if operation_stack:
				# reseting the result list for any future concatenations
				aux = result_list
				result_list = ""

				evaluate(format_list(aux), operation_stack.pop())

		elif token[0] == 'function_invocation' and get_function(token[1]) == 'lambda' or token[0] == 'lambda':
			evaluate(get_lambda_result(token[1]), action)

		pos += 1


# returns the id of the first lambda function in the list
def get_id(input: str) -> str:
	start_index = input.index("lambda") + len("lambda") + 1
	end_index = input.index(":")

	return input[start_index: end_index]

# returns the index of paranthesis enclosing the first lambda function
# the index is raported to the tokenized list
def get_paranthesis_index(tokens: list) -> (int, int):
	start_index = 0
	end_index = 0
	paranthesis_stack = []
	
	for pos in range(1, len(tokens)):
		if tokens[pos][0] == 'lambda':
			start_index = pos - 1
			break

	for pos in range (start_index + 1, len(tokens)):
		if tokens[pos][0] == 'open_paran':
			paranthesis_stack.append('(')
		elif tokens[pos][0] == 'closed_paran':
			if not paranthesis_stack:
				end_index = pos
				break
			else:
				paranthesis_stack.pop()

	return (start_index, end_index)

# receives the index of the closing paranthesis and returns
# the string which will replace the id of the lambda function
def get_replacement(tokens: list, end_index: int) -> str:
	return tokens[end_index - 1][1]

# returns the body of the lambda (without 'lambda id: ')
def get_lambda_body (tokens: list, start_index: int, end_index: int) -> str:
	body = tokens[start_index + 1][1]
	body = body[body.index(':') + 1:]

	for pos in range(start_index + 2, end_index - 1):
		token = tokens[pos]
		body += token[1]

	return body

# finds all occurences of 'id' in text
def find_all_occurrences(text: str, id: int) -> list:
    occurrences = []
    start = 0

    while start < len(text):
        index = text.find(id, start)
        if index == -1:
            break
        occurrences.append(index)
        start = index + 1

    return occurrences

# returns the indexes (relative to body) of the occurences if 'id'
# which shoul be replaced
def get_replacement_indexes(lambda_body: str, body: str, id: str) -> list:
	if lambda_body.count(f'lambda {id}') > 1:
		# print("no replacements")
		return []
	
	tokens = lexer_lambda.lex(body)

	replacement_indexes = []

	pos_in_string = 0

	for token in tokens:
		if token[0] != 'lambda' or token[1].find(f'lambda {id}') == -1:
			occurences = find_all_occurrences(token[1], id)
			occurences = [index + pos_in_string for index in occurences]

			replacement_indexes.extend(occurences)

		pos_in_string += len(token[1])

	return replacement_indexes

def replace_id(body: str, replacement_indexes: list, id: str, replacement: str) -> str:
	for index in replacement_indexes[::-1]:
		body = body[:index] + replacement + ' ' + body[index + len(id):]

	return body

# receives a lambda call and resolves the leftmost lambda function
def get_lambda_result(input: str) -> str:
	
	global lexer_lambda

	if input[0] != '(':
		input = '(' + input + ')'

	tokens = lexer_lambda.lex(input)

	id = get_id(input)
	first_paran, last_paran = get_paranthesis_index(tokens)
	replacement = get_replacement(tokens, last_paran)
	body = get_lambda_body(tokens, first_paran, last_paran)

	replacement_indexes = get_replacement_indexes(tokens[first_paran + 1][1], body, id)
	body = replace_id(body, replacement_indexes, id, replacement)
	

	tokens = tokens[:first_paran] + [("", body)] + tokens[last_paran + 1:]

	output = ""
	for token in tokens:
		output += token[1]
	return output.lstrip().rstrip()

# receives a number as a string, removes spaces and returns the corresponding integer
def get_number(input: str) -> int:
	input = input.rstrip()
	input = input.lstrip()

	return int(input)

# receives a 'function call' string and returns the operation
# ex: sum, concat or lambda 
def get_function(input: str) -> str:
	index_sum = 0
	index_concat = 0
	index_lambda = 0
	
	try:
		index_sum = input.index('+')
	except ValueError:
		index_sum = 100000

	try:
		index_concat = input.index('++')
	except ValueError:
		index_concat = 100000

	try:
		index_lambda = input.index('lambda')
	except ValueError:
		index_lambda = 100000

	min_index = min(index_sum, index_concat, index_lambda)

	if min_index == index_lambda:
		return 'lambda'
	elif min_index == index_concat:
		return 'concat'
	elif min_index == index_sum:
		return 'sum'
	else:
		return 'error'

# removes the '(' and ')' surrounding the list
# removes the trailing whitespaces at both the beggining and the end of the string
# removes the function call
def format_function(input: str) -> str:
	beggining = input.find('(')
	end = input.rfind(')')

	input = input[beggining + 1:end].lstrip().rstrip()

	first_parant_index = input.index('(')

	input = input[first_parant_index + 1:len(input) - 1]

	return input

# removes the '(' and ')' surrounding the list
# removes the trailing whitespaces at both the beggining and the end of the string
def format_list(input: str):
	beggining = input.find('(')
	end = input.rfind(')')

	return input[beggining + 1:end].lstrip().rstrip()


# removes newlines and whitespaces which are not adjacent to numbers
def format_input(input: str) -> str:
	# remove newlines
	input = input.replace('\n', ' ')
	formatted_input = ' '.join(input.split())

	return formatted_input

if __name__ == '__main__':
    main()
