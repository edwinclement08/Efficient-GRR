#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include "graph_transfer.hpp"
#include "serialize_tuple.hpp"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;

GraphTransfer::GraphTransfer(const std::string &name) : name(name)
{
    std::cout << "GraphTransfer constructor called with name " << name << std::endl;
}

void GraphTransfer::add_vertex_if_not_exists(const int id, VertexProperties vertexProperties){
    graph.nodes[id] = vertexProperties;
}


void GraphTransfer::add_edge(int a, int b, EdgeProperties edgeProperties) {
    graph.edges[make_tuple(a, b)] = edgeProperties;
}


void GraphTransfer::show_graph_details() {
    std::cout << "Graph has " << graph.nodes.size() << " vertices and " << graph.edges.size() << " edges." << std::endl;
}

void GraphTransfer::save_graph(string filename) {
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << graph;
}

void GraphTransfer::load_graph(string filename) {
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);
    ia >> graph;
}
