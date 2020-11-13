#pragma once
#include "MeshDataTypes.h"
#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <iostream>
#include <unordered_map>

//������������� std::hash, ���������� � ������������ ��� std ��� unordered_multimap<std::pair<unsigned int, unsigned int>, FiniteElement>		�	 Edge
namespace std
{
	template<> struct hash<std::pair<unsigned int, unsigned int>>
	{
		std::size_t operator()(std::pair<unsigned int, unsigned int> const& p) const noexcept
		{
			std::size_t seed = p.first;
			seed ^= stdext::hash_value(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}
namespace std
{
	template<> struct hash<Edge>
	{
		std::size_t operator()(const Edge& edge) const noexcept
		{
			std::size_t seed = edge.first_node_id;
			seed ^= stdext::hash_value(edge.second_node_id) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}


class MeshLoader
{
protected:

	std::vector<Node> Nodes_Container;
	std::vector<FiniteElement>  FE_Container;
	std::vector<BoundaryFiniteElement> BFE_Container;
	//��������������� ����������
	std::unordered_multimap<std::pair<unsigned int, unsigned int>, FiniteElement> FE_based_on_Edge;	//����������� ����� �� �� ��
	std::vector<std::vector<Node>> AdjacentNodesContainer;
	void make_FE_Edge_map();

public:
	virtual void LoadMesh(const std::string&) = 0;
	virtual ~MeshLoader() = default;

	//������, ����������� �������� STL - ���������� �����, �������� ��������� � ��������� ���������
	std::vector<Node>& Get_Nodes_Container();
	std::vector<FiniteElement>& Get_FE_Container();
	std::vector <BoundaryFiniteElement>& Get_BFE_Container();

	//�����, ����������� ����� �� �� ID ���� ��� ��������� �����
	std::vector<FiniteElement> Get_FE_by_3ID(unsigned int, unsigned int, unsigned int) const;

	//�����, ����������� ����� �� �� �����, ��������� � ������� ID ���� �����
	std::vector<FiniteElement> Get_FE_by_Edge_Fast(unsigned int, unsigned int);
	std::vector<FiniteElement> Get_FE_by_Edge_Slow(unsigned int, unsigned int) const;

	//�����, ������������ ��������� ��������� ����� �� ID �������;
	std::vector<Node> Get_BoundaryNodes_by_borderId(unsigned int) const;

	//�����, ������������ ��������� �� � �������� ID ���������;
	std::vector<FiniteElement> Get_FE_by_matId(unsigned int) const;

	//�����, ������������ ��������� ��������� �� � �������� ID �������;
	std::vector<BoundaryFiniteElement> Get_BFE_by_borderId(unsigned int) const;

	//�����, ������������ ������� ����� ����� � �������� ����� ������������ �� (������� ��������� ��).
	void Add_NewNodes();

	//�����, ��������� ���������, n - �� ������� �������� ������ ��������� ����	�������� �� ������ ��� ���� n;
	std::vector<std::vector<Node>> Get_AdjacentNodesContainer();

	//������, ����������� ����������������� ����� Node, Element � BoundaryFiniteElement �� �������;
	void Print_Nodes_Container() const;
	void Print_FE_Container() const;
	void Print_BFE_Container() const;
	void Print_MeshData() const;

	void Print_Node(const Node&) const;
	void Print_FiniteElement(const FiniteElement&) const;
	void Print_BoundaryFiniteElement(const BoundaryFiniteElement&) const;

	//����� �������� ���������� ����
	bool isVertex(unsigned int id, ...) const;
	bool isVertex(const FiniteElement& fe) const;
};