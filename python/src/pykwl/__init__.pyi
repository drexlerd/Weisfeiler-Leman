from typing import Tuple, List, Dict

class Graph:
    def __init__(self, directed : bool) -> None: ...
    def add_node(self, label: int = 0) -> int: ...
    def add_edge(self, src_node: int, dst_node: int, label: int = 0) -> None: ...

class WeisfeilerLeman:
    def __init__(self, k: int, ignore_counting: bool = False) -> None: ...
    def get_k(self) -> int: ...
    def get_ignore_counting(self) -> bool: ...
    def compute_coloring(self, graph: Graph) -> Tuple[int, List[int], List[int]]: ...
    def get_coloring_function(self) -> Dict[Tuple[int, List[Tuple[int, int]], List[Tuple[int, int]]], int]: ...
