#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

struct _NODE {

    // attributes
    int ID;
    int x;
    int y;
    int digit;
    
    double demand=0.0;
    bool is_depot = false;

    //method prototypes
    // sets the ID and the coordinates of this node
    void set_values(int _id, int _x, int _y);
    void set_demand(double _demand);
    // print the node data to the console
    void print_on_console(void);
    // print the node data in the tikz-style
    void print_tikz(void);
};

#endif // NODE_H_INCLUDED
