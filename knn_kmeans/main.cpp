#include <iostream>
#include <cmath>
#include <ctime>
#include "kvpair.h"
#include "point2d.h"
#include "knn.h"
#include "kmeans.h"

using namespace std;
using namespace tk;

extern int knn_test();



int main() {
	//knn_test();
	kmeans<Point2D<double>> km(3);
	vector<Point2D<double>> dt;
	dt.push_back(Point2D<double>(0,0));
	dt.push_back(Point2D<double>(6, 9));
	dt.push_back(Point2D<double>(1, 2));
	dt.push_back(Point2D<double>(12, 15));
	dt.push_back(Point2D<double>(13, 12));
	dt.push_back(Point2D<double>(2, 1));
	dt.push_back(Point2D<double>(7, 8));
	dt.push_back(Point2D<double>(13, 12));
	dt.push_back(Point2D<double>(2, 1));
	dt.push_back(Point2D<double>(15, 14));
	dt.push_back(Point2D<double>(12, 15));
	dt.push_back(Point2D<double>(6, 7));
	dt.push_back(Point2D<double>(2, 1));
	dt.push_back(Point2D<double>(15, 14));
	km.load_data(dt);
	vector<Point2D<double>> center;
	center.push_back(Point2D<double>(3, 4));
	center.push_back(Point2D<double>(5, 7));
	center.push_back(Point2D<double>(10, 12));
	/*srand((unsigned)time(NULL));
	for (size_t i = 0; i < 2; i++) {
		center.push_back(Point2D<double>(15*abs(rand()) / (double)RAND_MAX, 15*abs(rand()) / (double)RAND_MAX));
	}*/
	vector<kvpair<Point2D<double>,vector<Point2D<double>>>> rt = km.clustering(center);
	for (size_t i = 0; i < rt.size(); i++) {
		cout << "中心点:(" << rt[i].key.x << "," << rt[i].key.y << ")，所属点：" << endl;
		for (size_t j = 0; j < rt[i].value.size(); j++) {
			cout << "(" << rt[i].value[j].x << "," << rt[i].value[j].y << ")" << " ";
		}
		cout << endl;
	}
	
	return 0;
}