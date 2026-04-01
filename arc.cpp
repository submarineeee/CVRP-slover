#include <iostream>         // needed to use cout etc.
#include <cmath>
#include "arc.h"            // needed to use struct _ARC
#include "graph.h"

using namespace std;

void _ARC::set_values(int _id,int from_id, int to_id,double length)
{
    this->ID = _id;
    this->from = from_id;
    this->to = to_id;
    this->length = 0.0;
}
void _ARC::print_on_console()
{
    cout << "Arc id:" << this -> ID
        << "\t" << this-> from
        << "\t" << this->to
        << "\t" << this->length << endl;
}
void _ARC::calculate_length(struct _GRAPH& MyGraph)
{
    struct _NODE ORIG = MyGraph.fetch_node_from_nodelist(this->from);
    struct _NODE DEST = MyGraph.fetch_node_from_nodelist(this->to);

    double diff_x = ORIG.x - DEST.x;
    double diff_y = ORIG.y - DEST.y;

    this->length = sqrt ( diff_x * diff_x + diff_y * diff_y );
}

void _ARC::print_tikz(void)
{
  
    std::cout << "  \\draw[->] (NODE"
         << this->from
         << ") -- (NODE"
         << this->to
         << ");" << std::endl;
}
