#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>

#ifdef _MSC_VER
    #define EXPORT_SYMBOL __declspec(dllexport)
#else
    #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYMBOL float graph_transfer(int int_param, float float_param);
EXPORT_SYMBOL void store_graph();
using namespace std;

struct NetConvert {
    struct MyVertex { int id; };
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, MyVertex> Graph;

//    typedef property<vertex_index_t, int> VertexProperty;
//
//    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, VertexProperty> Graph;

    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex; // Define Vertex
    typedef boost::graph_traits<Graph>::vertex_iterator VertexItr; // Define Vertex iterator
    typedef boost::graph_traits<Graph>::edge_descriptor Edge; // Define Edge
    typedef boost::graph_traits<Graph>::edge_iterator EdgeItr; // Define Edge Iterator

    Graph graph;
    std::string name;

    NetConvert(const std::string &name);

    void add_edge(int a, int b );
    Vertex add_vertex_if_not_exists(const int id);
    void show_graph_details();


    void save_graph(std::string filename);
};

#ifdef __cplusplus
}
#endif
