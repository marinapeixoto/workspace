#ifndef TK_ML_KVPAIR_HPP
#define TK_ML_KVPAIR_HPP

namespace tk{

	template <typename K, typename V> class KVpair{
	public:
		K key;
		V value;
		KVpair(K k, V v){
			key = k;
			value = v;
		}
		bool operator <(const KVpair& kvp) const{
			return value < kvp.value;
		}
		bool operator >(const KVpair& kvp) const{
			return value > kvp.value;
		}
		bool operator ==(const KVpair& kvp) const{
			return value == kvp.value;
		}
	};
}
#endif // TK_ML_KVPAIR_HPP