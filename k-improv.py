def check_if_matching(G, k):
    """Проверка, является ли граф G k-паросочетанием"""
    for n in G.nodes:
        if len(list(G.neighbors(n))) > k:
            return False
    return True

def get_components_count(G):
    """Подсчёт по графу G всех необходимых значений: 
    количество путей и циклов, объединением которых он является,
    количество вершин в циклах и
    количество синглтонов"""
    subgraphs = nx.connected_component_subgraphs(G)
    number_vertices_in_cycles = 0
    components_count = 0
    singletons_count = 0
    for g in subgraphs:
        components_count += 1
        if g.number_of_nodes() == 2 and g.number_of_edges() == 1:
            singletons_count += 1
        try:
            if len(nx.find_cycle(g)) == len(g.nodes):
                number_vertices_in_cycles += len(G.nodes)
        except nx.NetworkXNoCycle:
            pass
    return components_count, number_vertices_in_cycles, singletons_count

def get_chain_on_nodes(G, C_nodes):
    """Возвращает граф, который есть цепь из G на данных C_nodes вершинах"""
    return set([(C_nodes[i], C_nodes[i+1]) 
                for i in range(0, len(C_nodes)-1) 
                if (C_nodes[i], C_nodes[i+1]) in G.edges])

def improv(K, G, draw=False, desired=None):
    """Алгоритм K-IMPROV
    draw может работать плохо из-за каких-то багов с colormap'ами в networkx"""
    if K > G.number_of_edges():
        K = G.number_of_edges()
    
    # Изначально A пусто
    A = nx.Graph()
    A.add_nodes_from(G.nodes)
    A_components_num = sys.maxsize
    A_vertices_in_cycles_num = sys.maxsize
    A_singletons_num = sys.maxsize
    
    # Перебираем все возможные варианты для C
    log.info('Граф с %d вершинами и %d рёбрами'%(G.number_of_nodes(), G.number_of_edges()))
    max_iter = sum([comb(G.number_of_edges(), i) for i in range(0, K+1)])-G.number_of_edges()-1
    log.info('Число комбинаций для перебора %d при K=%d'%(max_iter, K))
    
    n_global_iter = 0
    something_changes = True
    last_modification_iter, last_modification_global_iter = 0, 1
    while something_changes:
        n_global_iter += 1
        log.info('             GLOBAL ITERATION %d'%n_global_iter)
        something_changes = False
        C_edges_s = itertools.chain.from_iterable(itertools.combinations(G.edges, i) 
                                                      for i in range(1, K+1))
        
        if log.getLogger().isEnabledFor(log.INFO):
            pbar = tqdm.tqdm_notebook(total=max_iter)
        exiting = False
        for n_iter, C_edges in enumerate(C_edges_s):
            if log.getLogger().isEnabledFor(log.INFO):
                pbar.update(1)
            
            if last_modification_global_iter < n_global_iter and n_iter > last_modification_iter:
                log.info("Early exit: nothing to check")
                break
            if C_edges == set():
                continue
            
            C = nx.Graph()
            C.add_nodes_from(G.nodes)
            C.add_edges_from(C_edges)
            G_new = nx.symmetric_difference(A, C)

            cur_components_num, cur_vertices_in_cycles_num, cur_singletons_num = get_components_count(G_new)

            if check_if_matching(G_new, 2):
                if (cur_components_num<A_components_num) \
                        or (cur_components_num == A_components_num \
                            and cur_vertices_in_cycles_num > A_vertices_in_cycles_num) \
                        or (cur_components_num == A_components_num \
                            and cur_vertices_in_cycles_num == A_vertices_in_cycles_num \
                            and cur_singletons_num < A_singletons_num):
                    something_changes = True
                    log.info('Changing A: %d %d %d'%( cur_components_num, cur_vertices_in_cycles_num, cur_singletons_num))
                    log.debug('was: %d %d %d'%( A_components_num, A_vertices_in_cycles_num, A_singletons_num))
                    last_modification_iter = n_iter
                    last_modification_global_iter = n_global_iter

                    # Баг networkx: если вставить рисовалку сюда, первый раз будет неправильный цвет.
    
                    A = G_new
                    A_components_num = cur_components_num
                    A_vertices_in_cycles_num = cur_vertices_in_cycles_num
                    A_singletons_num = cur_singletons_num
                    
                    # Сделаем выход пораньше, чтобы тестилось побыстрее
                    algo_ans = G.number_of_nodes()+A_components_num
                    if desired is not None:
                        if algo_ans < 8/7*desired:
                            exiting=True
                            log.info('K=%d was enough'%C.number_of_edges())
                            break
                        
                    if draw:
                        edge_color = [choose_color(edge, A, C) for edge in G.edges ]
                        plt.figure(figsize=(5,5))
                        draw_weighted_graph(G, edge_color=edge_color, width=4, edge_cmap=rb_cmap)
                        plt.show()
        if log.getLogger().isEnabledFor(log.INFO):
            pbar.close()
        if exiting:
            break

    algo_ans = G.number_of_nodes()+A_components_num
    if A_components_num == 1 and A_vertices_in_cycles_num > 0:
        algo_ans -= 1
    #nx.draw(A, with_labels=True)
    #plt.show()
    return algo_ans, A, C.number_of_edges()
