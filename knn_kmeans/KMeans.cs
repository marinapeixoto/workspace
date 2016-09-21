using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace GasStations {
    class KMeans {
        public List<PointF> data;
        public List<List<PointF>> result;
        public List<PointF> center;
        public int k;
        public KMeans() {

        }
        public KMeans(int k_) {
            data = new List<PointF>();
            result = new List<List<PointF>>(k_);
            // 初始化
            for (int i = 0; i < k_; i++) {
                result.Add(new List<PointF>());
            }

            center = new List<PointF>(k_); 
            k = k_;
        }
        public void SetData(List<PointF> p) {
            data = p;
        }
        public void AddData(PointF pt) {
            data.Add(pt);
        }
        private void RndCenter(PointF start, PointF end) {
            PointF pt = new PointF();
            Random rnd = new Random();
            for (int i = 0; i < k; i++) {
                pt.X = (float)(rnd.NextDouble() * (Math.Abs(end.X - start.X)) + start.X);
                pt.Y = (float)(rnd.NextDouble() * (Math.Abs(end.Y - start.Y)) + start.Y);
                center.Add(pt);
            }
        }
        public void Clustering(PointF start,PointF end) {
            RndCenter(start, end); // 产生中心点

            bool flag = false;
            List<KeyValuePair<int, float>> dst = new List<KeyValuePair<int, float>>();
            if (!flag) {
                for (int i = 0; i < data.Count; i++) {
                    for (int j = 0; j < k; j++) {
                        float d = (float)Math.Sqrt(Math.Pow(data[i].X - center[j].X, 2) + Math.Pow(data[i].Y - center[j].Y, 2));
                        KeyValuePair<int, float> kv = new KeyValuePair<int, float>(j, d); // 到j中心点距离
                        dst.Add(kv);
                    }
                    // 降序排列
                    dst.Sort(delegate(KeyValuePair<int, float> kv1, KeyValuePair<int, float> kv2) {
                        if (kv1.Value > kv2.Value) {
                            return 1;
                        } else {
                            return 1;
                        }
                    });
                    result[dst[0].Key].Add(data[i]); // 将该点添加到与之距离最近的中心点对应的结果中去
                }
                dst.Clear();
                ShowCenter();
                // 重新计算中心点，最后中心点不变,虽然计算存在舍入问题，但到最后会稳定
                flag = CalcCenter();
                if (!flag) {
                    for (int i = 0; i < k; i++) {
                        result[i].Clear();
                    }
                }
            }
        }
        private bool CalcCenter() {
            bool allDone = true; // 假设所有中心点没变化
            for (int i = 0; i < k; i++) {
                PointF ctPt = new PointF(0, 0);
                // 由结果计算中心点，再和center里的点对比
                for (int j = 0; j < result[i].Count; j++) {
                    ctPt.X += result[i][j].X;
                    ctPt.Y += result[i][j].Y;
                }
                ctPt.X /= result[i].Count;
                ctPt.Y /= result[i].Count;
                if (ctPt.X != center[i].X || ctPt.Y!=center[i].Y) {
                    allDone = false;
                }
                center[i] = ctPt;
            }
            return allDone;
        }

        public void ShowCenter() {
            for (int i = 0; i < center.Count; i++) {
                Console.WriteLine("("+center[i].X+","+center[i].Y+")");
            }
        }
        public void ShowResult() {
            Console.WriteLine("聚类结果:");
            for (int i = 0; i < k; i++) {
                Console.WriteLine("以点(" + center[i].X + "," + center[i].Y + ")为中心的点有:");
                for (int j = 0; j < result[i].Count; j++) {
                    Console.WriteLine("(" + result[i][j].X + "," + result[i][j].Y+")");
                }
            }
        }
    }
}
