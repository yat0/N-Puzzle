/*
 * Configuration.cpp
 */

#include "headers/Configuration.h"


// all-member constructor
Configuration::Configuration(const Board& state, int depth, Configuration *parent)
    : state(state),
      depth(depth),
      parent(parent)
{ }

// copy constructor
Configuration::Configuration(const Configuration& c)
    : state(c.state),
      depth(c.depth),
      parent(c.parent)
{ }

// = operation support
Configuration& Configuration::operator=(const Configuration& c)
{
    state = c.state;
    depth = c.depth;

    Configuration *newptr = c.parent;
    delete parent;
    parent = newptr;

    return *this;
}

// destructor
Configuration::~Configuration()
{
    delete parent;
}

vector<Configuration*> Configuration::makeDescendants()
{
    vector<Configuration*> descendants;
    vector<Board> stateDescendants = state.makeDescendants();

    for(vector<Board>::iterator it = stateDescendants.begin(); it!=stateDescendants.end(); ++it)
    {
	Configuration *c = new Configuration(*it, depth+1, this);
	descendants.push_back( c );
    }
    
    return descendants;	    
}

bool Configuration::isSolvable(const Configuration *goal) const
{
    return state.isSolvable(goal->state);
}

int Configuration::cost(bool greedy, const Configuration *goal) const
{
    int dx = state.manhattanDist(goal->state);
    return greedy ? dx : depth + dx;
}

string Configuration::toString() const
{
    return state.toString();
}

int Configuration::getDepth() const
{
    return depth;
}

int Configuration::getMove() const
{
    return state.getMove();
}

Configuration* Configuration::getParent() const
{
    return parent;
}

ostream& operator<<(ostream& os, const Configuration& c)
{
    os << "------- Configuration -------\n\n";
    os << "Depth: " << c.depth << endl << endl;
    os << "State: \n" << c.state << endl << endl;
    
    return os;
}

bool Configuration::operator==(const Configuration& c)
{
    return state.manhattanDist(c.state) == 0;
}
