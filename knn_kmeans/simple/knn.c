#include <stdio.h>
#include <math.h>
//#define NDEBUG
#include <assert.h>




int main(){
	int i, j; // 循环变量，统计
	double x_sets[] = { 1., 1., .0, .0 }; // 横坐标
	double y_sets[] = { 1.1, 1., .0, .1 }; // 纵坐标
	char labels[] = { 'A', 'A', 'B', 'B' }; // 所给点标签
	double distance[] = { .0, .0, .0, .0 }; // 所给的点与待分类点(x,y)的距离
	double x = 0.2, y = 0.2; // 待分类点 

	double tmp; // 距离排序临时变量
	char tmpchar; // 标签排序临时变量
	int size = 4; // 所给点个数
	int k = 3; // k近邻算法的k值
	for (i = 0; i < size; i++){
		/* 计算距离 */
		distance[i] = sqrt(pow(x_sets[i] - x, 2) + pow(y_sets[i] - y, 2));
	}
	/* sort with labels.排序 */
	for (i = 1; i < size; i++){
		tmp = distance[i];
		tmpchar = labels[i];
		j = i - 1;
		while (j >= 0 && tmp<distance[j]){
			distance[j + 1] = distance[j];
			labels[j + 1] = labels[j];
			j--;
		}
		distance[j + 1] = tmp;
		labels[j + 1] = tmpchar;
	}

	/* 选择前k最近点,并计算频率,此处只有两类，利用i和j统计了。 */
	i = 0; j = 0;
	while (k>0){
		if (labels[i] == 'A') i++;
		else j++;
		k--;
	}
	printf("点属于%c类\n", i > j ? 'A' : 'B');
	k = 0;
	assert(k);
	return 0;
}