/******************************************************************************/
/*!
\file   ALGraph.cpp
\author Kacey Lei Yeo
\par    email: yeo.k\@digipen.edu
\par    DigiPen login: yeo.k
\par    Course: CS280
\par    Assignment #6
\date   7/27/2017
\brief
  This is the implementation for an adjacency-list based graph class with
  internal support for Dijkstra's algorithm for calculating shortest paths.

*/
/******************************************************************************/
#include "ALGraph.h"

namespace
{
/******************************************************************************/
/*!
 *  \brief
 *      Helper function to create an AdjacencyInfo in a reliable way.
 *
 *  \param ID
 * 	    The destination node ID.
 *
 *  \param WT
 *      The edge weight.
 *
 *  \return
 *      An AdjacencyInfo struct containing values.
 *
 */
/******************************************************************************/
  AdjacencyInfo createInfo(unsigned ID, unsigned WT)
  {
    AdjacencyInfo returner = { ID, WT };

    return returner;
  }

/******************************************************************************/
/*!
 *  \brief
 *      Helper function to create an AdjacencyInfo in a reliable way.
 *
 *  \param first
 * 	    The first value being tested.
 *
 *  \param second
 *      The second value being tested.
 *
 *  \return
 *      true if the values are in order, false otherwise.
 *
 */
/******************************************************************************/
  bool AListComp(const AdjacencyInfo &first, const AdjacencyInfo &second)
  {
    if (first.weight < second.weight)
    {
      return true;
    }
    else
    {
      if (first.weight == second.weight)
      {
        if (first.id < second.id)
        {
          return true;
        }
      }
      return false;
    }
  }
}

/******************************************************************************/
/*!
 *  \brief
 *      A constructor.
 *
 *  \param size
 * 	    The number of nodes to create for the graph.
 *
 */
/******************************************************************************/
ALGraph::ALGraph(unsigned size) : mNodes(size)
{
}

/******************************************************************************/
/*!
 *  \brief
 *      A deconstructor.
 *
 */
/******************************************************************************/
ALGraph::~ALGraph()
{
}

/******************************************************************************/
/*!
 *  \brief
 *      Adds a directed edge to the graph.
 *
 *  \param source
 * 	    The source node the edge starts from.
 *
 *  \param destination
 *      The destination node the edge leads to.
 *
 *  \param weight
 *      The weight of the edge.
 *
 */
/******************************************************************************/
void ALGraph::AddDEdge(unsigned source, unsigned destination, unsigned weight)
{
  mNodes[source - 1].addEdge(destination - 1, weight);
}

/******************************************************************************/
/*!
 *  \brief
 *      Adds an undirected edge to the graph.
 *
 *  \param node1
 * 	    The first node to connect.
 *
 *  \param node2
 *      The second node to connect.
 *
 *  \param weight
 *      The weight of the edges added.
 *
 */
/******************************************************************************/
void ALGraph::AddUEdge(unsigned node1, unsigned node2, unsigned weight)
{
  AddDEdge(node1, node2, weight);
  AddDEdge(node2, node1, weight);
}

/******************************************************************************/
/*!
 *  \brief
 *      Finds all nodes that are reachable from a starting node.
 *
 *  \param start_node
 * 	    the node id to start from
 *
 *  \param method
 *      The means of traversing the list
 *
 *  \return
 *      A list of all node IDs reachable from the starting node.
 *
 */
/******************************************************************************/
std::vector<unsigned> ALGraph::SearchFrom(unsigned, TRAVERSAL_METHOD) const
{
  return std::vector<unsigned>();
}

/******************************************************************************/
/*!
 *  \brief
 *      Calculates the cost to reach each other node from the start node.
 *
 *  \param start_node
 * 	    The node to start searching from.
 *
 *  \return
 *       A vector containing the cost to reach each node from the start, as
 *       well as the path taken for each node.
 *
 */
/******************************************************************************/
std::vector<DijkstraInfo> ALGraph::Dijkstra(unsigned start_node) const
{
  --start_node;
  std::priority_queue<AdjInfo> check;
  std::vector<bool> evaluated(mNodes.size(), false);
  std::vector<DijkstraInfo> info(mNodes.size());

  for (std::vector<DijkstraInfo>::iterator iter = info.begin(); iter != info.end(); ++iter)
  {
    (*iter).cost = UINT_MAX;
  }

  info[start_node].cost = 0u;
  info[start_node].path.push_back(start_node + 1); // this is for the actual node number
  check.push(MakeAdjInfo(start_node, 0u));

  while (!check.empty())
  {
    AdjInfo curr = check.top();
    check.pop();

    if (!evaluated[curr.mNode])
    {
      evaluated[curr.mNode] = true;

      for (EdgeList::const_iterator iter = mNodes[curr.mNode].GetEList().begin(); iter != mNodes[curr.mNode].GetEList().end(); ++iter)
      {
        if (curr.mCost + (*iter).GetWT() < info[(*iter).GetDest()].cost)
        {
          // If the new cost to reach neighbor is lower...
          info[(*iter).GetDest()].cost = curr.mCost + (*iter).GetWT();
          // Update the path list.
          info[(*iter).GetDest()].path = info[curr.mNode].path;
          info[(*iter).GetDest()].path.push_back((*iter).GetDest() + 1);
        }
        check.push(MakeAdjInfo((*iter).GetDest(), info[(*iter).GetDest()].cost));
      }
    }
  }
  return info;
}

