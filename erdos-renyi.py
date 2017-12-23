def set_edges_weight(G, w):
    for u, v in G.edges():
        G[u][v]['weight'] = w
    return G

def gen_random_graph(n, p=None, m=None):
    if p is None and m is None or not (p is None or m is None):
        raise Exception("Wrong params")
        
    if p is not None:
        G = nx.random_graphs.gnp_random_graph(n, p)
    else:
        G = nx.random_graphs.gnm_random_graph(n, m)
    nx.draw(G, with_labels=True)
    plt.show()
    G = set_edges_weight(G, 1)
    return G

G = gen_random_graph(20, m=45) 
