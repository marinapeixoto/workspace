using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace DecisionTree {
    // 这里很简单，属性是二值的，但不妨碍我们对决策树的学习
    /// <summary>
    /// 决策树数据类，保存单个数据，可以修改数据使用与多个属性情况
    /// </summary>
    public class DTData {
        private bool[] data;
        private string label;
        public DTData() {
            data = new bool[2]; // 数据具有两个属性
            label = ""; // 标签
        }
        public DTData(bool[] d, string l) {
            data = d;
            label = l;
        }
        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="d0">第一个属性数据</param>
        /// <param name="d1">第二个属性数据</param>
        /// <param name="l">标签</param>
        public DTData(bool d0, bool d1, string l) {
            data = new bool[2];
            data[0] = d0;
            data[1] = d1;
            label = l;

        }
        public bool[] Data {
            get { return data; }
            set { data = value; }
        }
        public string Label {
            get { return label; }
            set { label = value; }
        }
        
        /// <summary>
        /// 计算香农熵
        /// </summary>
        /// <param name="dtList">传入的数据列表</param>
        /// <returns>熵</returns>
        public static double CalcEntropy(List<DTData> dtList) {
            List<string> lbs = new List<string>(); // 保存标签
            List<int> lbCount = new List<int>(); // 对应的标签计数
            foreach (DTData lb in dtList) {
                if (!lbs.Contains(lb.Label)) {
                    lbs.Add(lb.Label);
                    lbCount.Add(1);
                } else {
                    lbCount[lbs.IndexOf(lb.Label)]++; // 标签对应的计数列表里的数+1
                }
            }
            int sum = dtList.Count; // 总数据数
            double entr = .0;
            foreach (int item in lbCount) {
                entr -= (item / (double)sum) * Math.Log(item / (double)sum, 2);
            }
            return entr;
        }
        /// <summary>
        /// 计算按某个属性（d1或d2）分类得到的熵
        /// </summary>
        /// <param name="dtList">数据集</param>
        /// <param name="featureIndex">该属性索引</param>
        /// <returns></returns>
        public static double FeatureEntropy(List<DTData> dtList,int featureIndex){
            if (featureIndex<0||featureIndex>2) {
                return -2;
            }
            double entr = .0;
            List<DTData> dtl_true = new List<DTData>();
            List<DTData> dtl_false = new List<DTData>();
            foreach (DTData item in dtList) {
                if (item.Data[featureIndex]==true) {
                    dtl_true.Add(item);
                } else {
                    dtl_false.Add(item);
                }
            }
            entr += (dtl_true.Count/(double)dtList.Count)*CalcEntropy(dtl_true);
            entr += (dtl_false.Count / (double)dtList.Count) * CalcEntropy(dtl_false);
            return entr;
        }
        public static int ChooseBestFeature(List<DTData> dtList) {
            double bestEntr = CalcEntropy(dtList);
            int index = -1;
            for (int i = 0; i < 2; i++) { // 虽然只有2个feature，但是循环下吧
                if (FeatureEntropy(dtList,i)<=bestEntr) { // 计算增益，这里不显式计算增益,当然显示计算再比价也可以
                    bestEntr = FeatureEntropy(dtList, i);
                    index = i;
                }
            }
            return index;
        }
    }
    class Program {

        static void Main(string[] args) {
            List<DTData> dtList = new List<DTData>();
            dtList.Add(new DTData(true,true,"yes"));
            dtList.Add(new DTData(true, true, "yes"));
            dtList.Add(new DTData(true, false, "no"));
            dtList.Add(new DTData(false, true, "no"));
            dtList.Add(new DTData(false, true, "no"));
            
            double entr = DTData.CalcEntropy(dtList);
            Console.WriteLine("原始数据的熵:{0}",entr);
            Console.WriteLine("按第一个属性分类的熵:{0}",DTData.FeatureEntropy(dtList,0));
            Console.WriteLine("按第二个属性分类的熵:{0}",DTData.FeatureEntropy(dtList, 1));
            // 一步成功吧：
            int index = DTData.ChooseBestFeature(dtList);
            Console.WriteLine("最适宜的分类标准是：{0}",index);
        }
    }
}
