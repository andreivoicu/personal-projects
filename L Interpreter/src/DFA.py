from collections.abc import Callable
from dataclasses import dataclass


@dataclass
class DFA[STATE]:
    S: set[str]
    K: set[STATE]
    q0: STATE
    d: dict[tuple[STATE, str], STATE]
    F: set[STATE]

    def accept(self, word: str) -> bool:
        # simulate the dfa on the given word. return true if the dfa accepts the word, false otherwise
        state = self.q0

        for symbol in word:
            if symbol not in self.S or state is None:
                return False

            state = self.next_state(state, symbol)

        return state in self.F

    def next_state(self, state: STATE, symbol: str) -> STATE | None:
        return self.d[(state, symbol)] if (state, symbol) in self.d else None

    def remap_states[OTHER_STATE](self, f: Callable[[STATE], 'OTHER_STATE']) -> 'DFA[OTHER_STATE]':
        new_d = {}
        for (state, symbol), next_state in self.d.items():
            new_d[(f(state), symbol)] = f(next_state)

        return DFA(
            self.S,
            set(map(f, self.K)),
            f(self.q0),
            new_d,
            set(map(f, self.F))
        )
