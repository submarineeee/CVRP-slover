#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

struct _GRAPH;


struct _ARC {

    // attributes
    int ID;
    int from;
    int to;
    double length;

    // method prototypes

    // set the attributes for arc ID, originnode ID and destinationnode ID
    void set_values(int _id, int from_id, int to_id,double length);
    // print this arc to the console
    void print_on_console(void);
    // determines the Euclidean Distance of this arc. It access the nodelist stored in _MyGraph
    void calculate_length(struct _GRAPH& _MyGraph);
    void print_tikz(void);
};

#endif // ARC_H_INCLUDED
