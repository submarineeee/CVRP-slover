// we have to include the following self-implemented libraries
#include <iostream>

#include <scip/scip.h>
#include <scip/scipdefplugins.h>

#include "node.h"       // needed to use struct _NODE
#include "arc.h"        // needed to use struct _ARC
#include "graph.h"      // needed to use struct _GRAPH

// defining the namespace of the instructions
using namespace std;

int main()
{
    // declare a variable to host the graph definition
    struct _GRAPH MyGraph;
    
    MyGraph.NodeList.clear();
    MyGraph.ArcList.clear();
    MyGraph.VehicleList.clear();
    
    string root_path = "/Users/ruidai/Desktop/TUD-study/Operation reserch /exam3/c++ flord_01/operation exam 3/operation exam 3/";
    string xml_file_path = root_path + "graph.xml";
    
    //  Digits
    vector<int> my_id_digits = {9, 1, 3, 5, 4};
    
    vector<int> current_digits;

    
    for(int i = 0; i < 5; i++) {
   
        cout << " Processing Iteration " << i+1 << endl;
        
        
        // read data
        MyGraph.read_data_from_xml(xml_file_path);
        
        current_digits.push_back(my_id_digits[i]);

        //task1 graph
        if (i == 4) {
                         cout << "Generating Task 1 Graph (Raw XML Data)..." << endl;
                         MyGraph.save_task1_tikz(root_path + "Task1_FinalGraph.tex", current_digits);
                    }

        // caculate length
        MyGraph.calculate_arc_length_values();

        // initializes the distance matrix to be stored in MyGraph
        MyGraph.initialize_distance_matrix();

        // print the initialized distance matrix to the console
        cout << "The distance matrix before Floyd algorithm execution" << endl;
        MyGraph.print_distance_matrix();

        // initiates the floyd algorithm to complete the distance matrix
        MyGraph.floyd_algorithm();

        // print the completed distance matrix to the screen
        cout << "The distance matrix after Floyd algorithm execution" << endl;
        MyGraph.print_distance_matrix();

        // data
        MyGraph.prepare_instance_data(current_digits);
        
        // Task 2 matrix
        cout << "Generating Task 2 Matrix..." << endl;
        MyGraph.save_task2_matrix(root_path + "Task2_Matrix.tex");

        // Task 3 CVRP  XML
        string cvrp_filename = root_path + "CVRP" + to_string(i+1) + ".xml";
        MyGraph.save_cvrp_instance(cvrp_filename, current_digits);
        
        // Task 4
        cout << " Task 4: Solving Instance " << (i+1) << "..." << endl;
        MyGraph.solve_cvrp(cvrp_filename);
        
        // Task 2 graph
        cout << "Generating Full Graph TikZ..." << endl;
        MyGraph.print_full_graph_tikz();
    }

    return 0;
}
