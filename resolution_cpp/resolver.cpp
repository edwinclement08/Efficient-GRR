#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include "resolver.hpp"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;

NetConvert::NetConvert(const std::string &name) : name(name)
{
    std::cout << "NetConvert constructor called with name " << name << std::endl;
}

template <class Name>
class VertexWriter {
public:
     VertexWriter(Name _name) : name(_name) {}
     template <class Vertex>
     void operator()(std::ostream& out, const Vertex& v) const {
		 	auto item = name[v];
			out << "[id=\"" << item.id << "\"]";
     }
private:
     Name name;
};


NetConvert::Vertex NetConvert::add_vertex_if_not_exists(const int id){
    NetConvert::VertexItr vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi) {
        if(graph[*vi].id == id) {
            return *vi; // Found it, it's already in the graph
        }
    }

    NetConvert::Vertex v = boost::add_vertex(graph);
    graph[v].id = id;
    return v;
}


void NetConvert::add_edge(int a, int b ) {
    NetConvert::Vertex v1 = add_vertex_if_not_exists(a);
    NetConvert::Vertex v2 = add_vertex_if_not_exists(b);
    boost::add_edge(v1, v2, graph);
}

void NetConvert::show_graph_details() {
    std::cout << "Graph has " << boost::num_vertices(graph) << " vertices and " << boost::num_edges(graph) << " edges." << std::endl;
}

void NetConvert::save_graph(string filename) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }

	VertexWriter<Graph> vertexWriter(graph);
    // Write the adjacency list to the file in Graphviz DOT format
    boost::write_graphviz(outputFile, graph, vertexWriter);
    // Close the file
    outputFile.close();

    std::cout << "Graph has been stored in " << filename << " file." << std::endl;
}