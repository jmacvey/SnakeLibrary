//--------------------------------------------------------------------
// C++ Header File - BodyPart.h
// Author - J. MacVey, J. Johnson
//--------------------------------------------------------------------

#ifndef BODYPART_H
#define BODYPART_H

class BodyPart {
public:
	BodyPart(int row, int col, BodyPart* next) : row(row), col(col), next(next) {}
	int row;
	int col;
	BodyPart* next;
	bool operator==(BodyPart rhs)
	{
		return rhs.col == this->col && rhs.row == this->row;
	}
	bool operator!=(BodyPart rhs)
	{
		return rhs.col != this->col && rhs.row != this->row;
	}
};

#endif
