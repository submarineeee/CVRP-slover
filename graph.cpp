
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <limits>

#include "graph.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "arc.h"

#include <scip/scip.h>
#include <scip/scipdefplugins.h>

#define MyBigM 100000.0

using namespace std;

bool is_digit_valid(int d, const vector<int>& valid_digits) {
    for(int val : valid_digits) {
        if(val == d) return true;
    }
    return false;
}


// an additional function to convert a number to a string
std::string itos(int i) {std::stringstream s; s << i; return s.str(); }




void _GRAPH::print_node_list_on_console(void)
{
    list<struct _NODE>::iterator StoredNode;
    for( StoredNode = this->NodeList.begin() ; StoredNode != this->NodeList.end() ; StoredNode++ )
    {
        StoredNode->print_on_console();
    }
}

void _GRAPH::print_arc_list_on_console(void)
{
    list<struct _ARC>::iterator StoredArc;
    for( StoredArc = this->ArcList.begin() ; StoredArc != this->ArcList.end() ; StoredArc++ )
    {
        StoredArc->print_on_console();
    }
}

struct _NODE _GRAPH::fetch_node_from_nodelist(int _NodeId)
{
    struct _NODE TmpNode;

    list<struct _NODE>::iterator ItNode;

    for( ItNode = this->NodeList.begin() ; ItNode != this->NodeList.end() ; ItNode++ )
    {
        if( ItNode->ID == _NodeId )
        {
            TmpNode = *ItNode;
        }
    }

    return(TmpNode);
};

void _GRAPH::calculate_arc_length_values(void)
{
    list<struct _ARC>::iterator ItArc;
    for( ItArc=this->ArcList.begin() ; ItArc!=this->ArcList.end() ; ItArc++ )
    {
        ItArc->calculate_length(*this);
    }
}

void _GRAPH::initialize_distance_matrix(void)
{
    // reserve space for all rows
    this->DISTMAT = new double* [this->NodeList.size()];

    // reserve space in the memory for all colums
    for( int ROW=0 ; ROW < this->NodeList.size() ; ROW++ )
    {
        this->DISTMAT[ROW] = new double [this->NodeList.size()];
        for( int COLUMN=0 ; COLUMN < this->NodeList.size() ; COLUMN++ )
        {
            this->DISTMAT[ROW][COLUMN] = 100000.0;
        }
    }
    
    for(int i=0; i<this->NodeList.size(); i++) {
            this->DISTMAT[i][i] = 0.0;
        }

        list<struct _ARC>::iterator ArcIt;
    for( ArcIt = this->ArcList.begin() ; ArcIt != this->ArcList.end() ; ArcIt++ )
    {
        int u = get_node_index(ArcIt->from);
        int v = get_node_index(ArcIt->to);
        if (u != -1 && v != -1) {
                    this->DISTMAT[u][v] = ArcIt->length;
                }
    }

}

void _GRAPH::print_distance_matrix(void)
{
    for( int ROW=0 ; ROW < this->NodeList.size() ; ROW++ )
    {
        for( int COLUMN=0 ; COLUMN < this->NodeList.size() ; COLUMN++ )
        {
            cout << "\t" << this->DISTMAT[ROW][COLUMN];
        }
        cout << endl;
    }

}

void _GRAPH::floyd_algorithm(void)
{
    
    int size = (int)this->NodeList.size();

    for( int k=0 ; k < size ; k++ )
    {
        for( int i=0 ; i < size ; i++ )
        {
            for( int j=0 ; j < size ; j++ )
            {
                if (this->DISTMAT[i][k] + this->DISTMAT[k][j] < this->DISTMAT[i][j])
                {
                    this->DISTMAT[i][j] = this->DISTMAT[i][k] + this->DISTMAT[k][j];
                }
            }
        }
    }
}

void _GRAPH::reset(void)
{
    // dismantle the existing distance matrix
    if( this->DISTMAT != nullptr )
    {
        for( int row=0 ; row < this->NodeList.size() ; row++ )
        {
            if( this->DISTMAT[row] != nullptr )
                delete[] this->DISTMAT[row];
        }
        delete[] this->DISTMAT;
        this->DISTMAT = nullptr;
    }

    // clear the existing lists
    this->NodeList.clear();
    this->ArcList.clear();
    this->VehicleList.clear();

}



