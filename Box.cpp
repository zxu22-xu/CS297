#include "Box.h"
#include <cmath>
#include <cassert>
#include <cstdio>
#include <iostream>


Box::Box() {}

Box::Box(const int a_lowcorner[DIM], const int a_highcorner[DIM]) {
	for (int i = 0; i < DIM; i++) {
		m_lowcorner[i] = a_lowcorner[i];
		m_highcorner[i] = a_highcorner[i];
	}
}

Box::Box(const Box& a_Box) {
	for (int i = 0; i < DIM; i++) {
		m_lowcorner[i] = a_Box.m_lowcorner[i];
		m_highcorner[i] = a_Box.m_highcorner[i];
	}
}

Box Box::operator*(const Box& a_rightBox) const {
	int rhs_lowcorner[DIM];
	int rhs_highcorner[DIM];
	int intersect_lowcorner[DIM];
	int intersect_highcorner[DIM];

	a_rightBox.getHighCorner(rhs_highcorner);
	a_rightBox.getLowCorner(rhs_lowcorner);

	for (int i = 0; i < DIM; i++) {
		if (rhs_highcorner[i] > m_lowcorner[i] && rhs_lowcorner[i] < m_highcorner[i]) {
			intersect_lowcorner[i] = m_lowcorner[i];
		}

		else if (m_lowcorner[i] > rhs_highcorner[i] && m_lowcorner[i] < rhs_highcorner[i]) {
			intersect_lowcorner[i] = m_lowcorner[i];
		}

		else {
			assert(0);
			intersect_lowcorner[i] = 0;
		}

		if (rhs_highcorner[i] > m_lowcorner[i] && rhs_highcorner[i] < m_highcorner[i])
		{
			intersect_highcorner[i] = rhs_highcorner[i];
		}
		/// comparison of the high corner of box A with the corners to see if it is in between
		else if (m_highcorner[i] > rhs_lowcorner[i] && m_highcorner[i] < rhs_highcorner[i])
		{
			intersect_highcorner[i] = m_highcorner[i];
		}
		else
		{
			/// stop the program if no intersection
			assert(0);
			intersect_highcorner[i] = 0;
		}

	}

	return Box(intersect_lowcorner, intersect_highcorner);
}

Box Box::shift(int a_direction, int a_offset) {
	assert(a_direction > 0 && a_direction < DIM);
	int newlowcorner[DIM];
	int newhighcorner[DIM];

	for (int i = 0; i < DIM; i++) {
		if (i == a_direction) {
			newlowcorner[i] = m_lowcorner[i] + a_offset;
			newhighcorner[i] = m_highcorner[i] + a_offset; 
		}

		else {
			newlowcorner[i] = m_lowcorner[i];
			newhighcorner[i] = m_highcorner[i];
		}
	}

	return Box(newlowcorner, newhighcorner);
}

Box Box::grow(int numpoints) {
	int newlowcorner[DIM];
	int newhighcorner[DIM];

	for (int i = 0; i < DIM; i++) {
		newlowcorner[i] = m_lowcorner[i] - numpoints;
		newhighcorner[i] = m_highcorner[i] + numpoints;
	}

	return Box(newlowcorner, newhighcorner);
}

void Box::getLowCorner(int a_lowcorner[DIM]) const {
	for (int i = 0; i < DIM; i++) {
		a_lowcorner[i] = m_lowcorner[i];
	}
}

void Box::getHighCorner(int a_highcorner[DIM]) const {
	for (int i = 0; i < DIM; i++) {
		a_highcorner[i] = m_highcorner[i];
	}
}

int Box::linearIndex(const int a_tupleIndex[DIM]) const {
	return a_tupleIndex[1] - m_lowcorner[0] + (a_tupleIndex[0] - m_lowcorner[1]) * (m_highcorner[0] - m_lowcorner[0] + 1);
}

void Box::tupleIndex(int a_linearIndex, int a_tupleIndex[DIM]) const {
	a_tupleIndex[1] = a_linearIndex % (m_highcorner[0] - m_lowcorner[0] + 1) + m_lowcorner[0];
	a_tupleIndex[0] = a_linearIndex % (m_highcorner[0] - m_lowcorner[0] + 1) + m_lowcorner[1];
}

int Box::sizeOf() const {
	int dimensions[DIM];
	int size = 1;

	for (int i = 0; i < DIM; i++) {
		dimensions[i] = m_highcorner[i] - m_lowcorner[i] + 1;
		size *= dimensions[i];
	}
	return size;
}

bool Box::operator== (const Box& a_rhsBox) const {
	bool equals = 1;
	int rhs_lowcorner[DIM];
	int rhs_highcorner[DIM];

	a_rhsBox.getLowCorner(rhs_lowcorner);
	a_rhsBox.getHighCorner(rhs_highcorner);

	for (int i = 0; i < DIM; i++) {
		while (equals) {
			if (m_lowcorner[i] == rhs_lowcorner[i] && m_highcorner[i] == rhs_highcorner[i]) {}
			else {
				equals = 0;
			}
		}
	}

	return equals;
}

//the method to print the whole Box
void Box::print() const {
	std::cout << "The Lower Corner: " << std::endl;
	for (int i = 0; i < DIM; i++) {
		std::cout << m_lowcorner << std::endl;
	}
	std::cout << "The Higher Corner: " << std::endl;
	for (int i = 0; i < DIM; i++) {
		std::cout << m_highcorner << std::endl;
	}

}
