from src.Regex import parse_regex

from .NFA import NFA, EPSILON

class Lexer:
    def __init__(self, spec: list[tuple[str, str]]) -> None:
        spec_pos = 0
        shifting_index = 0
        nfas = []

        for (token, regex) in spec:

            nfa = parse_regex(regex).thompson()
            nfa = nfa.remap_states(lambda x: x + shifting_index + 1)
            nfa = nfa.remap_states(lambda x: (x, "") if x not in nfa.F else (x, token, spec_pos))
            
            spec_pos += 1
            shifting_index += len(nfa.K)
            nfas.append(nfa)

        new_S = set()
        new_K = set()
        new_q0 = (0, "")
        new_d = dict()
        new_F = set()

        for nfa in nfas:
            new_S |= nfa.S
            new_F |= nfa.F
            new_d.update(nfa.d)
            new_K |= nfa.K

            if (new_q0 ,EPSILON) in new_d:
                new_d[(new_q0, EPSILON)] = new_d[(new_q0, EPSILON)] | {nfa.q0}
            else:
                new_d[(new_q0, EPSILON)] = {nfa.q0}


        spec_nfa = NFA(
            new_S,
            new_K,
            new_q0,
            new_d,
            new_F
        )

        spec_dfa = spec_nfa.subset_construction()

        self.dfa = spec_dfa

    def lex(self, word: str) -> list[tuple[str, str]] | None:
        result = list()

        chars_prev_lines = 0
        line_no = 0
        error = False
        error_code = ""

        current_start = 0
        current_end = 0
        current_pos = 0
        current_token = ""
        current_state = self.dfa.q0

        while current_pos < len(word) + 1:
            if current_state == None or current_pos < len(word) and word[current_pos] not in self.dfa.S:
                error_code = f'No viable alternative at character {current_pos - chars_prev_lines}, line {line_no}'
                error = True
                break

            if current_pos < len(word) and word[current_pos] == '\n':
                chars_prev_lines = current_pos + 1
                line_no += 1

            if current_state in self.dfa.F:
                current_token = self.get_token(current_state)
                current_end = current_pos

                # the last character matches on a token
                if current_pos == len(word):
                    result.append((current_token, word[current_start:]))

                    break
            
            if current_pos == len(word):
                break

            # sink state
            if len(self.dfa.next_state(current_state, word[current_pos])) == 0:
                if current_token == "":
                    error_code = f'No viable alternative at character {current_pos - chars_prev_lines}, line {line_no}'
                    error = True
                    break

                result.append((current_token, word[current_start:current_end]))

                # restarting the dfa
                current_token = ""
                current_start = current_end
                current_pos = current_end
                current_state = self.dfa.q0

            current_state = self.dfa.next_state(current_state, word[current_pos])
            current_pos += 1

        if not error and current_token == "":
            error_code = f'No viable alternative at character EOF, line {line_no}'
            error = True

        return result if not error else [("", error_code)]

    def get_token(self, state_set: set) -> str | None:
        
        token = ""
        token_priority = 10000
        for current_state in state_set:
            if len(current_state) == 3 and current_state[2] < token_priority:
                token = current_state[1]
                token_priority = current_state[2]

        return token