void _GRAPH::read_data_from_xml(string _XML_FILE_NAME)
{
    this->reset();
    pugi::xml_document XMLFILE;
    
    pugi::xml_parse_result result = XMLFILE.load_file(_XML_FILE_NAME.c_str());
   
    
    pugi::xml_node
    FIRST_NODE = XMLFILE.child("problem_data").child("graph_data").child("node_data").child("node");

    // now iterate over all nodes stored in _XML_FILE_NAME
    for( pugi::xml_node MyNode=FIRST_NODE ; MyNode ; MyNode = MyNode.next_sibling("node") )
   {
       std::cout << "ID of found node = " << MyNode.attribute("ID").as_int() << std::endl;
    std::cout << "x-coordinate of found node = " << MyNode.attribute("x").as_double() << std::endl;
        std::cout << "y-coordinate of found node = " << MyNode.attribute("y").as_double() << std::endl;

        // declare a struct _NODE variable for storing the found attributes
        struct _NODE TmpNode;
        TmpNode.ID = MyNode.attribute("ID").as_int();
        TmpNode.x = MyNode.attribute("x").as_double();
        TmpNode.y = MyNode.attribute("y").as_double();
        TmpNode.digit = MyNode.attribute("digit").as_int();

        // add the recently found found to the list of nodes
        this->NodeList.push_back(TmpNode);
    }
    cout << "Nodes number: " << this->NodeList.size() << endl;

        // 4. Read ARCS
        pugi::xml_node FIRST_ARC = XMLFILE.child("problem_data").child("graph_data").child("arc_data").child("arc");

        for( pugi::xml_node MyArc=FIRST_ARC ; MyArc ; MyArc = MyArc.next_sibling("arc") )
        {
            struct _ARC TmpArc;
            TmpArc.ID = MyArc.attribute("ID").as_int();
            TmpArc.from = MyArc.attribute("from").as_int();
            TmpArc.to = MyArc.attribute("to").as_int();
            TmpArc.length = MyArc.attribute("length").as_double();

            this->ArcList.push_back(TmpArc);
        }
        
        cout << "Arcs number: " << this->ArcList.size() << endl;
        this->print_arc_list_on_console();
    
        // 5. vehicle
        pugi::xml_node FIRST_VEHICLE = XMLFILE.child("problem_data").child("vehicles").child("vehicle");

        for( pugi::xml_node MyVehNode = FIRST_VEHICLE ; MyVehNode ; MyVehNode = MyVehNode.next_sibling("vehicle") )
        {
            struct _VEHICLE TmpVeh;
            
            TmpVeh.ID = MyVehNode.attribute("ID").as_int();
            
            if (!MyVehNode.attribute("capacity").empty())
                 TmpVeh.capacity = MyVehNode.attribute("capacity").as_double();

            this->VehicleList.push_back(TmpVeh);
        }

        std::cout << "VehicleList after reading data from xml file" << std::endl;
        this->print_vehicle_list_on_console();
        
    
    }




int _GRAPH::get_node_index(int _id)
{
    int index = 0;
    for(auto& node : this->NodeList)
    {
        if(node.ID == _id) return index;
        index++;
    }
    return -1;
}



void _GRAPH::prepare_instance_data(vector<int> valid_digits)
{
    for(auto& node : this->NodeList) {
            node.is_depot = false;
            node.demand = 0.0;
        }

        list<struct _NODE*> active_nodes;
        for(auto& node : this->NodeList) {
            if(is_digit_valid(node.digit, valid_digits)) {
                active_nodes.push_back(&node);
            }
        }

        // node
        active_nodes.sort([](_NODE* a, _NODE* b) {
            return a->ID < b->ID;
        });
    
        vector<double> demand_pattern = {0.25, 0.5, 0.75, 1.0, 0.75, 0.5, 0.25, 0.5, 0.75};
        int pattern_idx = 0;
        bool first_node = true;
    
        for (auto* node_ptr : active_nodes) {
            if (first_node) {
                node_ptr->is_depot = true;
                node_ptr->demand = 0.0;
                first_node = false;
            } else {
                node_ptr->is_depot = false;
                node_ptr->demand = demand_pattern[pattern_idx];
                
                pattern_idx++;
                if (pattern_idx >= demand_pattern.size()) pattern_idx = 0;
            }
        }

       //Task 3: 5 trucks, payload 15
        this->VehicleList.clear();
        for(int k=0; k<5; k++) {
            struct _VEHICLE truck;
            truck.ID = k;
            truck.capacity = 15.0;
            this->VehicleList.push_back(truck);
        }
 
}




