from dataclasses import dataclass
from collections.abc import Callable

from queue import Queue

from .DFA import DFA

EPSILON = ''  # this is how epsilon is represented by the checker in the transition function of NFAs

@dataclass
class NFA[STATE]:
    S: set[str]
    K: set[STATE]
    q0: STATE
    d: dict[tuple[STATE, str], set[STATE]]
    F: set[STATE]

    def epsilon_closure(self, state: STATE) -> set[STATE]:
        visited_states = set()

        state_queue = Queue()
        state_queue.put(state)

        while not state_queue.empty():
            state = state_queue.get()
            visited_states.add(state)

            if (state, EPSILON) in self.d:
                # only the states which have not been visited must be added into the queue
                not_visited_states = self.d[(state, EPSILON)] - visited_states

                state_queue.queue.extend(item for item in not_visited_states)

        return visited_states

    def subset_construction(self) -> DFA[frozenset[STATE]]:

        dfa_initial_state = frozenset(self.epsilon_closure(self.q0))
        dfa_states = {dfa_initial_state}
        dfa_transitions = {}
        dfa_final_states = set()

        states_queue = Queue()
        states_queue.put(dfa_initial_state)

        while not states_queue.empty():
            state = states_queue.get()

            for symbol in self.S:
                next_dfa_state = set()
                for atomic_state in state:
                    next_dfa_state |= self.next_state(atomic_state, symbol)

                # convert to a hashable type
                next_dfa_state_hashable = frozenset(next_dfa_state)

                dfa_transitions[(state, symbol)] = next_dfa_state_hashable

                if next_dfa_state_hashable not in dfa_states:
                    dfa_states.add(next_dfa_state_hashable)
                    states_queue.put(next_dfa_state_hashable)

        for state in dfa_states:
            if self.is_final_state(state):
                dfa_final_states.add(state)

        return DFA(
            self.S,
            dfa_states,
            dfa_initial_state,
            dfa_transitions,
            dfa_final_states
        )

    def next_state(self, state: STATE, symbol: str) -> set[STATE]:
        next_state = set()

        if (state, symbol) in self.d:
            for current_state in self.d[(state, symbol)]:
                next_state |= self.epsilon_closure(current_state)

        return next_state

    # checks if state is a final state for the new dfa
    def is_final_state(self, state: STATE) -> bool:
        return state & self.F

    def remap_states[OTHER_STATE](self, f: 'Callable[[STATE], OTHER_STATE]') -> 'NFA[OTHER_STATE]':
        new_d = {}
        for (state, symbol), next_state in self.d.items():
            new_d[(f(state), symbol)] = set(map(f, next_state))

        return NFA(
            self.S,
            set(map(f, self.K)),
            f(self.q0),
            new_d,
            set(map(f, self.F))
        )
