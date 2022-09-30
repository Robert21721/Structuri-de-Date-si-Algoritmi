#include "Structuri.h"
//######################

// Cerinta 1

//######################


// functie pentru creerea unui nod
TNode *create_node (char *name) {
    TNode *n = (TNode*) malloc(sizeof(TNode));

    n->name = name;
    n->next = NULL;
}

// ###############################################################################

// functie pentru aflarea indexului vectoruiui de noduri dupa nume
int get_idx(TGraph *g, char *name) {
    int n = g->no;

    for (int i = 0; i < n; i++) {
        if (strcmp(g->arr[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// ###############################################################################

// functie de sortarea in ordine lexicografica a nodurilor din vectorul arr
void sort(TNode **arr, int n) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(arr[i]->name, arr[j]->name) > 0) {
                TNode *aux = arr[i];
                arr[i] = arr[j];
                arr[j] = aux;
            }
        }
    }
}

// #############################################################################

// functie care creeaza graful (vector + liste de adiacenta)
TGraph *create_graph(FILE *f_in) {
    int n;
    TGraph *g = (TGraph*) malloc(sizeof(TGraph));

    // citesc numarul de noduri din graf
    fscanf(f_in, "%d", &n);
    g->no = n;

    // aloc memorie pentru vector si initializez fiecare element cu NULL (folosind calloc)
    g->arr = (TNode**) calloc(n, sizeof(TNode*));

    // pentru fiecare nod citesc numele din fisier si creez nodul
    for (int i = 0; i < n; i++) {
        char *name = (char*) malloc(N * sizeof(char));

        fscanf(f_in, "%s", name);
        TNode *n = create_node(name);
        g->arr[i] = n;
    }

    // sortez vectorul
    sort(g->arr, n);

    char *next_nodes_name, sep[] = " :\n", *p;
    next_nodes_name = (char*) malloc(N * sizeof(char));
    // citesc inca un caracter in gol (deoarece ramane un \n necitit)
    char back_slash[N];
    fgets(back_slash ,N, f_in);

    for (int i = 0; i < n; i++) {
        // citesc cate o linie din fisier ce reprezinta lista de adiacenta a fiecarui nod
        fgets(next_nodes_name, N, f_in);

        // p primeste numele nodului
        p = strtok(next_nodes_name, sep);

        // j obtine indexul nodului cu numele p
        int j = get_idx(g, p);
        p = strtok(NULL, sep);

        // trec la urmatorul nume din lista de adiacenta, creez nodurile si le adaug in lista
        while (p != NULL) {
            char *name = strdup(p);
            TNode *n = create_node(name);

            n->next = g->arr[j]->next;
            g->arr[j]->next = n;
            p = strtok(NULL, sep);
        }
    }

    free(next_nodes_name);
    return g;
}

// ##############################################################################

// aplic algoritmul dfs pentru a afla daca exista cicluri
void dfs_node(TNode *start_node, TGraph *g, int k, int visited[N], int *ok) {
    TNode *idx;
    visited[k]++;

    // incep cautarea de la g->arr[k]->next adica primul element din lista de adiacenta
    // a nodului de start
    for (idx = g->arr[k]->next; idx != NULL; idx = idx->next) {
        // daca in timpul executiei ajung din nou cu idx in nodul de start
        // ok va lua valoarea 0 (adica graful este ciclic)
        // si opresc executia
        if (strcmp (start_node->name, idx->name) == 0) {
            *ok = 0;
            return;
        }

        // i ia valoarea indexului nodului cu numele idx->name
        int i = get_idx(g, idx->name);

        // daca nodul nu a fost inca vizitat se apeleaza dfs pe acel nod
        if (visited[i] == 0) {
            dfs_node(start_node, g, i, visited, ok);
        }
    }
}

// #############################################################################

// verifica daca graful este ciclic
void verify_ciclic_graph(FILE *f_out, TGraph *g) {
    int n = g->no;
    int visited[N] = {0};
    int ok = 1;

    // se aplica dfs din fiecare nod
    for (int i = 0; i < n; i++) {
        dfs_node(g->arr[i], g, i, visited, &ok);
    }

    // daca ok ramane 1 inseana ca graful nu are cicluri
    if (ok == 1) {
        fprintf(f_out, "%s\n", "correct");
    } else {
        fprintf(f_out, "%s\n", "impossible");
    }
}

//#######################

// Cerinta 2

//#######################

// reprezinta multimea nodurilor referite direct sau indirect de catre Node
int *past(FILE *f_out ,TNode *node, TGraph *g) {
    int visited[N] = {0};
    int *ret_arr = (int*) calloc(N, sizeof(int));
    int ok;
    int n = g->no;

    // aflu indexul la care se afla nodul si aplic dfs pornind din el
    int idx = get_idx(g, node->name);
    dfs_node(node, g, idx, visited, &ok);

    fprintf(f_out, "past(%s) : ", node->name);
    for (int i = 0; i < n; i++) {
        // parcurg vectorul visited si daca valoarea acestuia este diferita de 0
        // iar i este diferit de index (adica nu este vorba de nodul de start)
        // scriu in fisier numele nodului de la indexul i
        if (visited[i] != 0 && i != idx) {
            fprintf(f_out, "%s ", g->arr[i]->name);
            ret_arr[i] = 1;
        }
    }
    fprintf(f_out, "\n");
    // functia returneaza un vector de intregi care are 1 pe pozitiile nodurilor
    // ce respecta criteriul si 0 in rest
    return ret_arr;
}

// ##############################################################################

// reprezinta multimea nodurilor care au referinta directa sau indirecta catre blocul Node
int *future(FILE *f_out, TNode* node, TGraph *g, int ok_print) {
    int *ret_arr = (int*) calloc(N, sizeof(int));
    int ok;
    int n = g->no;
    int idx = get_idx(g, node->name);

    // folosesc un ok_print deoarece doresc sa folosesc aceasta functie in functia
    // tips fara a printa in fisier rezultatul functiei
    if (ok_print) {
        fprintf(f_out, "future(%s) : ", node->name);
    }

    // parcurg fiecare nod
    for (int i = 0; i < n; i++) {
        // initializez vectorul visited cu 0
        int visited[N] = {0};
        
        // daca i diferit de index aplic dfs din acel nod
        if (i != idx) {
          dfs_node(g->arr[i], g, i, visited, &ok);  
        }
        
        // daca visited[idx] == 1 inseamna ca din nodul cu indicele i
        // se poate ajunge in nodul cu indicele idx
        if (visited[idx] == 1) {
            if (ok_print) {
                fprintf(f_out, "%s ", g->arr[i]->name);   
            }
            ret_arr[i] = 1;
        }
    }
    if (ok_print) {
        fprintf(f_out, "\n");
    }
    // functia returneaza un vector de intregi care are 1 pe pozitiile nodurilor
    // ce respecta criteriul si 0 in rest
    return ret_arr;
}

// ##########################################################################

// reprezintă multimea nodurilor pentru care ordinea dintre ele si blocul Node este ambigua
int *anticone(FILE *f_out, int *v_past, int *v_future, TNode *node, TGraph *g) {
    int *ret_arr = (int*) calloc(N, sizeof(int));
    int n = g->no;
    int idx = get_idx(g, node->name);

    fprintf(f_out, "anticone(%s) : ", node->name);
    for (int i = 0; i < n; i++) {
        // din main, v_past si v_future sunt vectorii returnati de functiile anterioare
        // daca pe pozitia i ambele valori sunt 0 inseamna ca nodul nu face parte nici
        // din past nici din future si deci va fi parte din anticone
        if (v_past[i] == 0 && v_future[i] == 0 && i != idx) {
            fprintf(f_out, "%s ", g->arr[i]->name);
            ret_arr[i] = 1;
        }
    }
    fprintf(f_out, "\n");
    // functia returneaza un vector de intregi care are 1 pe pozitiile nodurilor
    // ce respecta critariul si 0 in rest
    return ret_arr;
}

// ############################################################################

// reprezinta multimea nodurilor din graful G care au gradul intern egal cu 0
int *tips(FILE *f_out, TGraph *g) {
    int *ret_arr = (int*) calloc(N, sizeof(int));
    int n = g->no;

    fprintf(f_out, "tips(G) : ");
    for (int i = 0; i < n; i++) {
        // pentru fiecare nod din graf se apeleaza functia de future fara printare
        int *v = future(f_out, g->arr[i], g, 0);
        int ok = 1;

        // daca toate valorile lui v raman 0 dupa apel inseamna ca nodul i
        // respecta regla de la tips
        for (int j = 0; j < n; j++) {
            if (v[j] != 0) {
                ok = 0;
            }
        }

        // daca nodul i respecta regula de la tips, se face scrierea in fisier
        if (ok) {
            fprintf(f_out, "%s ", g->arr[i]->name);
            ret_arr[i] = 1;
        }
        free(v);
    }

    fprintf(f_out, "\n");
    // functia returneaza un vector de intregi care are 1 pe pozitiile nodurilor
    // ce respecta criteriul si 0 in rest
    return ret_arr;
}

// ###########################################################################

// functie de stergere a unei liste de adiacenta
void delete_list(TNode* node) {
    if (node == NULL) {
        return;
    }

    delete_list(node->next);
    free(node->name);
    free(node);
}

// ############################################################################

// functie pentru stergerea intregului graf
void free_graph(TGraph *g) {
    int n = g->no;

    for (int i = 0; i < n; i++) {
        delete_list(g->arr[i]);
    }

    free(g->arr);
    free(g);
}
