#ifndef TK_ML_POINT2D_HPP
#define TK_ML_POINT2D_HPP

#include <math.h>

namespace tk{

	template <typename T> class Point2D
	{
	public:
		T x;
		T y;
		Point2D();
		Point2D(T x_, T y_);
		~Point2D();
		double distance(Point2D<T> p);

	private:

	};


	template <typename T>
	Point2D<T>::Point2D()
	{

	}
	template <typename T>
	Point2D<T>::Point2D(T x_, T y_){
		x = x_;
		y = y_;
	}

	template <typename T>
	Point2D<T>::~Point2D()
	{
	}

	template <typename T>
	double Point2D<T>::distance(Point2D<T> p){
		return sqrt(pow((double)(x - p.x), 2) + pow((double)(y - p.y), 2));
	}
}
#endif // TK_ML_POINT2D_HPP