#ifndef BOX_H
#define BOX_H
#define DIM 2

class Box {
private:
	int m_lowcorner[DIM], m_highcorner[DIM];
public:
	Box(); //empty constructor 

	Box(const int a_lowcorner[DIM], const int a_highcorner[DIM]); //value constructor
	Box(const Box& a_Box); //copy constructor

	Box operator* (const Box& a_rightBox) const; //define multiply
	bool operator == (const Box& a_Box) const; //define equality
	Box shift(int direction, int offest); //shift method
	Box grow(int points); //grow method (in all directions!) 

	void getLowCorner(int a_lowcorner[DIM]) const; //get lowcorner
	void getHighCorner(int a_highcorner[DIM]) const; //get highcorner

	int linearIndex(const int a_tupleIndex[DIM]) const; //compute linear index corresponding to input array
	void tupleIndex(int a_linearIndex, int a_tupleIndex[DIM]) const; //generate tuple indexes

	int sizeOf() const; //get size of the Box
	void print() const; //print the Box

};


#endif