void _GRAPH::save_cvrp_instance(string filename, vector<int> valid_digits)
{
    ofstream out(filename);

    if (valid_digits.empty()) return;

    cout << "Saving CVRP instance to: " << filename << endl;

    // 1. Depot Logic
    int depot_node_id = -1;
    int min_id_found = 999999;
    
    vector<_NODE*> active_nodes;

    for(auto& node : this->NodeList) {
        bool is_valid = false;
        for(int d : valid_digits) if(node.digit == d) is_valid = true;

        if (is_valid) {
            active_nodes.push_back(&node);
            if (node.ID < min_id_found) {
                min_id_found = node.ID;
                depot_node_id = node.ID;
            }
        }
    }



    out << "<problem_data>" << endl;
    
    // 2. Vehicles
    out << "\t<vehicles>" << endl;
    for(int k=0; k<5; k++) {
        out << "\t\t<vehicle ID=\"" << k << "\" capacity=\"15\" />" << endl;
    }
    out << "\t</vehicles>" << endl;

    // 3.Graph Data
    out << "\t<graph_data>" << endl;
    
    // (1) Nodes
    out << "\t\t<node_data>" << endl;
    for(auto* node : active_nodes) {
        out << "\t\t\t<node ID=\"" << node->ID << "\" x=\"" << node->x
            << "\" y=\"" << node->y << "\" digit=\"" << node->digit << "\" />" << endl;
    }
    out << "\t\t</node_data>" << endl;

    // (2) Arcs
    out << "\t\t<arc_data>" << endl;
    int arc_count = 0;
    
    for(auto* n1 : active_nodes) {
        for(auto* n2 : active_nodes) {
            if(n1->ID == n2->ID) continue;

            // Floyd
            int idx1 = this->get_node_index(n1->ID);
            int idx2 = this->get_node_index(n2->ID);
            
            double dist = 100000.0;
            if (this->DISTMAT != nullptr && idx1 >= 0 && idx2 >= 0) {
                dist = this->DISTMAT[idx1][idx2];
            }

            
            if (dist < 90000.0) {
                out << "\t\t\t<arc ID=\"" << arc_count++ << "\" from=\"" << n1->ID
                    << "\" to=\"" << n2->ID << "\" length=\"" << dist << "\" />" << endl;
            }
        }
    }
    out << "\t\t</arc_data>" << endl;
    out << "\t</graph_data>" << endl;

    // 4. Depot
    out << "\t<depots>" << endl;
    out << "\t\t<depot node_id=\"" << depot_node_id << "\" />" << endl;
    out << "\t</depots>" << endl;

    // 5.  Requests
    out << "\t<requests>" << endl;
    int req_id = 0;
    for(auto* node : active_nodes) {
        if(node->ID != depot_node_id) {
            double demand = (node->demand > 0.001) ? node->demand : 1.0;
            out << "\t\t<request ID=\"" << req_id++ << "\" node_id=\"" << node->ID
                << "\" demand=\"" << demand << "\" />" << endl;
        }
    }
    out << "\t</requests>" << endl;

    out << "</problem_data>" << endl;
    out.close();
}


