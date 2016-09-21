#ifndef TK_HWRECOG_HPP
#define TK_HWRECOG_HPP


class HWImg{
public:
	char data[1024];
	int label;
	HWImg(){}
	HWImg(char d[1024], int l){ 
		set(d, l);
	}
	void set(char d[1024], int l){
		for (int i = 0; i < 1024; i++){
			data[i] = d[i];
		}
		label = l;
	}
	~HWImg(){}
	double distance(HWImg img){
		double dst = 0;
		for (int i = 0; i < 1024; i++){
			dst += (data[i] == img.data[i] ? .0 : 1.0);
		}
		return dst;
	}
};

#endif // TK_HWRECOG_HPP
