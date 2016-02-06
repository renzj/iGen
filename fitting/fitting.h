#ifndef FITTING_H
#define FITTING_H

#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <numeric>
#include <map>
#include <algorithm>
#include <tr1/memory>
#include <inttypes.h>
#include <stdexcept>
#include <assert.h>
#include "helper.h"

namespace porcupine
{

class Polynomial
{
public:
	Polynomial() {}

	Polynomial(const std::vector<double> polynomial) : mPolynomial(polynomial) {}

	Polynomial operator + (const Polynomial& p)
	{
		Polynomial res;
		size_t maxLength = std::max(mPolynomial.size(), p.mPolynomial.size());
		res.mPolynomial = std::vector<double>(maxLength, 0);
		for(size_t i = 0;i < maxLength; ++i)
		{
			if(i < mPolynomial.size())
				res.mPolynomial[i] += mPolynomial[i];
			if(i < p.mPolynomial.size())
				res.mPolynomial[i] += p.mPolynomial[i];
		}
		return res;
	}

	Polynomial operator * (const Polynomial& p)
	{
		Polynomial res;
		assert(p.mPolynomial.size() && mPolynomial.size());
		res.mPolynomial = std::vector<double>(mPolynomial.size() + p.mPolynomial.size() - 1, 0);
		for(size_t i = 0;i < mPolynomial.size(); ++i)
		{
			for(size_t j = 0;j < p.mPolynomial.size(); ++j)
			{
				res.mPolynomial[i + j] += mPolynomial[i] * p.mPolynomial[j];
			}
		}
		return res;
	}

	Polynomial operator * (const double factor)
	{
		assert(mPolynomial.size());
		Polynomial res;
		res.mPolynomial = mPolynomial;
		for(size_t i = 0;i < res.mPolynomial.size(); ++i)
			res.mPolynomial[i] *= factor;
		return res;
	}

	Polynomial operator / (const double factor)
	{
		assert(mPolynomial.size());
		assert(factor);
		Polynomial res;
		res.mPolynomial = mPolynomial;
		for(size_t i = 0;i < res.mPolynomial.size(); ++i)
			res.mPolynomial[i] /= factor;
		return res;
	}

	double Get(double x) const
	{
		double res = 0;
		double j = 1;
		for(size_t i = 0;i < mPolynomial.size(); ++i, j *= x)
			res += mPolynomial[i] * j;
		return res;
	}

private:
	std::vector<double>	mPolynomial;
};

typedef std::vector<std::vector<double> > MatrixData;

class Matrix
{
public:

	Matrix() 
	{

		mData = MatrixData(1, std::vector<double>(1, 0));
	}

	Matrix(int row, int col)
	{
		mData = MatrixData(row, std::vector<double>(col, 0));
	}

	Matrix(MatrixData data) : mData(data) {}

	Matrix operator *(const Matrix& m)
	{
		assert(m.mData.size() && mData.size());
		assert(mData[0].size() == m.mData.size());
		MatrixData res(mData.size(), std::vector<double>(m.mData[0].size(), 0)); 
		for(size_t i = 0;i < res.size(); ++i)
			for(size_t j = 0;j < res[0].size(); ++j)
				for(size_t x = 0;x < mData[0].size(); ++x)
						res[i][j] += mData[i][x] * m.mData[x][j];
		return Matrix(res);
	}

	Matrix Inverse() const
	{
		assert(mData.size() == mData[0].size());
		MatrixData res(mData.size(), std::vector<double>(mData[0].size() << 1, 0));
		for(size_t i = 0;i < mData.size(); ++i)
		{
			res[i][i + mData.size()] = 1;
			for(size_t j = 0;j < mData[0].size(); ++j)
			{
				res[i][j] = mData[i][j];
			}
		}
		// transofrom
		bool seq[res.size()];
		memset(seq, 0, sizeof(seq));
		for(size_t t = 0;t < res.size(); ++t)
		{
			int obj = -1;
			for(size_t x = 0;x < res.size(); ++x)
				if(!seq[x] && res[x][x])
				{
					obj = x;
					seq[x] = 1;
					break;
				}
			if(obj == -1)
			{
				std::string error("This matrix is uninvertible!");
				LOG(FATAL) << error << std::endl;
				throw std::runtime_error(error);
			}
			double tmp = res[obj][obj];
			for(size_t j = 0;j < res[0].size(); ++j)
				res[obj][j] /= tmp;
			for(size_t i = 0;i < res.size(); ++i)
			{
				if(i == obj)
					continue;
				tmp = res[i][obj];
				for(size_t j = 0;j < res[0].size(); ++j)
					res[i][j] -= tmp * res[obj][j];
			}
		}
		MatrixData r(mData);
		for(size_t i = 0;i < r.size(); ++i)
		{
			size_t k = 0;
			for(; k < r.size(); ++k)
				if(res[k][i])
					break;
			for(size_t j = r.size(); j < res[0].size(); ++j)
				r[i][j - r.size()] = res[k][j];
		}
		return Matrix(r);
	}

	Matrix Transpose() const
	{
		MatrixData res(mData[0].size(), std::vector<double>(mData.size(), 0));
		for(size_t i = 0;i < mData.size(); ++i)
			for(size_t j = 0;j < mData[i].size(); ++j)
				res[j][i] = mData[i][j];
		return Matrix(res);
	}

	MatrixData GetData()
	{
		return  mData;
	}

	

	~Matrix() {}
private:
	bool cmp(const std::pair<size_t, int> a, const std::pair<size_t, int> b)
	{
		return a.second > b.second;
	}

private:
	std::vector<std::vector<double> > mData;


};

class Fitting
{
public:

	Fitting() {}

	virtual ~Fitting() {}

	/**
	* @brief load data in file to vectors
	*/
	void Load(const std::string& file, std::vector<double>& x_array, std::vector<double>& y_array)
	{
		double x,y;
		std::ifstream in(file.c_str());
		if(!in.is_open())
		{
			std::string error = std::string("Fail to open fitting file ") + file;
			LOG(FATAL) << error << std::endl;
			throw std::logic_error(error);
		}
		while(in >> x >> y)
		{
			x_array.push_back(x);
			y_array.push_back(y);
		}
	}

	/**
	* @brief load data from file
	*/
 	virtual void Init(const std::string& file) = 0;

	/**
	* @brief get the fitting value
	* @param x
	*/
	virtual double Get(double x) const = 0;
};

typedef std::tr1::shared_ptr<Fitting> FittingPtr;

/***************************************************************
  Functions and data structures for vector
***************************************************************
***************************************************************
**************************************************************/

/**
* @brief calculate sum of square for {accumulate}
*/
template<typename T>
struct Square
{
	T operator () (const T& left, const T& right) const
	{
		return left + right * right;
	}
};

}

#endif
