import networkx as nx

G = nx.DiGraph()
G.add_edges_from([(0, 3), (0, 1), (0, 2), (2, 1), (2, 4), (1, 4), (3, 2), (3, 5), (4, 5)])
all_topological_sorts = list(nx.algorithms.dag.all_topological_sorts(G))
print(all_topological_sorts)

G = nx.DiGraph()
G.add_edges_from([(0,1), (0,2), (0,3), (1, 4), (2, 1), (2, 4), (3, 2), (3, 5), (4, 6), (5, 6)])
all_topological_sorts = list(nx.algorithms.dag.all_topological_sorts(G))
print(all_topological_sorts)

# print([0, 1, 2, 3] in all_topological_sorts) # True
# print([2, 3, 1, 0] in all_topological_sorts) # False
