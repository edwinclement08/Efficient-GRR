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
#include <boost/serialization/vector.hpp>
#include <boost/graph/adj_list_serialize.hpp>

using namespace std;

GraphTransfer::GraphTransfer(const std::string &name) : name(name)
{
    std::cout << "GraphTransfer constructor called with name " << name << std::endl;

    vertex_index_map = get(boost::vertex_index1_t(), graph);
    vertex_point_map = get(&VertexProperties::point, graph);
    vertex_config_map = get(&VertexProperties::config, graph);

    edge_a_id_map = get(&EdgeProperties::a_id , graph);
    edge_b_id_map = get(&EdgeProperties::b_id , graph);

    edge_weight_map = get(&EdgeProperties::weight , graph);
    edge_q_list_map = get(&EdgeProperties::q_list , graph);
    edge_connected_map = get(&EdgeProperties::connected , graph);
}

Vertex GraphTransfer::add_vertex_if_not_exists(Graph &graph, const int id, VertexProperties vertexProperties){
    Vertex v = check_vertex_exists(graph, id);
    if (v == boost::graph_traits<Graph>::null_vertex()) {
        v = boost::add_vertex(graph);
        vertex_index_map[v] = id;
        vertex_point_map[v] = vertexProperties.point;
        vertex_config_map[v] = vertexProperties.config;
    }

    return v;
}

Vertex GraphTransfer::check_vertex_exists(Graph &graph, const int id) {
    VertexItr vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi) {
        if (vertex_index_map[*vi] == id) {
            return *vi;
        }
    } 
    return boost::graph_traits<Graph>::null_vertex();
}

void GraphTransfer::add_edge(Graph &graph, int a, int b, EdgeProperties edgeProperties) {
    Vertex a_vertex = check_vertex_exists(graph, a);
    Vertex b_vertex = check_vertex_exists(graph, b);

    assert(a_vertex != boost::graph_traits<Graph>::null_vertex());
    assert(b_vertex!= boost::graph_traits<Graph>::null_vertex());
    pair<Edge,bool> e_pair = boost::add_edge(a_vertex, b_vertex, graph);
    Edge e = e_pair.first;

    edge_a_id_map[e] = a;
    edge_b_id_map[e] = b;
    edge_weight_map[e] = edgeProperties.weight;
    edge_q_list_map[e] = edgeProperties.q_list;
    edge_connected_map[e] = edgeProperties.connected;
}

void GraphTransfer::show_graph_details(Graph &graph) {
    std::cout << "Graph has " << boost::num_vertices(graph) << " vertices and " << boost::num_edges(graph) << " edges." << std::endl;
}

void GraphTransfer::print_all_data(Graph &graph) {
    // VertexItr vi, vi_end;
    // for (boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi) {
    //     std::cout << "Vertex " << graph[*vi].id << " has point: ";
    //     for (const auto& elem : graph[*vi].point) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << " and config: ";
    //     for (const auto& elem : graph[*vi].config) {
    //         std::cout << elem << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // EdgeItr ei, ei_end;
    // for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
    //     std::cout << "Edge " << graph[source(*ei, graph)].id << " -> " << graph[target(*ei, graph)].id << " has weight " << graph[*ei].weight << std::endl;
    // }
}

void GraphTransfer::save_graph(Graph &graph, string filename) {
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << graph;
}

void GraphTransfer::load_graph(Graph &graph, string filename) {
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);
    ia >> graph;
}


// Internal version of functions
Vertex GraphTransfer::add_vertex_if_not_exists_internal( const int id, VertexProperties vertexProperties) {
    return add_vertex_if_not_exists(graph, id, vertexProperties);
}
void GraphTransfer::add_edge_internal(int a, int b, EdgeProperties edgeProperties) {
    add_edge(graph, a, b, edgeProperties);
}
Vertex GraphTransfer::check_vertex_exists_internal(const int id) {
    return check_vertex_exists(graph, id);
}

void GraphTransfer::show_graph_details_internal() {
    show_graph_details(graph);
}
void GraphTransfer::print_all_data_internal() {
    print_all_data(graph);
}

// Load/Save graph
void GraphTransfer::load_graph_internal(string filename) {
    load_graph(graph, filename);
}

void GraphTransfer::save_graph_internal(string filename) {
    save_graph(graph, filename);
}
