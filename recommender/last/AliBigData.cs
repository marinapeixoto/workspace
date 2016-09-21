using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace tk.Ali {
    class AliBigData {
        
        public List<Dt> trainData;
        public List<UserBrandTable> ubt;
        public List<Recommend> rcm;
        public List<BrandInfo> brandsInfo;
        public double[,] sim; // 品牌相似矩阵
        public AliBigData() {
            trainData = ReadData(AliDataInfo.OrgDataFile); // 读取训练数据
            ubt = new List<UserBrandTable>();
            rcm = new List<Recommend>();
            brandsInfo = new List<BrandInfo>();
        }
        public List<Dt> ReadData(string dataFile) {
            List<Dt> data = new List<Dt>();
            string path = AliDataInfo.pathPrefix + dataFile;
            StreamReader sr = new StreamReader(path, Encoding.GetEncoding("gb2312"));
            if (dataFile.Equals(AliDataInfo.OrgDataFile)) {
                sr.ReadLine(); // 原始数据第一行不需要
            }
            string line;
            string[] item;
            while (!sr.EndOfStream) {
                line = sr.ReadLine();
                item = line.Split(',');
                if (item[3].Contains("月")) {
                    data.Add(new Dt(int.Parse(item[0]), int.Parse(item[1]), int.Parse(item[2]), DateTime.Parse("2014年" + item[3])));
                } else {
                    data.Add(new Dt(int.Parse(item[0]), int.Parse(item[1]), int.Parse(item[2]), DateTime.Parse(item[3])));
                }
            }
            sr.Close();
            return data;
        }
        /// <summary>
        ///  分析数据，从原始数据分析得到各种处理后的数据
        ///  品牌信息表，用户-品牌表
        /// </summary>
        public void Analysis() {
            List<Dt> oneUserData = new List<Dt>(); // 保存一个用户的数据
            // 一个个分析训练数据
            for (int i = 0; i < trainData.Count; i++) {
                // 得到品牌详细统计
                int index = brandsInfo.FindIndex(delegate(BrandInfo bi) { return bi.brand_id == trainData[i].brand_id; });
                if (index<0) { // 品牌详细表中没有信息
                    brandsInfo.Add(new BrandInfo(trainData[i].brand_id, 0, 0));
                    index = brandsInfo.FindIndex(delegate(BrandInfo bi) { return bi.brand_id == trainData[i].brand_id; });
                }
                brandsInfo[index].brand_count++; // 该品牌总计加1
                if (trainData[i].operation == AliDataInfo.OP_BUY) { brandsInfo[index].brand_buy++; } // 该品牌购买+1

                // 统计用户品牌表
                // 用户表不空，且这个元素不是属于这个表，则先计算oneUserData，然后把元素加入
                if (oneUserData.Count!=0 && trainData[i].user_id!=oneUserData[0].user_id) {
                    ubt.Add(new UserBrandTable(oneUserData));
                    oneUserData.Clear();
                }
                oneUserData.Add(trainData[i]);
                // 处理最后一个元素
                if (i==trainData.Count-1) {
                    ubt.Add(new UserBrandTable(oneUserData));
                    oneUserData.Clear();
                }
            }
            // 对用户的评分表排序
            for (int i = 0; i < ubt.Count; i++) {
                ubt[i].brands.Sort(); // 升序排列
            }

        }
        /// <summary>
        /// 计算物品间的相似度
        /// </summary>
        public void BrandSimilarity() {
            int size = brandsInfo.Count;
            List<int> bList = new List<int>();
            for (int i = 0; i < size; i++) {
                bList.Add(brandsInfo[i].brand_id); // 添加list查找id更快
            }
            sim = new double[size,size]; // 得到矩阵
            int[] userCount = new int[size]; // 品牌对应人数
            int index1 = 0;int index2=0;
            for (int i = 0; i < size; i++) {userCount[i] = 0;} // 初始化
            for (int i = 0; i < ubt.Count; i++) {
                for (int j = 0; j < ubt[i].brands.Count-1; j++) {
                    //index1 = brandsInfo.FindIndex(delegate(BrandInfo bi) { return bi.brand_id == ubt[i].brands[j].brand_id; }); // 得到品牌索引
                    index1 = bList.IndexOf(ubt[i].brands[j].brand_id);
                    userCount[index1]++; // 统计该品牌的人数
                    for (int k = 0; k < ubt[i].brands.Count; k++) {
                        //index2 = brandsInfo.FindIndex(delegate(BrandInfo bi) { return bi.brand_id == ubt[i].brands[k].brand_id; }); // 得到品牌索引
                        index2 = bList.IndexOf(ubt[i].brands[k].brand_id);
                        sim[index1, index2] += 1.0;
                        sim[index2, index1] += 1.0;
                    }
                }
                // 用户i品牌表中最后一个元素
                //index1 = brandsInfo.FindIndex(delegate(BrandInfo bi){return bi.brand_id == ubt[i].brands[ubt[i].brands.Count-1].brand_id;});
                index1 = bList.IndexOf(ubt[i].brands[ubt[i].brands.Count - 1].brand_id);
                userCount[index1]++;
                
            }
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    sim[i, j] = sim[i, j] / Math.Sqrt((double)userCount[i]*userCount[j]);
                }
            }
        }
        /// <summary>
        /// 预测用户
        /// </summary>
        /// <param name="kSize">kSize用户给分高的物品</param>
        /// <param name="sSize">sSize个相似物品</param>
        /// <param name="h">最终给分阈值</param>
        public void Predict(int kSize,int sSize,double h) {
            int bSize = brandsInfo.Count;
            int uSize = ubt.Count;
            List<int> bList = new List<int>();
            for (int i = 0; i < bSize; i++) {
                bList.Add(brandsInfo[i].brand_id); // 添加list查找id更快
            }
            List<BrandAndRating> bSim = new List<BrandAndRating>();
            List<BrandAndRating> bRating = new List<BrandAndRating>(); // 预测给分
            List<int> hvPrd = new List<int>();
            // 对所有用户预测
            for (int i = 0; i < uSize; i++) {
                // 选取用户i的kSize个给分高的,kSize不能多余用户关注商品表中的物品个数
                for (int j = 0; j < (kSize<ubt[i].brands.Count?kSize:ubt[i].brands.Count); j++) {
                    for (int k = 0; k < bSize; k++) {
                        bSim.Add(new BrandAndRating(bList[k], sim[bList.IndexOf(ubt[i].brands[j].brand_id), k])); // 添加该商品j的相似物品
                    }
                    bSim.Sort(); // 对相似度排序，注意自己和自己肯定是最大的

                    int index = bList.IndexOf(ubt[i].brands[j].brand_id);
                    bRating.Add(new BrandAndRating(ubt[i].brands[j].brand_id,ubt[i].brands[j].rating*AliDataInfo.PARAM_MOD*brandsInfo[index].BuyRatio)); // 本品牌
                    hvPrd.Add(ubt[i].brands[j].brand_id);
                    // sSize个相似品牌
                    for (int l = 0; l < sSize; l++) {
                        index = bList.IndexOf(bSim[l].brand_id);
                        // 添加设置相似阈值而非几个相似
                        if (!hvPrd.Contains(bSim[l].brand_id) && bSim[l].rating>0.2) {
                            bRating.Add(new BrandAndRating(bSim[l].brand_id, ubt[i].brands[j].rating * bSim[l].rating * brandsInfo[index].BuyRatio));
                            hvPrd.Add(bSim[l].brand_id);
                        }
                    }
                    bSim.Clear(); // 保存相似信息的列表可以清空了
                }
                bRating.Sort(); // 最终给分排序

                

                if (bRating.Count<=0) {
                    continue; // 没有推荐的则预测下一个用户
                }
                if (bRating[0].rating<h) {
                    continue; // 打分小于阈值则不用加入
                }
                rcm.Add(new Recommend(ubt[i].user_id));
                for (int m = 0; m < bRating.Count; m++) {
                    if (bRating[m].rating>=h) {
                        rcm[rcm.Count - 1].AddBrand(bRating[m]);
                    }
                }
                hvPrd.Clear();
                bRating.Clear();
            }
            
        }
        public void SavePredict(string path) {
            StreamWriter sw = new StreamWriter(path);
            for (int i = 0; i < rcm.Count; i++) {
                sw.Write(rcm[i].user_id + "\t");
                for (int j = 0; j < rcm[i].brands.Count; j++) {
                    sw.Write(rcm[i].brands[j].brand_id);
                    if (j!=rcm[i].brands.Count-1) {
                        sw.Write(",");
                    }
                }
                sw.WriteLine();
            }
            sw.Close();
        }

        public void Check(string dataFile) {
            List<Dt> dat = ReadData(dataFile);
            int hitN = 0, pN = 0, hitM = 0, pM = 0;
            for (int i = 0; i < rcm.Count; i++) {
                for (int j = 0; j < rcm[i].brands.Count; j++) {
                    pN++;
                    // 预测的商品，该用户在下个月购买了
                    if (dat.FindIndex(delegate(Dt one) { return one.user_id == rcm[i].user_id && one.brand_id == rcm[i].brands[j].brand_id && one.operation==AliDataInfo.OP_BUY; })>=0) {
                        hitN++;
                    }
                }
            }

            for (int i = 0; i < dat.Count; i++) {
                if (dat[i].operation==AliDataInfo.OP_BUY) {
                    pM++;
                    for (int j = 0; j < rcm.Count; j++) {
                        for (int k = 0; k < rcm[j].brands.Count; k++) {
                            if (rcm[j].user_id==dat[i].user_id && rcm[j].brands[k].brand_id==dat[i].brand_id) {
                                hitM++;
                            }
                        }
                    }
                }
            }
            double p = hitN / (double)pN;
            double r = hitM / (double)pM;
            double f = 2 * p * r / (p + r);
            Console.WriteLine("准确率：{0}",p);
            Console.WriteLine("召回率:{0}",r);
            Console.WriteLine("F-score：{0}",f);

        }
    }
}
