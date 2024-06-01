from typing import Tuple, List, MutableSet

class EdgeColoredGraph:
    def __init__(self, directed : bool) -> None: ...
    def add_node(self, label: int = 0) -> int: ...
    def add_edge(self, src_node: int, dst_node: int, label: int = 0) -> None: ...

class GraphColoring:
    def get_frequencies(self) -> Tuple[List[int], List[int]]: ...

class CanonicalColorRefinement:
    @staticmethod
    def histogram(self, coloring: List[MutableSet[int]]) -> List[int]: ...
    def calculate(self, graph: EdgeColoredGraph, factor_matrix = False) -> None: ...
    def get_coloring(self) -> List[int]: ...
    def get_factor_matrix(self) -> List[Tuple[Tuple[int, int], int]]: ...
    def get_coloring_function_size(self) -> int: ...

class WeisfeilerLeman:
    def __init__(self, k: int, ignore_counting: bool = False) -> None: ...
    def get_k(self) -> int: ...
    def get_ignore_counting(self) -> bool: ...
    def compute_coloring(self, graph: EdgeColoredGraph) -> Tuple[bool, int, List[int], List[int]]: ...
    def compute_initial_coloring(self, graph: EdgeColoredGraph) -> GraphColoring: ...
    def compute_next_coloring(self, graph: EdgeColoredGraph, current_coloring: GraphColoring, next_coloring: GraphColoring) -> bool: ...
    def get_coloring_function_size(self) -> int: ...