void _GRAPH::solve_cvrp(string xml_filename) {
    cout << " Solving CVRP for " << xml_filename  << endl;
    
    // 1. Reset & Load Data
    this->reset();
    
    pugi::xml_document doc;
    if (!doc.load_file(xml_filename.c_str())) {
        cout << "Error: Failed to load XML file: " << xml_filename << endl;
        return;
    }
    
    // Load Vehicles
    pugi::xml_node root = doc.child("problem_data");
    double vehicle_capacity = 15.0;
    pugi::xml_node veh_node = root.child("vehicles").child("vehicle");
    if(veh_node) vehicle_capacity = veh_node.attribute("capacity").as_double();
    
    // Load Nodes
    for (pugi::xml_node n = root.child("graph_data").child("node_data").child("node"); n; n = n.next_sibling("node")) {
        _NODE tmp;
        tmp.ID = n.attribute("ID").as_int();
        tmp.x = n.attribute("x").as_double();
        tmp.y = n.attribute("y").as_double();
        tmp.digit = n.attribute("digit").as_int();
        
        tmp.is_depot = false;
        tmp.demand = 0.0;
        this->NodeList.push_back(tmp);
    }
    
    // Identify Depot
    int depot_id = -1;
    for (pugi::xml_node d = root.child("depots").child("depot"); d; d = d.next_sibling("depot")) {
        int id = d.attribute("node_id").as_int();
        for(auto& node : this->NodeList) {
            if(node.ID == id) {
                node.is_depot = true;
                depot_id = id;
                break;
            }
        }
    }
    
    // Load Requests
    for (pugi::xml_node r = root.child("requests").child("request"); r; r = r.next_sibling("request")) {
        int id = r.attribute("node_id").as_int();
        double dem = r.attribute("demand").as_double();
        for(auto& node : this->NodeList) if(node.ID == id) node.demand = dem;
    }
    
    // Load Arcs
    for (pugi::xml_node a = root.child("graph_data").child("arc_data").child("arc"); a; a = a.next_sibling("arc")) {
        _ARC tmp;
        tmp.ID = a.attribute("ID").as_int();
        tmp.from = a.attribute("from").as_int();
        tmp.to = a.attribute("to").as_int();
        tmp.length = a.attribute("length").as_double();
        this->ArcList.push_back(tmp);
    }
    
    // 2. Prepare Distance Matrix
    this->initialize_distance_matrix();
    this->floyd_algorithm();
    
    // Create Index Mappings
    vector<int> index_to_id;
    map<int, int> id_to_index;
    int idx = 0;
    int depot_idx = -1;
    
    for(auto& node : this->NodeList) {
        index_to_id.push_back(node.ID);
        id_to_index[node.ID] = idx;
        if(node.is_depot) depot_idx = idx;
        idx++;
    }
    int num_nodes = index_to_id.size();
    
    // 3. SCIP Modeling
    SCIP* scip = nullptr;
    SCIPcreate(&scip);
    SCIPincludeDefaultPlugins(scip);
    SCIPcreateProbBasic(scip, "CVRP_Instance");
    SCIPsetRealParam(scip, "limits/time", 7200.0); // Time limit
    
    // Variables
    vector<vector<SCIP_VAR*>> x(num_nodes, vector<SCIP_VAR*>(num_nodes, nullptr));
    vector<SCIP_VAR*> u(num_nodes, nullptr);
    char var_name[255];
    
    // Create X variables (Routing)
    for(int i=0; i<num_nodes; i++) {
        for(int j=0; j<num_nodes; j++) {
            if (i == j) continue;
            
            // Optimization: Skip edges that are essentially disconnected
            if (this->DISTMAT[i][j] > 90000.0) {
                x[i][j] = nullptr;
                continue;
            }
            
            sprintf(var_name, "x_%d_%d", index_to_id[i], index_to_id[j]);
            double cost = this->DISTMAT[i][j];
            SCIPcreateVarBasic(scip, &x[i][j], var_name, 0.0, 1.0, cost, SCIP_VARTYPE_BINARY);
            SCIPaddVar(scip, x[i][j]);
        }
    }
    
    // Create U variables
    for(int i=0; i<num_nodes; i++) {
        sprintf(var_name, "u_%d", index_to_id[i]);
        double node_demand = 0.0;
        bool is_current_node_depot = false;
        
        for(auto& n : this->NodeList) {
            if(n.ID == index_to_id[i]) {
                node_demand = n.demand;
                is_current_node_depot = n.is_depot;
                break;
            }
        }
        double lower_bound = is_current_node_depot ? 0.0 : node_demand;
        double upper_bound = 15.0;
        SCIPcreateVarBasic(scip, &u[i], var_name, lower_bound, upper_bound, 0.0, SCIP_VARTYPE_CONTINUOUS);
        SCIPaddVar(scip, u[i]);
    }
    
    // Constraints
    // 1.Visit each customer exactly once
    for(int j=0; j<num_nodes; j++) {
        if(j == depot_idx) continue;
        
        sprintf(var_name, "In_Degree_%d", index_to_id[j]);
        SCIP_CONS* cons = nullptr;
        SCIPcreateConsLinear(scip, &cons, var_name, 0, nullptr, nullptr, 1.0, 1.0, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        
        for(int i=0; i<num_nodes; i++) {
            if(i!=j && x[i][j] != nullptr) SCIPaddCoefLinear(scip, cons, x[i][j], 1.0);
        }
        SCIPaddCons(scip, cons);
        SCIPreleaseCons(scip, &cons);
    }
    
    // 2. Out-Degree = 1
    for(int i=0; i<num_nodes; i++) {
        if(i == depot_idx) continue;
        
        sprintf(var_name, "Out_Degree_%d", index_to_id[i]);
        SCIP_CONS* cons = nullptr;
        SCIPcreateConsLinear(scip, &cons, var_name, 0, nullptr, nullptr, 1.0, 1.0, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        
        for(int j=0; j<num_nodes; j++) {
            if(i!=j && x[i][j] != nullptr) SCIPaddCoefLinear(scip, cons, x[i][j], 1.0);
        }
        SCIPaddCons(scip, cons);
        SCIPreleaseCons(scip, &cons);
    }
    
    // Depot Constraints (Vehicle Limits & Flow Balance)
    {
        // A. Max Vehicles
        sprintf(var_name, "Depot_Vehicle_Limit");
        SCIP_CONS* cons = nullptr;
        SCIPcreateConsLinear(scip, &cons, var_name, 0, nullptr, nullptr, -SCIPinfinity(scip), 5.0, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        
        for(int j=0; j<num_nodes; j++) {
            if(j != depot_idx && x[depot_idx][j] != nullptr) {
                SCIPaddCoefLinear(scip, cons, x[depot_idx][j], 1.0);
            }
        }
        SCIPaddCons(scip, cons);
        SCIPreleaseCons(scip, &cons);
        
        // B. Flow Balance at Depot
        sprintf(var_name, "Depot_Flow_Balance");
        cons = nullptr;
        SCIPcreateConsLinear(scip, &cons, var_name, 0, nullptr, nullptr, 0.0, 0.0, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        
        for(int k=0; k<num_nodes; k++) {
            if(k != depot_idx) {
                if(x[depot_idx][k] != nullptr) SCIPaddCoefLinear(scip, cons, x[depot_idx][k], 1.0); // Leave
                if(x[k][depot_idx] != nullptr) SCIPaddCoefLinear(scip, cons, x[k][depot_idx], -1.0); // Return
            }
        }
        SCIPaddCons(scip, cons);
        SCIPreleaseCons(scip, &cons);
    }
    
    // 3. MTZ Subtour Elimination
    double BigM = 100000.0;
    
    for(int i=0; i<num_nodes; i++) {
        for(int j=0; j<num_nodes; j++) {
            if(i == j || i == depot_idx || j == depot_idx) continue;
            
            if (x[i][j] == nullptr) continue;
            
            double demand_j = 0.0;
            for(auto& n : this->NodeList) {
                if(n.ID == index_to_id[j]) {
                    demand_j = n.demand;
                    break;
                }
            }
            
            
            
            sprintf(var_name, "MTZ_%d_%d", index_to_id[i], index_to_id[j]);
            SCIP_CONS* cons = nullptr;
            
            double rhs = BigM - demand_j;
            
            SCIPcreateConsLinear(scip, &cons, var_name, 0, nullptr, nullptr, -SCIPinfinity(scip), rhs, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
            
            SCIPaddCoefLinear(scip, cons, u[i], 1.0);
            SCIPaddCoefLinear(scip, cons, u[j], -1.0);
            
            SCIPaddCoefLinear(scip, cons, x[i][j], BigM);
            
            SCIPaddCons(scip, cons);
            SCIPreleaseCons(scip, &cons);
        }
    }
    // 4. Load Lower Bound (u_i >= d_i)
    for(int i=0; i<num_nodes; i++) {
        if (i == depot_idx) continue;
        double demand_i = 0.0;
        for(auto& n : this->NodeList) if(n.ID == index_to_id[i]) { demand_i = n.demand; break; }
        
        sprintf(var_name, "Load_LB_%d", index_to_id[i]);
        SCIP_CONS* cons = nullptr;
        SCIPcreateConsLinear(scip, &cons, var_name, 0, nullptr, nullptr, demand_i, SCIPinfinity(scip), TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        SCIPaddCoefLinear(scip, cons, u[i], 1.0);
        SCIPaddCons(scip, cons);
        SCIPreleaseCons(scip, &cons);
    }
    
    // Solving
    SCIPsolve(scip);
    
    // Result Retrieval
    SCIP_SOL* sol = SCIPgetBestSol(scip);
    if (sol) {
            double obj = SCIPgetSolOrigObj(scip, sol);
            cout << " Optimal Found! Cost: " << obj << endl;
            
            //vehicle
            int vehicles_used = 0;
            cout << endl << " --- Detailed Route Report ---" << endl;
            
            if (depot_idx != -1) {
                for (int j = 0; j < num_nodes; j++) {
                    if (j != depot_idx && x[depot_idx][j] != nullptr && SCIPgetSolVal(scip, sol, x[depot_idx][j]) > 0.5) {
                        vehicles_used++;
                        
                        cout << " Vehicle " << vehicles_used << ": " << index_to_id[depot_idx];
                        double route_len = 0.0;
                        int curr = j;
                        
                        route_len += this->DISTMAT[depot_idx][curr];
                        
                        while(curr != depot_idx) {
                            cout << " -> " << index_to_id[curr];
                            
                            int next = -1;
                            for(int k=0; k<num_nodes; k++) {
                                if(x[curr][k] != nullptr && SCIPgetSolVal(scip, sol, x[curr][k]) > 0.5) {
                                    next = k;
                                    break;
                                }
                            }
                            
                            if(next != -1) {
                                route_len += this->DISTMAT[curr][next];
                                curr = next;
                            } else {
                                cout << " -> [Path Broken]";
                                break;
                            }
                        }
                        cout << " -> " << index_to_id[depot_idx];
                        cout << " | Length: " << route_len << endl;
                    }
                }
            }
            cout << " Number of Used Vehicles: " << vehicles_used << endl;
            cout << " ---------------------------------" << endl;
            
            //
            cout << endl << "%%%%%%%%%%%%%%%%% Task 4 Solution TikZ %%%%%%%%%%%%%%%%%" << endl;
            cout << "\\begin{tikzpicture}[scale=0.8]" << endl;
            cout << "  \\draw[help lines, step=2, color=gray!20] (0,0) grid (20,15);" << endl;
            
            // Routes
            cout << "  % Optimal Routes (Drawn FIRST so nodes are on top)" << endl;
            for(int i=0; i<num_nodes; i++) {
                for(int j=0; j<num_nodes; j++) {
                    if(x[i][j] != nullptr) {
                        if(SCIPgetSolVal(scip, sol, x[i][j]) > 0.5) {
                            int id_from = index_to_id[i];
                            int id_to = index_to_id[j];
                            
                        
                            cout << "  \\draw[->, line width=1.5pt, color=red] (NODE"
                                 << id_from << ") -- (NODE" << id_to << ");" << endl;
                        }
                    }
                }
            }

            // Nodes
            cout << "  % Nodes (Drawn SECOND)" << endl;
            for(auto& node : this->NodeList) {
                node.print_tikz();
            }

            cout << "\\end{tikzpicture}" << endl;
            cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl << endl;
        }
        
        // Release SCIP vars (Cleanup)
        for(int i=0; i<num_nodes; i++) {
            if(u[i]) SCIPreleaseVar(scip, &u[i]);
            for(int j=0; j<num_nodes; j++) {
                if(x[i][j]) SCIPreleaseVar(scip, &x[i][j]);
            }
        }
        SCIPfree(&scip);
    }


void _GRAPH::print_vehicle_list_on_console(void)
{
    cout << "--- Vehicle List ---" << endl;
    list<struct _VEHICLE>::iterator StoredVehicle;
    for( StoredVehicle = this->VehicleList.begin() ; StoredVehicle != this->VehicleList.end() ; StoredVehicle++ )
    {
        StoredVehicle->print_on_console();
    }
    cout << "--------------------" << endl;
}

//latex print for task1
void _GRAPH::save_task1_tikz(string filename, const vector<int>& valid_digits)
{
    cout << endl << "%%%%%%%%%%%%%%%%% Task 1 TikZ Output %%%%%%%%%%%%%%%%%" << endl;
    cout << "% Copy this into your .tex file" << endl;
    cout << "\\begin{tikzpicture}[scale=0.8]" << endl;
    
    // 1. draw axis
    cout << "  \\draw[help lines, color=gray!20] (0,0) grid (20,15);" << endl;

    // 2.print node
    for(auto& node : this->NodeList) {
        bool is_valid = false;
        if(valid_digits.empty()) is_valid = true;
        else { for(int d : valid_digits) if(node.digit == d) is_valid = true; }
        
        if(is_valid) {
            node.print_tikz(); // from node.cpp
        }
    }

    // 3.print Arc
    for(auto& arc : this->ArcList) {
        bool from_valid = false, to_valid = false;
        if(valid_digits.empty()) { from_valid = true; to_valid = true; }
        else {
             int f_digit = -1, t_digit = -1;
             for(auto& n : NodeList) {
                 if(n.ID == arc.from) f_digit = n.digit;
                 if(n.ID == arc.to) t_digit = n.digit;
             }
             for(int d : valid_digits) {
                 if(f_digit == d) from_valid = true;
                 if(t_digit == d) to_valid = true;
             }
        }

        if(from_valid && to_valid) {
            arc.print_tikz();
        }
    }

    cout << "\\end{tikzpicture}" << endl;
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl << endl;
}

void _GRAPH::save_task2_matrix(string filename)
{
    ofstream out(filename);
    
    cout << "Generating Task 2 Matrix to: " << filename << endl;
    
    // 1. Convert List to Vector for easy index access
    vector<int> node_ids;
    for(auto& node : this->NodeList) {
        node_ids.push_back(node.ID);
    }

    // 2. Use \bordermatrix for labeled rows/cols
    // Note: bordermatrix syntax uses \cr instead of \\ for new lines
    
    int limit = min((int)this->NodeList.size(), 27);
    
    // --- Print Header Row (First Row of IDs) ---
    out << "\t& "; // Top-left corner is empty
    for(int j=0; j<limit; j++) {
        out << node_ids[j];
        if(j < limit - 1) out << " & ";
    }
    out << " \\cr" << endl; // End of header row
    
    // --- Print Data Rows (ID + Distances) ---
    for(int i=0; i<limit; i++) {
        // Print Row Label (First Column)
        out << node_ids[i] << " & ";
        
        // Print Row Data
        for(int j=0; j<limit; j++) {
            double val = this->DISTMAT[i][j];
            if(val >= 90000) out << "\\infty";
            else out << val;
            
            if(j < limit - 1) out << " & ";
        }
        out << " \\cr" << endl; // End of data row
    }
    
    // Handle truncation if matrix is too large
    if(this->NodeList.size() > limit) {
        out << "\\vdots & \\vdots & \\ddots & \\vdots \\cr" << endl;
    }
    
    out << "} \\]" << endl; // Close bordermatrix
    out.close();
}



//print full graph
void _GRAPH::print_full_graph_tikz(void)
{
    cout << endl << "% --- Start of TikZ Graph ---" << endl;
    cout << "\\begin{tikzpicture}[scale=0.8]" << endl;
    
    // 1. Print Arcs first (so they appear behind nodes)
    list<struct _ARC>::iterator ItArc;
    for( ItArc = this->ArcList.begin() ; ItArc != this->ArcList.end() ; ItArc++ )
    {
        ItArc->print_tikz();
    }

    // 2. Print Nodes
    list<struct _NODE>::iterator ItNode;
    for( ItNode = this->NodeList.begin() ; ItNode != this->NodeList.end() ; ItNode++ )
    {
        ItNode->print_tikz();
    }

    cout << "\\end{tikzpicture}" << endl;
    cout << "% --- End of TikZ Graph ---" << endl << endl;
}
