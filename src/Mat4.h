#ifndef _jmat4
#define _jmat4
#include "j_framework.h"

#define MATRIX_DIM 4
#define MATRIX_LEN (MATRIX_DIM*MATRIX_DIM)

class Mat4
{
	public:
		Mat4() { clear(); }
		Mat4(std::vector<double> &b) { clear(); if(size() == b.size()) _m = b; }
		Mat4(const Mat4 &b) { clear(); if(size() == b.size()) _m = b._m; }
		~Mat4() {}

		void Mat4::clear()
		{
			_m.assign(size(), 0.0f);
			for(auto i = 0; i<dim(); i++) { at(i, i) = 1.0f; }
		}
		int			dim() const							{ return MATRIX_DIM; }
		int			size() const							{ return MATRIX_LEN; }
		bool		idOk(int id) const					{ if(id >= 0 && id < size()) return true; else return false; }
		int			rowcol2id(int row, int col) const	{ return (row * dim()) + col; }

		double&		at(int d)							{ return _m.at(d); }
		double&		at(int row, int col)					{ return _m.at(rowcol2id(row, col)); }
		void		set(int d, double v)					{ _m[d] = v; }
		void		set(int row, int col, double v)		{ _m[rowcol2id(row, col)] = v; }
		double		get(int d) const						{ return(_m[d]); }
		double		get(int row, int col) const			{ return _m[rowcol2id(row, col)]; }
		Mat4&		operator=(const Mat4 &b)			{ if(this == &b) return(*this); _m = b._m; return(*this); }
		bool		operator==(const Mat4 &b) const		{ jint sz = size(); if(sz != b.size()) return false; for(auto i = 0; i<sz; i++) if(get(i) != b.get(i)) return false; return true; }
		Mat4&		operator*= (const Mat4& b)
		{
			// rows
			for(auto i = 0; i<dim(); i++)
			{
				// cols
				for(auto j = 0; j<b.dim(); j++) { set(i, j, dot(i, j, b)); }
			}
			return (*this);
		}
		Mat4 operator*(const Mat4 &b) const { Mat4 r(*this); r *= b; return r; }

		// square matrix (row * column) dot product
		double dot(int row, int col, const Mat4 &b) const
		{
			double r = 0.0;
			if(dim() != b.dim()) return r;
			for(auto i = 0; i<dim(); i++)
			{
				r += get(row, i) * b.get(i, col);
			}
			return r;
		}

		friend std::ostream& operator<<(std::ostream& ostr, const Mat4& b)
		{
			OSTR_SET
			// rows
			for(auto i = 0; i<b.dim(); i++)
			{
				ostr << "[";
				// columns
				for(auto j = 0; j<b.dim(); j++) { ostr << b.get(i, j) << ", "; }
				ostr << "]" << endl;
			}
			return ostr;
		}

	private:
		std::vector<double> _m;
};

#endif
