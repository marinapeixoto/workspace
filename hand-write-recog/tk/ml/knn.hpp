#ifndef TK_ML_KNN_HPP
#define TK_ML_KNN_HPP

#include <vector>
#include <algorithm>
#include <functional>

#include "kvpair.hpp"

namespace tk{
	template <typename T, typename S> class KNN
	{
	public:
		unsigned int k;
		std::vector<T> data;
		std::vector<S> labels;
		KNN(unsigned int k_ = 1);
		~KNN();
		bool haveData(){ return data.size() > 0; }
		int dataSize(){ return data.size(); }
		bool loadData(std::vector<T>& d, std::vector<S>& l);
		void addData(T d, S l);
		S predict(T d);

	private:

	};

	// 下面是实现
	// --------------------------------------------------

	template <typename T, typename S>
	KNN<T, S>::KNN(unsigned int k_ = 1)
	{
		k = k_;
	}

	template <typename T, typename S>
	KNN<T, S>::~KNN()
	{
	}
	template <typename T, typename S>
	bool KNN<T, S>::loadData(std::vector<T>& d, std::vector<S>& l){
		if (d.size() != l.size()){
			return false;
		}
		if (d.size() > 0){
			data = d;
			labels = l;
			return true;
		}
		else{
			return false;
		}

	}
	template <typename T, typename S>
	void KNN<T, S>::addData(T d, S l){
		data.push_back(d);
		labels.push_back(l);
	}
	template <typename T, typename S>
	S KNN<T, S>::predict(T d){
		vector<KVpair<S, double>> kv;
		for (unsigned i = 0; i < data.size(); i++){
			kv.push_back(KVpair<S, double>(labels[i], data[i].distance(d)));
		}
		std::sort(kv.begin(), kv.end(), less<KVpair<S, double>>());
		vector<KVpair<S, int>> kvc; // 计数
		unsigned int index;
		for (unsigned i = 0; i < k; i++){
			for (index = 0; index < kvc.size(); index++){
				if (kvc[index].key == kv[i].key){
					break;
				}
			}
			// kvc里面没有这个标签数据
			if (index == kvc.size()){
				kvc.push_back(KVpair<S, int>(kv[i].key, 1));
			}
			else{
				kvc[index].value += 1;
			}
		}
		std::sort(kvc.begin(), kvc.end(), greater<KVpair<S, int>>());
		return kvc[0].key;
	}

}
#endif // TK_ML_KNN_HPP