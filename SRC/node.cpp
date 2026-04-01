#include <iostream>
#include "node.h"

using namespace std;

void _NODE::set_values(int _id, int _x, int _y)
{
    this->ID = _id;
    this->x = _x;
    this->y = _y;
    this-> demand =0.0;
    this-> is_depot =false;
}

void _NODE::set_demand(double _demand)
{
    this->demand = _demand;
}

void _NODE::print_on_console(void)
{
    cout << "Node ID: "<<"\t"<< this-> ID
        << "\t X: " << this->x
        << "\t Y: " << this->y
        << "\t Demand: " << this->demand
        << "\t Type: " << (this->is_depot ? "Depot" : "Customer")
        << endl;
}

void _NODE::print_tikz(void)
{
    // determine depot
    string color = this->is_depot ? "red!70" : "blue!50";
    string fill = this->is_depot ? "red!20" : "white";
    string shape = this->is_depot ? "rectangle" : "circle";

    // NODE + ID
   std::cout << "  \\node[draw="
         << color
         << ", fill="
         << fill
         << ", "
         << shape
         << ", inner sep=1.5pt, minimum size=0.5cm] "
         << "(NODE"
         << this->ID
         << ") at (" 
         << this->x
         << ","
         << this->y
         << ") "
         << "{\\tiny "
         << this->ID
         << "};"
         <<std::endl;
}
