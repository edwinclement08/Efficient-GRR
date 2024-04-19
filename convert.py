"""Main file to run global redundancy resolution to find a solution for a robot"""
import os
import sys

from grr.resolution import RedundancyResolution
from grr.robot import KinematicChain, Kinova
import networkx as nx
from graph_transfer_cpp import EdgeProperties, GraphTransfer, VertexProperties

from grr.json_utils import load_json

from grr.robot import KinematicChain, Kinova
from grr.resolution import RedundancyResolution
from visualization.klampt_vis import GLRedundancyProgram

def compare_array(array1, array2):
    if len(array1) != len(array2):
        return False
    for a,b in zip(array1, array2):
        if a != b:
            return False

def save_graph(graph, filename):
    edges = list(graph.edges(data=True))
    nodes = list(graph.nodes(data=True))
    # print(filename)
    # print('nodes\t', nodes[0])
    # print('edges\t', edges[0])
            
    gt = GraphTransfer(filename)

    print("saving graph to "+filename)
    try:
        for edge in edges:
            ep = EdgeProperties(0, [], False)
            if len(edge) > 2 and len(edge[2]) >  0: # has data
                props = edge[2]
                weight = props['weight'] if 'weight' in props else 0
                q_list = list(props['q_list']) if 'q_list' in props else []
                connected = props['connected'] if 'connected' in props else False

                ep = EdgeProperties(weight, q_list, connected)
                
            gt.add_edge(edge[0], edge[1], ep)
    except Exception as e:
        print(edge)
        print(e)

    try:
        for node in nodes:
            np = VertexProperties([0.0,0.0,0.0], [])
            if len(node) > 1 and len(node[1]) >  0: # has data
                point = props['point'] if 'point' in props else [0.0,0.0,0.0]
                config = props['config'] if 'config' in props else []
                np = VertexProperties(point, config)
            gt.add_vertex_if_not_exists(node[0], np)
    except Exception as e:
        print(node)
        print(e)

    gt.show_graph_details()
    gt.save_graph(filename)
    print(f"Graph saved to {filename}")


def main(opts):
    """Main function to run the demo"""
    RobotClass = getattr(sys.modules[__name__], opts["robot_class"])
    robot = RobotClass(
        opts["robot_name"],
        opts["domain"],
        opts["rotation_domain"],
        opts["fixed_rotation"],
    )

    # Graph folder path
    graph_folder = (
        "graph/" + opts["robot_name"] + "/" + opts["problem_type"] + "/"
    )

    # Global redundancy resolution
    resolution = RedundancyResolution(robot)
    resolution.load_solver_graph(graph_folder + "graph_solver.pickle")
    resolution.load_resolution_graph(
        graph_folder + "graph_resolution.pickle",
        graph_folder + "nn_resolution.pickle",
    )
    resolution.workspace.load_workspace_graph_only(
        graph_folder + "graph_workspace.pickle",
    )

    save_graph(resolution.workspace.graph, "workspace.bin")   
    save_graph(resolution.graph, "resolution.bin")
    save_graph(resolution.solver.graph, "solver_graph.bin")

if __name__ == "__main__":
    # Default json file
    robot_name = "planar_5"
    json_file_name = "rot_free"

    # Override with system arguments if provided
    if len(sys.argv) == 2:
        print("Need to specify both the robot and the json file")
        print("python demo.py <robot> <json>")
    elif len(sys.argv) > 2:
        robot_name = sys.argv[1]
        json_file_name = sys.argv[2]

    opts = load_json(robot_name, json_file_name)
    main(opts)