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

EXPORT_SYMBOL float graph_transfer(int int_param, float float_param);
EXPORT_SYMBOL void store_graph();
using namespace std;

// typedef list<float> ConfigFor6Joint; 

// template <class Config>

class VertexProperties {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & point;
        ar & config;
    }

    public:

    tuple<int,int,int> point; // 3D point address
    list<float> config;

    VertexProperties() {}

    VertexProperties(tuple<float, float, float>_point, list<float> config) {
        point = _point;
        this->config = config;
    }
};

class EdgeProperties {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & weight;
        ar & q_list;
        ar & connected;
    }
    
    public:
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

class Graph{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & nodes;
        ar & edges;
    }

    public:
    map<tuple<int, int>, EdgeProperties> edges;
    map<int, VertexProperties> nodes;

    Graph() {}

    void printVertexData(int vertexId) {
        VertexProperties vp = nodes[vertexId];

        std::cout << "Vertex data:\n\tPoint: " 
            << get<0>(vp.point)  << ","
            << get<1>(vp.point)  << ","
            << get<2>(vp.point)  << ","
            << "\n\tConfig: ";

        for (float f : vp.config) {
            std::cout << f << " ";
        }
        std::cout << std::endl;
    }
};

struct GraphTransfer {
    Graph graph;
    std::string name;

    GraphTransfer(const std::string &name);

    void add_vertex_if_not_exists(const int id, VertexProperties vertexProperties);
    void add_edge(int a, int b, EdgeProperties edgeProperties);

    void show_graph_details();

    // Load/Save graph
    void load_graph(string filename);
    void save_graph(std::string filename);
};

// #ifdef __cplusplus
// }
// #endif