/******************************************************************************/
/*!
 *  \brief
 *      Gets a copy of the adjacency list.
 *
 *  \return
 *       A copy of the adjacency list.
 *
 */
/******************************************************************************/
ALIST ALGraph::GetAList() const
{
  ALIST returner(mNodes.size());

  NodeList::const_iterator currNode;
  EdgeList::const_iterator currEdge;
  ALIST::iterator alistNode = returner.begin();

  for (currNode = mNodes.begin(); currNode != mNodes.end(); ++currNode)
  {
    for (currEdge = (*currNode).GetEList().begin();
      currEdge != (*currNode).GetEList().end();
      ++currEdge)
    {
      (*alistNode).push_back(createInfo((*currEdge).GetDest() + 1, (*currEdge).GetWT()));
    }

    std::sort((*alistNode).begin(), (*alistNode).end(), AListComp);

    ++alistNode;
  }

  return(returner);
}

/******************************************************************************/
/*!
 *  \brief
 *      Flagged when extra-credit searches are implemented.
 *
 *  \return
 *      true if extra credit searches are implemented, false otherwise.
 *
 */
/******************************************************************************/
bool ALGraph::ImplementedSearches()
{
  return false;
}

/******************************************************************************/
/*!
 *  \brief
 *      Constructor
 *
 */
/******************************************************************************/
ALGraph::GEdge::GEdge() :mDest(0u), mWT(0u)
{
}

/******************************************************************************/
/*!
 *  \brief
 *      constructor
 *
 *  \param dest
 *      The destination node for this edge.
 *  
 *  \param wt
 *      The weight of this edge.
 *
 *
 */
/******************************************************************************/
ALGraph::GEdge::GEdge(unsigned dest, unsigned wt) :mDest(dest), mWT(wt)
{
}

/******************************************************************************/
/*!
 *  \brief
 *      Sets the weight of the current edge.
 *
 *  \param new_wt
 *      The new weight of the current edge.
 *
 *  \return
 *      A reference to the current edge.
 *
 */
/******************************************************************************/
ALGraph::GEdge& ALGraph::GEdge::SetWT(unsigned new_wt)
{
  mWT = new_wt;
  return(*this);
}

/******************************************************************************/
/*!
 *  \brief
 *      Sets the destination of the current edge.
 *
 *  \param new_dest
 *      The new destination of the current edge.
 *
 *  \return
 *      A reference to the current edge.
 *
 */
/******************************************************************************/
ALGraph::GEdge & ALGraph::GEdge::SetDest(unsigned new_dest)
{
  mDest = new_dest;
  return(*this);
}

/******************************************************************************/
/*!
 *  \brief
 *      Gets the weight of the current edge.
 *
 *  \return
 *      The weight of the current edge.
 *
 */
/******************************************************************************/
unsigned ALGraph::GEdge::GetWT() const
{
  return mWT;
}

/******************************************************************************/
/*!
 *  \brief
 *      Gets the destination node ID of the current edge.
 *
 *  \return
 *      The destination node ID of the current edge.
 *
 */
/******************************************************************************/
unsigned ALGraph::GEdge::GetDest() const
{
  return mDest;
}

/******************************************************************************/
/*!
 *  \brief
 *      Constructor
 *
 */
/******************************************************************************/
ALGraph::GNode::GNode() :mEdges()
{
}

/******************************************************************************/
/*!
 *  \brief
 *      Adds an edge starting from the current node.
 *
 *  \param dest
 *      The node the edge leads to.
 *
 *  \param wt
 *      The weight of the edge.
 *
 */
/******************************************************************************/
void ALGraph::GNode::addEdge(unsigned dest, unsigned wt)
{
  mEdges.push_back(GEdge(dest, wt));
}

/******************************************************************************/
/*!
 *  \brief
 *      Gets the list of edges coming out from the current node.
 *
 *  \return
 *      A constant reference to the edge list of the current node.
 *
 */
/******************************************************************************/
const ALGraph::EdgeList & ALGraph::GNode::GetEList() const
{
  return mEdges;
}

/******************************************************************************/
/*!
 *  \brief
 *      Constructor
 *
 */
/******************************************************************************/
ALGraph::AdjInfo::AdjInfo() : mNode(0u), mCost(0u)
{
}

/******************************************************************************/
/*!
 *  \brief
 *      Checks if the current AdjInfo should come before another.
 *
 *  \param rhs
 *      the info that is being compared to.
 *
 *  \return
 *      true if the current node comes before the rhs.
 *
 */
/******************************************************************************/
bool ALGraph::AdjInfo::operator<(const AdjInfo & rhs) const
{
  if (mCost > rhs.mCost)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/******************************************************************************/
/*!
 *  \brief
 *      Helper function to create an internal AdjInfo for use with the
 *      priority queue.
 *
 *  \param source
 *      The GNode to create the AdjInfo from.
 *
 *  \param curr_cost
 *      the currently running cost of the path
 *
 *  \return
 *      A new AdjInfo with the appropriate values set.
 *
 */
/******************************************************************************/
ALGraph::AdjInfo ALGraph::MakeAdjInfo(unsigned source, unsigned curr_cost) const
{
  AdjInfo returner;

  returner.mNode = source;
  returner.mCost = curr_cost;

  return(returner);
}