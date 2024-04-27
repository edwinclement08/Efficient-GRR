#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>

#ifdef _MSC_VER
    #define EXPORT_SYMBOL __declspec(dllexport)
#else
    #define EXPORT_SYMBOL
#endif

// #ifdef __cplusplus
// extern "C" {
// #endif

using namespace std;

// typedef list<float> ConfigFor6Joint; 

// template <class Config>

class VertexProperties {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & id;
        ar & point;
        ar & config;
    }

    public:

    int id; // Used internally to prevent duplicate vertices
    vector<float> point; // 3D point address
    vector<float> config;

    VertexProperties() {}

    VertexProperties(vector<float>_point, vector<float> _config): point(_point), config(_config){
    }
};

class EdgeProperties {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & a_id;
        ar & b_id;
        ar & weight;
        ar & q_list;
        ar & connected;
    }
    
    public:
    int a_id, b_id; // Used internally to prevent duplicate edges
    float weight;
    list<pair<int, int>> q_list;
    bool connected;

    EdgeProperties() {}

    EdgeProperties(float weight, list<pair<int, int>> q_list, bool connected) {
        this->weight = weight;
        this->q_list = q_list;
        this->connected = connected;
    }
};



typedef boost::adjacency_list< boost::listS, boost::vecS, boost::directedS, VertexProperties, EdgeProperties>
  Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex; // Define Vertex
typedef boost::graph_traits<Graph>::vertex_iterator VertexItr; // Define Vertex iterator
typedef boost::graph_traits<Graph>::edge_descriptor Edge; // Define Edge
typedef boost::graph_traits<Graph>::edge_iterator EdgeItr; // Define Edge Iterator


class GraphTransfer {
    public:
    std::string name;
    Graph graph;

    GraphTransfer(const std::string &name);

    /////////////////////////////////////////////////////////////////////////////////////////
    // Act on internal graph(for interaction with python)
    Vertex add_vertex_if_not_exists_internal( const int id, VertexProperties vertexProperties);
    void add_edge_internal(int a, int b, EdgeProperties edgeProperties);
    Vertex check_vertex_exists_internal(const int id);

    void show_graph_details_internal();
    void print_all_data_internal();

    // Load/Save graph
    void load_graph_internal(string filename);
    void save_graph_internal(string filename);
    /////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////
    // Act on external graph
    Vertex add_vertex_if_not_exists(Graph &graph, const int id, VertexProperties vertexProperties);
    void add_edge(Graph &graph, int a, int b, EdgeProperties edgeProperties);
    Vertex check_vertex_exists(Graph &graph, const int id);

    void show_graph_details(Graph &graph);
    void print_all_data(Graph &graph);

    // Load/Save graph
    void load_graph(Graph &graph, string filename);
    void save_graph(Graph &graph, string filename);
    /////////////////////////////////////////////////////////////////////////////////////////
};

// #ifdef __cplusplus
// }
// #endif
