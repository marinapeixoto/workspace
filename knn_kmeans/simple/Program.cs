using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;


namespace KNN
{
    public class KnnPoint : IComparable {
        private double x;
        private double y;
        private double dist;
        private string label;
        public KnnPoint(double x_, double y_, double dist_, string label_) {
            this.x = x_;
            this.y = y_;
            this.dist = dist_;
            this.label = label_;
        }
        public double X {
            get { return x; }
            set { x = value; }
        }
        public double Y {
            get { return y; }
            set { y = value; }
        }
        public double Dist {
            get { return dist; }
            set { dist = value; }
        }
        public string Label {
            get { return label; }
            set { label = value; }
        }
        public int CompareTo(object a) {
            KnnPoint p1 = a as KnnPoint;
            if (this.Dist > p1.Dist) {
                return 1;
            } else if (this.Dist == p1.Dist) {
                return 0;
            } else {
                return -1;
            }
        }

    }
    public class LabelCount : IComparable {
        private string label;
        private int count;
        public LabelCount(string label,int count) {
            this.label = label;
            this.count = count;
        }
        public string Label {
            get { return label; }
            set { label = value; }
        }
        public int Count {
            get { return count; }
            set { count = value; }
        }
        public int CompareTo(object obj) {
            LabelCount l = obj as LabelCount;
            if (this.count>l.count) {
                return 1;
            } else if (this.count == l.count) {
                return 0;
            } else {
                return -1;
            }
        }
    }
    class Program
    { 

        static void Main(string[] args){
            StreamReader sr = File.OpenText("./data.txt"); // 数据文件
            List<KnnPoint> pointList = new List<KnnPoint>(); // 训练数据点
            KnnPoint pt = new KnnPoint(0.9, 0.8, .0, ""); // 待判断点
            int k = 4; // k值
            double distance = .0;
            
            List<string> labels = new List<string>(); // 用于保存所有标签
            List<LabelCount> lc = new List<LabelCount>(); // 标签、计数,方便排序

            // 读取文件，获取标签和点
            string line; // 保存一行
            string[] d; // 一行中分割后的数据
            while (!sr.EndOfStream){
                line = sr.ReadLine();
                d = line.Split(' ');
                distance = Math.Sqrt(Math.Pow(double.Parse(d[0]) - pt.X, 2) + Math.Pow(double.Parse(d[1]) - pt.Y, 2)); // 计算距离
                pointList.Add(new KnnPoint(double.Parse(d[0]),double.Parse(d[1]),distance,d[2]));
                // 发现有哪些标签
                if (!labels.Contains(d[2])) {
                    labels.Add(d[2]);
                    lc.Add(new LabelCount(d[2], 0));
                }
            }
            pointList.Sort(); // 根据距离，对所有点排序
  
            // 选取前k个点
            for (int i = 0; i < k; i++) {
                lc[labels.IndexOf(pointList[i].Label)].Count++;
                // 与下面代码等价
                //index = labels.IndexOf(pointList[i].Label);
                //lc[index].Count++;
            }
            lc.Sort();

            // 决定属于哪一类
            Console.WriteLine("未知点属于" + lc[labels.Count-1].Label + "类");
            
            
        }
    }
}
