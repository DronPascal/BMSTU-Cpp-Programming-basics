#pragma once
#include <vector>
#include <iostream>


struct Node	//узел
{
	unsigned int id;	//>=1
	float x1, x2, x3;
	bool flag;	//1 - вершина, 0 - внутренний узел

	Node() = default;
	Node(int, float, float, float, bool);

	friend std::ostream& operator<<(std::ostream&, const Node&);
	bool operator==(const Node&) const;
	bool operator<(const Node&) const;
};

struct FiniteElement	//конечный эл-т
{
	unsigned int ke_id,
		mat_id;
	std::vector<unsigned int> nodes_id;

	FiniteElement() {};
	FiniteElement(int, int, std::vector<unsigned int>);

	friend std::ostream& operator<<(std::ostream&, const FiniteElement&);
};

struct BoundaryFiniteElement	//граничный конечный элемент
{
	unsigned int ke_id,		//ID элемента (>= 1), ID границы с данным типом граничного услови€
		border_id;
	std::vector<unsigned int> nodes_id;	//список ID узлов поверхностно  Ё

	BoundaryFiniteElement() {};
	BoundaryFiniteElement(int, int, std::vector<unsigned int>);

	friend std::ostream& operator<<(std::ostream&, const BoundaryFiniteElement&);
};

struct Edge {
	unsigned int first_node_id;
	unsigned int second_node_id;
	unsigned int center_node_id;

	Edge() = default;
	Edge(unsigned int, unsigned int, unsigned int);

	bool operator==(const Edge&) const;
	bool operator<(const Edge&) const;
	void SetCenter(unsigned int);
};