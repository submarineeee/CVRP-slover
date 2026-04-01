#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <list>
#include <string>
#include <vector>     
#include <iostream>
#include <map>
#include <fstream>

#include "node.h"
#include "arc.h"
#include "vehicle.h"


using namespace std;



struct _GRAPH {

    // attributes
    list<struct _NODE> NodeList;
    list<struct _ARC> ArcList;
    //vehicle
    list<struct _VEHICLE> VehicleList;

    double **DISTMAT = nullptr;           // two-dimensional distance matrix
    
    
    int get_node_index(int _id);
    // method prototypes
    // print all stored nodes to the console
    void print_node_list_on_console(void);
    // print all stored arcs to the console
    void print_arc_list_on_console(void);
    // function that return a copy from a stored node with ID==_NodeId
    struct _NODE fetch_node_from_nodelist(int _NodeId);
    // function to calculate the Euclidean Distance for all stored arcs
    void calculate_arc_length_values(void);
    // initializes the distance matrix DISTMAT and fills it with a large value
    // after initialization: this functions transfers the length of stored arcs to this matrix
    void initialize_distance_matrix(void);
    // print the distance matrix to the console
    void print_distance_matrix(void);
    // initiates the floyd algorithm to complete the distance matrix
    void floyd_algorithm(void);
    void prepare_instance_data(vector<int> valid_digits);
    

    // removes all stored data from graph structure
    void reset(void);
    // reads XML-file with graph data
    void read_data_from_xml(string _XML_FILE_NAME);
    void save_cvrp_instance(string filename, vector<int> valid_digits);

    //task4 CVRP solving
    void solve_cvrp(string filename);
    //vehicle
    void print_vehicle_list_on_console(void);
    
    
    //for latex task1
    void save_task1_tikz(string filename, const vector<int>& valid_digits);
    //for latex task2 matrix
    void save_task2_matrix(string filename);
    //for latex task2 graph
    void print_full_graph_tikz(void);

};

#endif // GRAPH_H_INCLUDED
