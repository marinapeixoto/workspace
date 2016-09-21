#ifndef TK_POINT2D_H
#define TK_POINT2D_H

#include <cmath>
using namespace std;

namespace tk {
	template <typename T> class Point2D {
	public:
		T x;
		T y;
		Point2D() { x = 0; y = 0; }
		Point2D(T x_,T y_) {
			x = x_; y = y_;
		}
		void reset(T x_, T y_) {
			x = x_; y = y_;
		}
		bool operator ==(const Point2D& pt) const {
			return (x == pt.x) && (y == pt.y);
		}
		Point2D operator -(const Point2D& pt) const {
			Point2D p(x - pt.x, y - pt.y);
			return p;
		}
		Point2D operator +(const Point2D& pt) const {
			Point2D p(x + pt.x, y + pt.y);
			return p;
		}
		Point2D operator *(int n) const {
			Point2D p(x * n, y * n);
			return p;
		}
		Point2D<double> operator /(int n) const {
			Point2D<double> p(x/(double)n,y/(double)n);
			return p;
		}
		double distance(const Point2D& pt) const {
			return sqrt(pow(x - pt.x, 2.0) + pow(y - pt.y, 2.0));
		}
	};
}


#endif // TK_POINT2D_H
