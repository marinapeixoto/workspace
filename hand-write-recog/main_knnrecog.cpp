#include "hwrecog.hpp"
#include "tk\tk.hpp"
#include "helper.h"


using namespace tk;

void skipLines(ifstream& ifs, int c) {
	char ch[256];
	for (int i = 0; i < c; i++) {
		ifs.getline(ch, 256);
	}
}

HWImg getImg(ifstream& ifs){
	char ch[1024];
	HWImg img;
	int lbl;
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			ch[i * 32 + j] = ifs.get();
		}
		ifs.get(); // skip \n
	}
	ifs.get(); // skip blank
	lbl = (int)ifs.get() - 48;
	img.set(ch,lbl);
	ifs.get(); // skip \n
	return img;
}



int main(int argc, char** argv){
	// ׼������
	vector<HWImg> data;
	vector<int> lbls;
	ifstream ifs("./optdigits-orig.tra");
	skipLines(ifs, 21);
	for (int i = 0; i < 1600; i++){ // 1600��ԭʼ�������ڱȽ�
		HWImg img = getImg(ifs);
		data.push_back(img);
		lbls.push_back(img.label);
	}
	ifs.close();
	KNN<HWImg,int> knn(20); // knn�����ݵ�Ϊimg���ͣ���ǩΪint�ͣ�k=20
	knn.loadData(data, lbls);

	// �����ȡ��֤�ļ����Ԥ��
	int ccount = 0;
	ifs.open("./optdigits-orig.cv");
	skipLines(ifs,21);
	for (int i = 0; i < 800; i++){ // Ԥ��300��
		HWImg img = getImg(ifs);
		int v = knn.predict(img);
		if (v==img.label){
			ccount++;
		}
		cout << "True Value: " << img.label << "\tvalidate value: " << v << endl;
	}
	ifs.close();
	cout << "----------------------" << endl;
	cout << "׼ȷ��:" << ccount / 800.0 << endl;
	return 0;
}