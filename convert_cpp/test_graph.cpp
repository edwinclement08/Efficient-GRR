#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/graph/adj_list_serialize.hpp>

#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
// #include "graph_transfer.hpp"

using namespace std;
using namespace boost;

struct VertexProperties
{
    int id;
    vector<double> point;
    vector<double> config;
};

typedef adjacency_list_traits<vecS, vecS, directedS>
    Traits;
typedef adjacency_list<vecS, vecS, directedS, property<vertex_index1_t, int, VertexProperties>, property<edge_index_t, int>> _Graph;
typedef subgraph<_Graph> Graph;

int main(int argc, char const *argv[])
{
    const int N = 6;
    Graph G0(N);

    enum
    {
        A,
        B,
        C,
        D,
        E,
        F
    }; // for conveniently referring to vertices in G0

    Graph &G1 = G0.create_subgraph(), G2 = G0.create_subgraph();
    enum
    {
        A1,
        B1,
        C1
    }; // for conveniently referring to vertices in G1
    enum
    {
        A2,
        B2
    }; // for conveniently referring to vertices in G2
    add_vertex(C, G1); // global vertex C becomes local A1 for G1
    add_vertex(E, G1); // global vertex E becomes local B1 for G1
    add_vertex(F, G1); // global vertex F becomes local C1 for G1

    add_vertex(A, G2); // global vertex A becomes local A2 for G2
    add_vertex(B, G2); // global vertex B becomes local B2 for G2

    add_edge(A, B, G0);
    add_edge(B, C, G0);
    add_edge(B, D, G0);
    add_edge(E, B, G0);
    add_edge(E, F, G0);
    add_edge(F, D, G0);
    add_edge(A1, C1, G1); // (A1,C1) is subgraph G1 local indices
                          // for the global edge (C,F).
    
    boost::put(boost::vertex_index1_t(), G0, A, 53);
    boost::put(boost::vertex_index1_t(), G0, B, 53);
    boost::put(boost::vertex_index1_t(), G0, C, 53);
    boost::put(boost::vertex_index1_t(), G0, E, 53);

    cout << "G0 has " << num_vertices(G0) << " vertices, "
         << num_edges(G0) << " edges." << endl;


    // cout << "G1 has " << num_vertices(G1) << " vertices, "
    //      << num_edges(G1) << " edges." << endl;
    // print out all the vertex_index1_t properties
    property_map<Graph, vertex_index1_t>::type v = get(vertex_index1_t(), G1);
    cout << "The index map for G1 is: " << endl;
    
    auto vi = vertex(A, G0);
    cout << "vertex " << vi << " = " << get(v, vi) << endl;


    property_map<Graph, int VertexProperties::*>::type id_map = get(&VertexProperties::id, G0);
    id_map[vi] = 24232;
                
    cout << "vertex id " << vi << " = " << get(id_map, vi) << endl;
    return 0;
}
