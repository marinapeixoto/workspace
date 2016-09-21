#include "helper.h"

void getImg(ifstream& ifs, Mat**& d, char*& l, unsigned count) {
	//char ch[32][32];
	char ch[1024];
	d = new Mat*[count];
	l = new char[count];
	for (; count > 0; count--) {
		//ch = new char[1024];

		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 32; j++) {
				//ch[i][j] = ifs.get();
				ch[i * 32 + j] = ifs.get();
			}
			ifs.get(); // 跳过换行
		}
		ifs.get(); // 跳过空格

		//Mat m(32, 32, CV_8UC1, ch);
		//Mat m(1, 1024, CV_8UC1, ch); 
		Mat* m = new Mat(1, 1024, CV_8UC1, ch);
		//d.push_back(m);
		//l.push_back((char)ifs.get());
		d[count] = m;
		l[count] = (char)ifs.get();
		ifs.get(); // 跳过换行
	}

	//delete ch;
}

// vector 转Mat数组
static Mat* formatData(Mat**& d, const unsigned count) {
	// 1024 = d[0].rows*d[0].cols
	//Mat dst(static_cast<int>(d.size()), 1024, CV_8UC1);
	Mat* dst = new Mat(count, 1024, CV_8UC1);
	for (unsigned int i = 0; i < count; i++) {
		Mat img_row = d[i]->reshape(1, 1); // 定义数据格式,1行
		Mat row_i = dst->row(i); // 将数据指向row_i
		img_row.convertTo(row_i, CV_8UC1); // 操作row_i改变数据
	}
	return dst;
}

void skipLines(ifstream& ifs, int c) {
	//char *ch = new char[256];
	char ch[256];
	for (int i = 0; i < c; i++) {
		ifs.getline(ch, 256);
	}
	//delete ch;
}

/*
1. 读取原始图像，做pca降维
2. SVM训练
3. 读取目标图像验证
*/
int main(int argc, const char* argv[]) {
	string trainDataFile = "./optdigits-orig.tra";
	string valiDataFile = "./optdigits-orig.cv";

	const unsigned SampleCount = 1600; 

	ifstream ifs(trainDataFile.c_str());
	skipLines(ifs, 21);
	//char* line = new char[256];
	//vector<Mat> data;
	Mat** data = nullptr; 
	//vector<char> labels;
	char* labels = nullptr; 
	getImg(ifs, data, labels, SampleCount); // 读取1600个数据

	//for (unsigned i = 0; i < 10; i++) {
	//	for (unsigned j = 0; j < 32; j++) {
	//		for (unsigned k = 0; k < 32; k++) {
	//			cout << data[i].data[j * 32 + k] << ' '; 
	//		}
	//		cout << endl; 
	//	}
	//}

	ifs.close();

	Mat* pcaPreData = formatData(data, SampleCount); // 格式数据,为pca准备

	// 接下来做PCA降维,
	PCA pca(*pcaPreData, cv::Mat(), CV_PCA_DATA_AS_ROW, 10);
	Mat pt = pca.project(pcaPreData->row(0));
	Mat reconstr = pca.backProject(pt); // 反映射
	reconstr = reconstr.reshape(data[0]->channels(), data[0]->rows); // 图像的重构

	// TODO: 数据保存到vector里面貌似除了点问题
	for (size_t i = 0; i < 32; i++) {
		for (size_t j = 0; j < 32; j++) {
			cout << (char)data[3]->data[i * 32 + j] << " ";
		}
		cout << endl;
	}
	Mat egiVals = pca.eigenvalues;

	/*for (size_t i = 0; i < egiVals.rows; i++)
	{
	cout << (int)egiVals.data[i]<<" ";
	}*/
	waitKey(0);

	//ifs.close(); 
	return 0;
}
