using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace tk.Data.Helper.Ali {
    /// <summary>
    /// 阿里大数据比赛数据处理类
    /// </summary>
    class AliData {
        public List<Dt> data; // 保存所有数据
        public List<int> uList;
        public List<int> bList;
        public List<UserBrandTable> ubt;
        public double[,] itemSim;
        public double[,] prediction;
        
        public AliData(string p) {
            StreamReader sr;
            try {
                sr = new StreamReader(p, Encoding.GetEncoding("gb2312"));
            } catch (FileNotFoundException e) {
                throw new FileNotFoundException("File Not Found: " + e.ToString());
            }
            this.data = new List<Dt>();
            sr.ReadLine();
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
            uList = GetUserList();
            bList = GetBrandList();
            itemSim = new double[bList.Count,bList.Count]; // 初始化物品相似矩阵
            for (int i = 0; i < bList.Count; i++) {
                for (int j = 0; j < bList.Count; j++) {
                    itemSim[i, j] = 0;
                }
            }
            prediction = new double[uList.Count, bList.Count];
            for (int i = 0; i < uList.Count; i++) {
                for (int j = 0; j < bList.Count; j++) {
                    prediction[i, j] = 0;
                }
            }
        }
        /// <summary>
        /// 显示数据
        /// </summary>
        public void DisplayData() {
            for (int i = 0; i < data.Count; i++) {
                Console.WriteLine(data[i].user_id + " " + data[i].brand_id + " " + data[i].type + " " + data[i].visit_datetime.ToShortDateString());
            }
        }

        /// <summary>
        /// 根据时间段保存数据
        /// </summary>
        /// <param name="path">保存路径</param>
        /// <param name="start">起始时间</param>
        /// <param name="end">结束时间</param>
        public void SaveDataByTime(string path, DateTime start, DateTime end) {
            StreamWriter sw = new StreamWriter(path);
            string line;
            for (int i = 0; i < data.Count; i++) {
                if (data[i].visit_datetime.CompareTo(start) >= 0 && data[i].visit_datetime.CompareTo(end) < 0) {
                    line = data[i].user_id + "," + data[i].brand_id + "," + data[i].type + "," + data[i].visit_datetime.ToShortDateString();
                    sw.WriteLine(line);
                }
            }
            sw.Close();
        }

        /// <summary>
        /// 统计用户以及品牌数
        /// </summary>
        /// <returns>[用户数,品牌数]</returns>
        public int[] UserBrandCount() {
            List<int> user = new List<int>();
            for (int i = 0; i < data.Count; i++) {
                if (!user.Contains(data[i].user_id)) {
                    user.Add(data[i].user_id);
                }
            }
            List<int> item = new List<int>();
            for (int i = 0; i < data.Count; i++) {
                if (!user.Contains(data[i].brand_id)) {
                    item.Add(data[i].brand_id);
                }
            }
            int[] count = new int[2];
            count[0] = user.Count;
            count[1] = item.Count;
            return count;
        }
        /// <summary>
        /// 返回所有用户列表
        /// </summary>
        /// <returns>用户列表</returns>
        public List<int> GetUserList() {
            List<int> ulist = new List<int>();
            for (int i = 0; i < data.Count; i++) {
                if (!ulist.Contains(data[i].user_id)) {
                    ulist.Add(data[i].user_id);
                }
            }
            return ulist;
        }
        /// <summary>
        /// 返回所有品牌列表
        /// </summary>
        /// <returns>品牌列表</returns>
        public List<int> GetBrandList() {
            List<int> blist = new List<int>();
            for (int i = 0; i < data.Count; i++) {
                if (!blist.Contains(data[i].brand_id)) {
                    blist.Add(data[i].brand_id);
                }
            }
            return blist;
        }
        /// <summary>
        /// 得到用户-物品表
        /// </summary>
        /// <returns>用户物品表</returns>
        public List<UserBrandTable> AllUserBrandTable() {
            List<UserBrandTable> ubt = new List<UserBrandTable>();
            // 添加所有涉及的用户,按照uList的顺序，所以可以通过uList来查找用户索引
            for (int i = 0; i < uList.Count; i++) {
                ubt.Add(new UserBrandTable(uList[i]));
            }
            // 查询data中，对该用户点击、购买过的品牌查找
            for (int j = 0; j < data.Count; j++) {
                // 如果该用户的商品列表中不存该该物品则加入
                if (!ubt[uList.IndexOf(data[j].user_id)].brands.Contains(data[j].brand_id)) {
                    ubt[uList.IndexOf(data[j].user_id)].brands.Add(data[j].brand_id);
                }
            }
            return ubt;
        }

        public void GenerateItemSim() {
            int[] userCount = new int[bList.Count]; // 和品牌表对应的用户数目
            for (int i = 0; i < bList.Count; i++) {
                userCount[i] = 0;
            }
            // 对于每个用户计算，统计每个品牌关注人数,以及每两个之间关联度
            for (int i = 0; i < ubt.Count; i++) {
                for (int j = 0; j < ubt[i].brands.Count-1; j++) {
                    userCount[bList.IndexOf(ubt[i].brands[j])]++; //若该品牌出现一次，则品牌的用户统计对应的位置+1
                    for (int k = j+1; k < ubt[i].brands.Count; k++) {
                        itemSim[bList.IndexOf(ubt[i].brands[j]), bList.IndexOf(ubt[i].brands[k])] += 1;
                        itemSim[bList.IndexOf(ubt[i].brands[k]), bList.IndexOf(ubt[i].brands[j])] += 1;
                    }
                }
                // 最后一个加入
                userCount[bList.IndexOf(ubt[i].brands[ubt[i].brands.Count - 1])] += 1;
            }
            for (int i = 0; i < bList.Count; i++) {
                for (int j = 0; j < bList.Count; j++) {
                    itemSim[i, j] = itemSim[i, j] / Math.Sqrt(userCount[i] * userCount[j]);
                }
            }
            
        }
        public void Predict(int k) {
            prediction = new double[uList.Count, bList.Count];
            for (int i = 0; i < uList.Count; i++) {
                for (int j = 0; j < bList.Count; j++) {
                    if (j==0) { // 初始化
                        prediction[i, j] = 0;
                    }
                    // 计算j的k个相似品牌
                    Dictionary<int, double> bv = new Dictionary<int, double>();
                    for (int l = 0; l < bList.Count; l++) {
                        bv.Add(bList[l], itemSim[j, l]); //添加j行或者j列都可以
                    }
                    // 字典排序
                    bv = (from pair in bv orderby pair.Value descending select pair).ToDictionary(pair => pair.Key, pair => pair.Value);
                    List<KeyValuePair<int, double>> kv = bv.ToList();
                    for (int m = 0; m < k; m++) {
                        if (ubt[i].brands.Contains(kv[m].Key)) { // 用户喜欢品牌、与j品牌相似k个品牌集合
                            prediction[i, j] += itemSim[j, bList.IndexOf(kv[m].Key)]; // 喜欢指数预测
                        }
                    }
                    bv.Clear();
                }
                Console.WriteLine("预测完成: "+uList[i]);
            }
        }
        public void Predict(int userStart,int userEnd,int k) {
            for (int i = userStart; i < userEnd; i++) {
                for (int j = 0; j < bList.Count; j++) {
                    // 如果i的列表中已经存在了j物品则不用计算了
                    //if (ubt[i].brands.Contains(bList[j])) {
                    //    continue;
                    //}
                    // 计算j的k个相似品牌
                    Dictionary<int, double> bv = new Dictionary<int, double>();
                    for (int l = 0; l < bList.Count; l++) {
                        bv.Add(bList[l], itemSim[j, l]); //添加j行或者j列都可以
                    }
                    // 字典排序
                    bv = (from pair in bv orderby pair.Value descending select pair).ToDictionary(pair => pair.Key, pair => pair.Value);
                    List<KeyValuePair<int, double>> kv = bv.ToList();
                    double tmp=0;
                    // 取k个相似品牌
                    for (int m = 0; m < k; m++) {
                        if (ubt[i].brands.Contains(kv[m].Key)) { // 用户喜欢品牌、与j品牌相似k个品牌集合
                            double a = itemSim[j, bList.IndexOf(kv[m].Key)];
                            double b = Rating(uList[i], kv[m].Key);
                            tmp += a;
                            prediction[i, j] += a*b*(k-m)/(double)k;
                            //Console.WriteLine(uList[i]+","+kv[m].Key+": "+ a +"*"+b);
                            //prediction[i, j] += itemSim[j, bList.IndexOf(kv[m].Key)] * Rating(uList[i], kv[m].Key, 0.001, -0.004, 0.002, 0.003); // 喜欢指数预测
                        }
                    }
                    //prediction[i, j] = prediction[i, j]/tmp;
                    bv.Clear();
                }
                Dictionary<int, double> uipd = new Dictionary<int, double>();
                for (int n = 0; n < bList.Count; n++) {
                    uipd.Add(bList[n], prediction[i, n]);
                }
                uipd = (from pair in uipd orderby pair.Value descending select pair).ToDictionary(pair => pair.Key, pair => pair.Value);
                List<KeyValuePair<int,double>> uikv = uipd.ToList();
                if (uikv[0].Value>0) { // 只显示比较大的
                    //显示三组
                    Console.WriteLine("预测完成: " + uList[i]);
                    Console.WriteLine("\t1st Recom: " + uikv[0].Key + "\tpresice: " + uikv[0].Value);
                    Console.WriteLine("\t2nd Recom: " + uikv[1].Key + "\tpresice: " + uikv[1].Value);
                    Console.WriteLine("\t3rd Recom: " + uikv[2].Key + "\tpresice: " + uikv[2].Value);
                    Console.WriteLine("\t4th Recom: " + uikv[3].Key + "\tpresice: " + uikv[3].Value);
                    Console.WriteLine("\t4th Recom: " + uikv[4].Key + "\tpresice: " + uikv[4].Value);
                }

            }
        }
        public double Rating(int uid, int bid) {
            double _p1 = 0, _p2 = 0, _p3 = 0, _p4 = 0;
            bool flag = false;
            int count = 0;
            for (int i = 0; i < data.Count; i++) {
                if ((data[i].user_id == uid) && (data[i].brand_id == bid)) {
                    count++;
                    //if (data[i].type == 0) {
                    //    //_p1 += 1;
                    //    return 1.1;
                    //}
                    //if (data[i].type == 1) {
                    //    //_p2 -= 4;
                    //    //flag = true;
                    //    return .6;
                    //}
                    //if (data[i].type == 2) {
                    //    //_p3 += 2;
                    //    return 1.2;
                    //}
                    //if (data[i].type == 3) {
                    //    //_p4 += 3;
                    //    return 1.3;
                    //}
                    if (data[i].type == 1) {
                        return 0.8;
                    }
                }
            }
            return 1.0;
            
        }
    }
}
