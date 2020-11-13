#include "MeshDataTypes.h"
#include <iomanip>

//NODE
Node::Node(int id, float x1, float x2, float x3, bool flag) :
	id(id), x1(x1), x2(x2), x3(x3), flag(flag) {}
std::ostream& operator<<(std::ostream& out, const Node& node)
{
	out << std::setw(10) << std::right << std::fixed << std::setprecision(6) << node.x1
		<< std::setw(10) << std::right << std::fixed << std::setprecision(6) << node.x2
		<< std::setw(10) << std::right << std::fixed << std::setprecision(6) << node.x3
		<<  "	ID" <<node.id << std::endl;
	return out;
}
bool Node::operator==(const Node& n1) const
{
	return id == n1.id;
}
bool Node::operator<(const Node& right_node) const
{
	return id < right_node.id;
}

//FINITE_ELEMENT
FiniteElement::FiniteElement(int ke_id, int mat_id, std::vector<unsigned int> nodes_id) :
	ke_id(ke_id), mat_id(mat_id), nodes_id(nodes_id) {};
std::ostream& operator<<(std::ostream& out, const FiniteElement& cur)
{
	out << std::setw(4) << std::right << cur.mat_id;
	for (auto iter = cur.nodes_id.begin(); iter != cur.nodes_id.end(); ++iter)
		out << std::setw(11) << std::right << *iter;
	out << "	ID" << cur.ke_id << std::endl;;
	return out;
}

//BOUNDERY_FINITE_ELEMENT
BoundaryFiniteElement::BoundaryFiniteElement(int ke_id, int border_id, std::vector<unsigned int> nodes_id) :
	ke_id(ke_id), border_id(border_id), nodes_id(nodes_id) {};
std::ostream& operator<<(std::ostream& out, const BoundaryFiniteElement& cur)
{
	out << std::setw(4) << std::right << cur.border_id;
	for (auto iter = cur.nodes_id.begin(); iter != cur.nodes_id.end(); ++iter)
		out << std::setw(13) << std::right << *iter;
	out << "	ID" << cur.ke_id << std::endl;;
	return out;
}

//EDGE
bool Edge::operator==(const Edge& second_edge) const
{
	if (second_edge.first_node_id == first_node_id && second_edge.second_node_id == second_node_id)
		return true;
	else return false;
}
Edge::Edge(unsigned int id1,unsigned int id2,unsigned int id3) :
center_node_id(id3)
{
	if (id1 < id2)
	{
		first_node_id = id1;
		second_node_id = id2;
	}
	else
	{
		first_node_id = id2;
		second_node_id = id1;
	}
}
void Edge::SetCenter(unsigned int id)
{
	center_node_id = id;
}
bool Edge::operator<(const Edge& second_edge) const
{
	return center_node_id < second_edge.center_node_id;
}
