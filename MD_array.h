#ifndef MD_array
#define MD_array

#include "Box.h"

template <class T> class MDarray {
private:
	T* m_data;
	Box m_box;
	bool m_isdefined;
	//MDarray& operator = (const MDarray& a_MDarray);
	//void fillGhosts();

public:
	~MDarray();
	MDarray();
	MDarray(Box a_box);
	
	void define(Box a_box);
	void setVal(T a_val);
	void copyTo(MDarray<T>& a_destArray) const;

	void operator += (const MDarray& a_MDarray);
	void operator -= (const MDarray& a_MDarray);
	void operator *= (const MDarray& a_MDarray);
	void operator /= (const MDarray& a_MDarray);
	T& operator[] (int a_index[DIM]) const; 
	T& indexShift(const int a_tuple[DIM], const int& a_dir, const int& a_shift) const;
	T& operator[] (int a_linearIndex) const;


	void shift(int a_tuple[DIM]);
	void shift(int a_dir, int a_len);

	const Box& getBox() const;
	void operator += (const T& a_rhs);
	void operator -= (const T& a_rhs);
	void operator *= (const T& a_rhs);
	void operator /= (const T& a_rhs);



};


#endif
