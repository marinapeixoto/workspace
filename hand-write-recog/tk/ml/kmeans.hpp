#ifndef TK_ML_KMEANS_HPP
#define TK_ML_KMEANS_HPP

#include <vector>
using namespace std;

#include "point2d.hpp"

namespace tk{
	// k均值聚类
	template <typename T> class Kmeans{
	public:
		unsigned int k;
		vector<T> data;
		Vector<vector<T>> result;
		Kmeans(unsigned k = 1);
		~Kmeans();
		bool havaData(){ return data.size() > 0; }
		int dataSize(){ return data.size(); }
		bool loadData(vector<T>&d);
		void addData(T d);
		void clustering();
		int predict(T d); // 返回其在result中的索引
	};

	// ------------------------------------------------
	template <typename T>
	bool Kmeans<T>::loadData(vector<T>& d){
		if (d.size()>0){
			data = d;
			return true;
		}else{
			return false;
		}
	}
	template <typename T>
	void Kmeans<T>::addData(T d){
		data.push_back(d);
	}
	// TODO: 聚类算法还没写
	template <typename T>
	void Kmeans<T>::clustering(){

	}
	template <typename T>
	int Kmeans<T>::predict(T d){
		return 0;
	}
}
#endif // TK_ML_KMEANS_HPP
