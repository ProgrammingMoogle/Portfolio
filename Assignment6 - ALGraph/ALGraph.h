/******************************************************************************/
/*!
\file   ALGraph.h
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #6
\date   7/27/2017
\brief
This is the header file for ALGraph.cpp.
*/
/******************************************************************************/
//---------------------------------------------------------------------------
#ifndef ALGRAPHH
#define ALGRAPHH
//---------------------------------------------------------------------------
#include <vector>
#include <list> /* To maintain our edge lists. */
#include <queue> /* Priority queue. */
#include <algorithm> // sort
#include <climits> // UINT_MAX

/*!
  Information used to communicate with the driver after Dijkstra's algorithm
  has run. There will typically be one of these per node in the graph.
*/
struct DijkstraInfo
{
  unsigned cost;              //!< The cost to reach this node
  std::vector<unsigned> path; //!< The shortest path that led to this node
};

/*!
  Stores information for the adjacency lists
*/
struct AdjacencyInfo
{
  unsigned id;     //!< The node's unique ID
  unsigned weight; //!< The weigth of the edge leading to this node
};

//! Convenient shorthand for an adjacency list type
typedef std::vector<std::vector<AdjacencyInfo> > ALIST;

/*!
  Class that represents a graph.
*/
class ALGraph
{
  public:
    //! The different types of traversals
    enum TRAVERSAL_METHOD {DEPTH_FIRST, BREADTH_FIRST};

    ALGraph(unsigned size);
    ~ALGraph();
    void AddDEdge(unsigned source, unsigned destination, unsigned weight);
    void AddUEdge(unsigned node1, unsigned node2, unsigned weight);

    std::vector<unsigned> SearchFrom(unsigned start_node, TRAVERSAL_METHOD method) const;
    std::vector<DijkstraInfo> Dijkstra(unsigned start_node) const;
    ALIST GetAList() const;
    static bool ImplementedSearches();

  private:
    // Private fields and methods ...
    class GEdge
    {
      unsigned mDest; //!< the destination
      unsigned mWT; //!< the weight

    public:
      GEdge();
      GEdge(unsigned dest, unsigned wt);

      GEdge& SetWT(unsigned new_wt);
      GEdge& SetDest(unsigned new_dest); // Sets the destination

      unsigned GetWT() const;
      unsigned GetDest() const;
    };
    typedef std::list<GEdge> EdgeList;

    class GNode
    {
      EdgeList mEdges; //!< the edges

    public:
      GNode();

      void addEdge(unsigned dest, unsigned wt);
      const EdgeList& GetEList() const;
    };

    struct AdjInfo
    {
      unsigned mNode; //!< the node
      unsigned mCost; //!< the cost of the node 
      AdjInfo();
      bool operator<(const AdjInfo& rhs) const;
    };

    typedef std::vector<GNode> NodeList;

    NodeList mNodes; //!< the list of nodes

    AdjInfo MakeAdjInfo(unsigned source, unsigned curr_cost) const;
};

#endif
