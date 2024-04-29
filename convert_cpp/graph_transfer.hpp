#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>



#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/pending/property_serialize.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/tuple/tuple.hpp>

#ifdef _MSC_VER
    #define EXPORT_SYMBOL __declspec(dllexport)
#else
    #define EXPORT_SYMBOL
#endif

// #ifdef __cplusplus
// extern "C" {
// #endif

using namespace std;

// typedef list<double> ConfigFor6Joint; 

// template <class Config>

class VertexProperties {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & point;
        ar & config;
    }

    public:

    vector<double> point; // 3D point address
    vector<double> config;

    VertexProperties() {}

    VertexProperties(vector<double>_point, vector<double> _config): point(_point), config(_config){
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
    double weight;
    list<pair<int, int>> q_list;
    bool connected;

    EdgeProperties() {}

    EdgeProperties(double weight, list<pair<int, int>> q_list, bool connected) {
        this->weight = weight;
        this->q_list = q_list;
        this->connected = connected;
    }
};


typedef boost::property<boost::vertex_index1_t,int, VertexProperties> vertex_prop;
typedef boost::property<boost::edge_index_t,int, EdgeProperties> edge_prop;

typedef boost::adjacency_list_traits< boost::vecS, boost::vecS, boost::directedS > Traits;
typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, vertex_prop, edge_prop> _Graph;
typedef boost::subgraph<_Graph> Graph;


typedef boost::graph_traits<Graph>::vertex_descriptor Vertex; // Define Vertex
typedef boost::graph_traits<Graph>::vertex_iterator VertexItr; // Define Vertex iterator
typedef boost::graph_traits<Graph>::edge_descriptor Edge; // Define Edge
typedef boost::graph_traits<Graph>::edge_iterator EdgeItr; // Define Edge Iterator



namespace boost {
    namespace serialization {
         // Turn off tracking for adjacency_list. It's not polymorphic, and we
    // need to do this to enable saving of non-const adjacency lists.
    template < class OEL, class VL, class D, class VP, class EP, class GP, class EL >
    struct tracking_level<boost::subgraph<boost::adjacency_list<OEL,VL,D,VP,EP,GP,EL>>>
    {
        typedef mpl::integral_c_tag tag;
        typedef mpl::int_< track_never > type;
        BOOST_STATIC_CONSTANT(int, value = tracking_level::type::value);
    };

    template < class Archive, class OEL, class VL, class D, class VP, class EP,
        class GP, class EL >
    inline void save(Archive& ar,
        const boost::subgraph<boost::adjacency_list<OEL,VL,D,VP,EP,GP,EL>>& subGraph,
        const unsigned int /* file_version */
    )
    {
        typedef boost::subgraph<boost::adjacency_list<OEL,VL,D,VP,EP,GP,EL>> SubGraph;
        typedef typename graph_traits< SubGraph >::vertex_descriptor Vertex;

        int V = num_vertices(subGraph);
        int E = num_edges(subGraph);
        ar << BOOST_SERIALIZATION_NVP(V);
        ar << BOOST_SERIALIZATION_NVP(E);

        // assign indices to vertices
        std::map< Vertex, int > indices;
        int num = 0;
        BGL_FORALL_VERTICES_T(v, subGraph, SubGraph)
        {
            indices[v] = num++;
            ar << get(vertex_all_t(), subGraph, v);
            // ar << serialization::make_nvp( "vertex_property", get(vertex_index_t(), subGraph, v));
            // ar << serialization::make_nvp( "vertex_property", get(vertex_all_t(), subGraph, v));
        }

        // write edges
        BGL_FORALL_EDGES_T(e, subGraph, SubGraph)
        {
            ar << serialization::make_nvp("u", indices[source(e, subGraph)]);
            ar << serialization::make_nvp("v", indices[target(e, subGraph)]);

            ar << get(edge_all_t(), subGraph, e);
            // ar << serialization::make_nvp( "edge_property", get(edge_all_t(), subGraph, e));
        }

        ar << serialization::make_nvp( "graph_property", get_property(subGraph, graph_all_t()));
    }

    template < class Archive, class OEL, class VL, class D, class VP, class EP,
        class GP, class EL >
    inline void load(
        Archive& ar, boost::subgraph<boost::adjacency_list< OEL, VL, D, VP, EP, GP, EL >>& subGraph,
        const unsigned int /* file_version */
    )
    {
        typedef boost::subgraph<boost::adjacency_list< OEL, VL, D, VP, EP, GP, EL >> SubGraph;
        typedef typename graph_traits< SubGraph >::vertex_descriptor Vertex;
        typedef typename graph_traits< SubGraph >::edge_descriptor Edge;

        unsigned int V;
        ar >> BOOST_SERIALIZATION_NVP(V);
        unsigned int E;
        ar >> BOOST_SERIALIZATION_NVP(E);

        std::vector< Vertex > verts(V);
        int i = 0;
        while (V-- > 0)
        {
            Vertex v = add_vertex(subGraph);
            verts[i++] = v;
            ar >> get(vertex_all_t(), subGraph, v);
            // ar >> serialization::make_nvp( "vertex_property", get(vertex_all_t(), subGraph, v));
        }
        while (E-- > 0)
        {
            int u;
            int v;
            ar >> BOOST_SERIALIZATION_NVP(u);
            ar >> BOOST_SERIALIZATION_NVP(v);
            Edge e;
            bool inserted;
            boost::tie(e, inserted) = add_edge(verts[u], verts[v], subGraph);
            // ar >> serialization::make_nvp( "edge_property", get(edge_all_t(), subGraph, e));
            // ar >> get(edge_all_t(), subGraph, e);
        }
        ar >> serialization::make_nvp(
            "graph_property", get_property(subGraph, graph_all_t()));
    }

    template < class Archive, class OEL, class VL, class D, class VP, class EP,
        class GP, class EL >
    inline void serialize(Archive& ar,
        boost::subgraph<boost::adjacency_list< OEL, VL, D, VP, EP, GP, EL >>& subGraph,
        const unsigned int file_version)
    {
        boost::serialization::split_free(ar, subGraph, file_version);
    }

    } // namespace serialization
} // namespace boost


class GraphTransfer {
    public:
    std::string name;
    Graph graph;

    boost::property_map<Graph, boost::vertex_index1_t>::type vertex_index_map;
    boost::property_map<Graph, vector<double>VertexProperties::* >::type vertex_point_map;
    boost::property_map<Graph, vector<double>VertexProperties::* >::type vertex_config_map;

    boost::property_map<Graph, int EdgeProperties::* >::type edge_a_id_map;
    boost::property_map<Graph, int EdgeProperties::* >::type edge_b_id_map;

    boost::property_map<Graph, double EdgeProperties::* >::type edge_weight_map;
    boost::property_map<Graph, list<pair<int, int>> EdgeProperties::* >::type edge_q_list_map;
    boost::property_map<Graph, bool EdgeProperties::* >::type edge_connected_map;

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
