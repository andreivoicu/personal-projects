import string

from .NFA import NFA, EPSILON

from .regexParser import Node

syntacticSugars = {
    'UPPERCASE': set(string.ascii_uppercase),
    'LOWERCASE': set(string.ascii_lowercase),
    'DIGIT': set(string.digits)
}

def simple_nfa(character: str) -> NFA:
    S = syntacticSugars[character] if character in syntacticSugars else {character}
    K = {0, 1}
    q0 = 0

    d = {}
    for char in S:
        d[(0, char)] = {1}

    F = {1}

    return NFA(
        S,
        K,
        q0,
        d,
        F
    )

def concatenation(nfa_1: NFA, nfa_2: NFA) -> NFA:
    nfa_1 = nfa_1.remap_states(lambda x: x + 1)
    nfa_2 = nfa_2.remap_states(lambda x: x + len(nfa_1.K) + 1)

    new_q0 = 0
    new_F = {len(nfa_1.K) + len(nfa_2.K) + 1}

    new_S = nfa_1.S | nfa_2.S
    new_K = nfa_1.K | nfa_2.K | {new_q0} | new_F

    new_d = {**nfa_1.d, **nfa_2.d}

    # add transition from the new initial state to the initial state of nfa_1
    new_d[(new_q0, EPSILON)] = {nfa_1.q0}

    # add transitions from every final state of nfa_1 to initial state of nfa_2
    for state in nfa_1.F:
        new_d[(state, EPSILON)] = {nfa_2.q0}

    # add transition from every final state of nfa_2 to the final state of the new NFA
    for state in nfa_2.F:
        new_d[((state, EPSILON))] = new_F

    return NFA(
        new_S,
        new_K,
        new_q0,
        new_d,
        new_F
    )

def union(nfa_1: NFA, nfa_2: NFA) -> NFA:
    nfa_1 = nfa_1.remap_states(lambda x: x + 1)
    nfa_2 = nfa_2.remap_states(lambda x: x + len(nfa_1.K) + 1)

    new_q0 = 0
    new_F = {len(nfa_1.K) + len(nfa_2.K) + 1}

    new_S = nfa_1.S | nfa_2.S
    new_K = nfa_1.K | nfa_2.K | {new_q0} | new_F

    new_d = {**nfa_1.d, **nfa_2.d}

    # add transitions from the new intial state to the initial state of each nfa
    new_d[(new_q0, EPSILON)] = {nfa_1.q0, nfa_2.q0}

    # add transitions from every final state of both nfa's to the new final state
    for state in nfa_1.F | nfa_2.F:
        new_d[(state, EPSILON)] = new_F

    return NFA(
        new_S,
        new_K,
        new_q0,
        new_d,
        new_F
    )

def star(nfa: NFA) -> NFA:
    nfa = nfa.remap_states(lambda x: x + 1)

    new_q0 = 0
    new_F = {len(nfa.K) + 1}

    new_S = nfa.S
    new_K = nfa.K | {new_q0} | new_F

    new_d = nfa.d

    # add transitions from the new initial state to the nfa initial state and the new last state
    new_d[(new_q0, EPSILON)] = {nfa.q0} | new_F

    # add transitions from every final state of the nfa to the initial state of the nfa
    # and to the new final state
    for state in nfa.F:
        new_d[(state, EPSILON)] = {nfa.q0} | new_F

    return NFA(
        new_S,
        new_K,
        new_q0,
        new_d,
        new_F
    )

def plus(nfa: NFA) -> NFA:
    nfa = nfa.remap_states(lambda x: x + 1)

    new_q0 = 0
    new_F = {len(nfa.K) + 1}

    new_S = nfa.S
    new_K = nfa.K | {new_q0} | new_F

    new_d = nfa.d

    # add transitions from the new initial state to the nfa initial state
    new_d[(new_q0, EPSILON)] = {nfa.q0}

    # add transitions from every final state of the nfa to the initial state of the nfa
    # and to the new final state
    for state in nfa.F:
        new_d[(state, EPSILON)] = {nfa.q0} | new_F

    return NFA(
        new_S,
        new_K,
        new_q0,
        new_d,
        new_F
    )

def question(nfa: NFA) -> NFA:
    nfa = nfa.remap_states(lambda x: x + 1)

    new_q0 = 0
    new_F = {len(nfa.K) + 1}

    new_S = nfa.S
    new_K = nfa.K | {new_q0} | new_F

    new_d = nfa.d

    # add transitions from the new initial state to the nfa initial state and the new last state
    new_d[(new_q0, EPSILON)] = {nfa.q0} | new_F

    # add transitions from every final state of the nfa to the new final state
    for state in nfa.F:
        new_d[(state, EPSILON)] = new_F

    return NFA(
        new_S,
        new_K,
        new_q0,
        new_d,
        new_F
    )

thompson_1_operand = {
    'STAR': star,
    'PLUS': plus,
    'QUESTION': question
}

thompson_2_operands = {
    'CONCAT': concatenation,
    'UNION': union
}

def get_nfa(node: Node) -> NFA:
    # if the current node has at least one child, it means it is an operation
    operation = node.value
    if operation in thompson_1_operand:
        return thompson_1_operand[operation](get_nfa(node.left))

    if operation in thompson_2_operands:
        return thompson_2_operands[operation](get_nfa(node.left), get_nfa(node.right))

    # if 'operation' is neither in 1_operand or 2_operands, it means it is a simple character
    return simple_nfa(operation)
