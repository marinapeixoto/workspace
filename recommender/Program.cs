using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using tk.Data.Helper.Ali;

namespace Ali_User_Opr_Model {
    class Program {
        static void CalcSimilarity(double[,] sim, List<int> bList,List<UserBrandTable> ubt) {
            int size = bList.Count;
            int[] userCount = new int[size]; // 统计相应品牌的人数
            for (int i = 0; i < size; i++) {
                userCount[i] = 0;
            }
            for (int i = 0; i < ubt.Count; i++) {
                for (int j = 0; j < ubt[i].brands.Count-1; j++) {
                    userCount[bList.IndexOf(ubt[i].brands[j].brand_id)]++; // 计算人数
                    for (int k = j; k < ubt[i].brands.Count; k++) {
                        sim[bList.IndexOf(ubt[i].brands[j].brand_id), bList.IndexOf(ubt[i].brands[k].brand_id)] += 1.0;
                        sim[bList.IndexOf(ubt[i].brands[k].brand_id), bList.IndexOf(ubt[i].brands[j].brand_id)] += 1.0;
                    }
                }
                userCount[bList.IndexOf(ubt[i].brands[ubt[i].brands.Count-1].brand_id)] += 1;
            }
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    sim[i, j] = sim[i, j] / Math.Sqrt(userCount[i]*userCount[j]);
                }
            }
        }
        static List<UserRecom> Predict(List<UserBrandTable> ubt,List<int> bList,double[,] sim, int k_ub,int k_b,double h,List<BrandInfo> bi) {
            List<UserRecom> recommend = new List<UserRecom>();

            int bsize = bList.Count;
            int usize = ubt.Count;
            List<BrandAndRating> bSim = new List<BrandAndRating>(); // 将相似度也看成一种给分
            List<BrandAndRating> pbSim = new List<BrandAndRating>(); // 商品预测给分
            List<int> havePrd = new List<int>();
            // 对所有用户
            for (int i = 0; i < usize; i++) {
                
                // 选取i用户的k_ub个给分高的商品
                for (int k = 0; k < (k_ub < ubt[i].brands.Count ? k_ub : ubt[i].brands.Count); k++) {
                    for (int j = 0; j < bsize; j++) {
                        // 用户感兴趣的物品的相似物品
                        bSim.Add(new BrandAndRating(bList[j], sim[bList.IndexOf(ubt[i].brands[k].brand_id), j]));
                    }
                    bSim.Sort(); // 对相似度排序
                    int idx = bi.FindIndex(delegate(BrandInfo bi_) { return bi_.brand_id == ubt[i].brands[k].brand_id; });
                    pbSim.Add(new BrandAndRating(ubt[i].brands[k].brand_id, ubt[i].brands[k].rating*(bi[idx].brand_click/(double)bi[idx].brand_count))); // 本商品
                    havePrd.Add(ubt[i].brands[k].brand_id);
                    // 选取k_b个相似商品
                    for (int l = 0; l < k_b; l++) {
                        idx = bi.FindIndex(delegate(BrandInfo bi_) { return bi_.brand_id == bSim[l].brand_id; });
                        // 本商品的不考虑，已算
                        if (!havePrd.Contains(bSim[l].brand_id)) {
                            pbSim.Add(new BrandAndRating(bSim[l].brand_id, ubt[i].brands[k].rating * bSim[l].rating * (bi[idx].brand_click / (double)bi[idx].brand_count)));// *2修正
                            havePrd.Add(bSim[l].brand_id);
                        }
                    }
                    bSim.Clear();
                }
                pbSim.Sort(); // 对预测给分排序
                recommend.Add(new UserRecom(ubt[i].user_id));
                //Console.WriteLine("用户:"+ubt[i].user_id);
                for (int m = 0; m < pbSim.Count; m++) {
                    recommend[recommend.Count - 1].Add(pbSim[m]);
                    //Console.Write(pbSim[m].brand_id+":"+pbSim[m].rating+",");
                }
                //Console.WriteLine();
                
                havePrd.Clear();
                
                pbSim.Clear();
                
            }
            DataRWHelper dh = new DataRWHelper();
            // save
            StreamWriter sw = new StreamWriter(dh.pathPrefix + "p.txt");
            string line;
            for (int i = 0; i < recommend.Count; i++) {
                line = ""+recommend[i].user_id+"    ";
                //sw.Write(recommend[i].user_id);
                //sw.Write('\t');
                for (int j = 0; j < recommend[i].brands.Count; j++) {
                    if (recommend[i].brands[j].rating>=h) {
                        //sw.Write(recommend[i].brands[j].brand_id);
                        line += recommend[i].brands[j].brand_id;
                        line += ":";
                        line += recommend[i].brands[j].rating;
                        line += ",";
                    }
                }
                // 没有推荐品牌的用户不输出
                if (!line.Equals(""+recommend[i].user_id+"    ")) {
                    //line.Remove(line.LastIndexOf(','));
                    sw.WriteLine(line.Substring(0,line.Length-1));
                }
            }

            sw.Close();
            return recommend;
        }
        static void Main(string[] args) {
            DataRWHelper dh = new DataRWHelper();
            List<Dt> data = dh.ReadData(dh.TRAIN_DATA); // 加载测试数据
            List<int> bList = new List<int>(); // 保存品牌表
            List<UserBrandTable> ubt = new List<UserBrandTable>();
            List<Dt> oneUserData = new List<Dt>(); // 保存一个用户的记录
            List<BrandInfo> bi = new List<BrandInfo>();
            for (int i = 0; i <= data.Count; i++) {
                if (i==data.Count) { // 处理最后一个数据
                    ubt.Add(new UserBrandTable(oneUserData)); // 添加到用户商品表
                    oneUserData.Clear();
                    break;
                }
                int idx = bi.FindIndex(delegate(BrandInfo bi_) { return bi_.brand_id == data[i].brand_id; });
                if (idx < 0) {
                    bi.Add(new BrandInfo(data[i].brand_id, 0, 0));
                }
                idx = bi.FindIndex(delegate(BrandInfo bi_) { return bi_.brand_id == data[i].brand_id; });
                bi[idx].brand_count++;
                if (data[i].operation == 1) {
                    bi[idx].brand_click++;
                }
                // 如果该表中存在用户而且加入的用户不同，则清理列表，加入新的数据
                if (oneUserData.Count !=0 && oneUserData[0].user_id != data[i].user_id) {
                    // 已经得到一个用户的全部操作信息
                    ubt.Add(new UserBrandTable(oneUserData)); // 添加到用户商品表
                    oneUserData.Clear();
                } 
                oneUserData.Add(data[i]);

                // 保存品牌列表
                if (!bList.Contains(data[i].brand_id)) {
                    bList.Add(data[i].brand_id);
                }
            }
            for (int i = 0; i < ubt.Count; i++) {
                ubt[i].brands.Sort(); // 对品牌评分排序
            }
            Console.WriteLine("用户的品牌评分完成！");

            double[,] sim = new double[bList.Count, bList.Count]; // 物品相似矩阵
            CalcSimilarity(sim,bList,ubt); // 计算相似矩阵
            Console.WriteLine("计算相似度完成！");
            //for (int i = 0; i < 10; i++) {
            //    for (int j = 0; j < 10; j++) {
            //        Console.Write(sim[i,j]+",");
            //    }
            //    Console.WriteLine();
            //}
            Console.WriteLine("开始预测...");
            List<UserRecom> rcm = Predict(ubt, bList, sim, 3, 3,4.0,bi);
            Console.WriteLine("预测完毕");

            // 清楚内存
            sim = null;
            data = null;
            ubt = null;
            GC.Collect();

            // 检验...
            Console.WriteLine("正在检验算法...");
            List<Dt> testData = dh.ReadData(dh.TEST_DATA); // 读取测试数据
            int hb = 0;
            int pb = 0;
            int bb = 0;
            for (int i = 0; i < rcm.Count; i++) {
                for (int j = 0; j < rcm[i].brands.Count; j++) {
                    // 跳过不符合条件的，没有考虑的
                    if (rcm[i].brands[0].rating<4.0) {
                        continue;
                    }
                    pb++; // 
                    if (testData.FindIndex(delegate(Dt d) {
                        // 找到用户购买了该商品
                        return d.user_id == rcm[i].user_id && d.brand_id == rcm[i].brands[j].brand_id && d.operation == 1;
                    })>=0) {
                        hb++;
                    }
                }
            }
            double p = hb / (double)pb;
            Console.WriteLine("准确率:{0}", p);
            hb = 0;
            for (int i = 0; i < testData.Count; i++) {
                if (testData[i].operation == 1) {
                    bb++;
                    
                }
            }
            
            double r=hb/(double)bb;
            // so precision:
            
            Console.WriteLine("召回率：{0}",r);
            Console.WriteLine("F-score:{0}",2*p*r/(p+r));


        }
    }
}
