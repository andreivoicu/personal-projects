from .NFA import NFA

from .regexParser import string_to_binary_tree, remove_unnecessary_whitespaces, combine_backslashes

from .regexToNFA import get_nfa

class Regex:
    # the root of the binary tree representation of the regex
    root = None

    def __init__(self, root):
        self.root = root

    def thompson(self) -> NFA[int]:
        return get_nfa(self.root)

def parse_regex(regex: str) -> Regex:
    # represent a regex as a binary tree

    # changing syntactic sugars to single characters so the regex can be easily understood by the algorithm
    syntacticSugars = {
        '[a-z]': '\\<',
        '[A-Z]': '\\>',
        '[0-9]': '\\$'
    }

    # remove unnecessary whitespaces
    regex = remove_unnecessary_whitespaces(regex)


    # remove syntactic sugars from regex
    for syntacticSugar, codification in syntacticSugars.items():
        regex = regex.replace(syntacticSugar, codification)

    # tokenize the regex
    tokens = list(regex)

    tokens = combine_backslashes(tokens)

    node_list = string_to_binary_tree(tokens)

    return Regex(node_list[0])
