/* 
 * SearchAlgorithm.cpp
 */

#include "headers/SearchAlgorithm.h"
#include <iomanip>
#include <utility>
#include <cstdio>


// constructor
SearchAlgorithm::SearchAlgorithm(const Configuration& startConfig, const Configuration& goalConfig, int searchType)
    : startConfig( new Configuration(startConfig) ),
      goalConfig( new Configuration(goalConfig) ),
      searchType(searchType)
{ }

// destructor
SearchAlgorithm::~SearchAlgorithm()
{
    delete startConfig;
    delete goalConfig;
}

bool SearchAlgorithm::enqueue(Configuration *c)
{   
    int cost = 0;
    
    switch(searchType)
    {
        case ASTAR:
	    cost = c->cost(false,goalConfig); break;
        case GREEDY:
	    cost = c->cost(true,goalConfig); break;
        case DFS:
	    cost = -1*c->getDepth(); break;
        case BFS:
	    cost = c->getDepth();
	    break;
        case IDFS:
	    if(c->getDepth() > depthLimit)
		return false; // ignore sucessor, don't enqueue
	    cost = -1*c->getDepth(); break;
        default:
	    return false;
    }

    unordered_map<string,Configuration*>::const_iterator it = closedSet.find(c->toString());
    
    if(it != closedSet.end() && it->second->getDepth() <= c->getDepth() )
    {
	closedSet.erase(it);	
	return false; // ignore sucessor, don't enqueue
    }
    
    q.push( make_pair(cost,c) );
    return true; // enqueued
}

void SearchAlgorithm::enqueueAll(vector<Configuration*> cList)
{
    for(size_t i=0; i<cList.size(); i++)
	if(!enqueue(cList[i]))
	    delete cList[i];
}

bool SearchAlgorithm::search()
{
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    enqueue(startConfig);
    
    while(!q.empty())
    {
	NODE node = q.top(); q.pop();

	if(*node.second == *goalConfig)
	{
	    clock_gettime(CLOCK_MONOTONIC, &finish);
            printSolution(node.second, &start, &finish);
	    
	    return true;
	}
	
	enqueueAll(node.second->makeDescendants());
	closedSet.insert( make_pair(node.second->toString(), node.second) );
    }

    cout << "Solution not found!" << endl;
    return false;
}

void SearchAlgorithm::iterativeSearch()
{   
    depthLimit=0;
    while(!search())
    {
	depthLimit++;
	closedSet.clear();
	cout << "searching with limit: " << depthLimit << endl;
    }
}

void SearchAlgorithm::printPath(Configuration *configPtr)
{
    #ifdef DEBUG
    Configuration *currConfig = configPtr;
    while (currConfig != NULL)
    {
        getchar();
        cout << currConfig->getMove() << endl;
        cout << currConfig << endl;
        currConfig = currConfig->getParent();
    }
    #endif
    
    if (configPtr->parent != NULL)
    {
        printPath(configPtr->parent);
        cout << configPtr->getMove() << " ";
    }
    
    return;
}

void SearchAlgorithm::printSolution(Configuration *solution, struct timespec *start, struct timespec *finish)
{
    // time calculation
    double elapsed = (finish->tv_sec - start->tv_sec) + ((finish->tv_nsec - start->tv_nsec)/1000000000.0);
    
    cout << "Found solution:" << endl;
    //printPath(solution);
    cout << setprecision(2) << endl << "Depth: " << solution->getDepth() << endl << "Time: " << elapsed << " seconds" << endl;
}
