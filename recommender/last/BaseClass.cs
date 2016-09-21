using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace tk.Ali {
    // ---------------------------------------------------------------------------------
    /// <summary>
    /// 保存相关配置,主要是配置
    /// </summary>
    public class AliDataInfo {
        public static int StartTime = DateTime.Parse("2014/4/15").DayOfYear; // 起始数据时间
        public static int EndTime = DateTime.Parse("2014/8/15").DayOfYear; // 数据结束时间

        public static string pathPrefix = "E:/BigData/Ali/";

        public static string OrgDataFile = "t_alibaba_data.csv";
        public static string TrainDataFile = "train.csv";
        public static string TestDataFile = "test.csv";

        public static int OP_CLICK = 0; // 点击
        public static int OP_BUY = 1; // 购买
        public static int OP_FAV = 2; // 收藏
        public static int OP_ADD = 3; // 加入购物车

        public static double CLICK_WEIGHT = 1.3;
        public static double FAV_WIEGHT = 2.8;
        public static double ADD_WEIGHT = 4.0;

        public static double PARAM_MOD = 3.4;

        public static int KSIZE = 6; // 5,2
        public static int SSIZE = 6;
        public static double PARAM_H = 0.4; // 0.35

    }

    // -----------------------------------------------------------------------------------
    /// <summary>
    /// 保存一条原始的数据,变量和csv文件中列对应
    /// 支持比较大小，实现排序
    /// </summary>
    public class Dt:IComparable<Dt> {
        public int user_id;
        public int brand_id;
        public int operation;
        public DateTime dtime;
        public Dt(int uid, int bid, int op, DateTime dt) {
            user_id = uid;
            brand_id = bid;
            operation = op;
            dtime = dt;
        }
        public int CompareTo(Dt t) {
            return dtime.CompareTo(t.dtime); // 根据时间排序,升序
        }
    }
    // ------------------------------------------------------------------------------------
    /// <summary>
    /// 品牌以及其打分
    /// </summary>
    public class BrandAndRating : IComparable<BrandAndRating> {
        public int brand_id;
        public double rating;
        public BrandAndRating(int bid, double rt) {
            brand_id = bid;
            rating = rt;
        }
        public int CompareTo(BrandAndRating br) {
            return br.rating.CompareTo(rating); // 实现降序排列
        }
    }
    // -------------------------------------------------------------------------------------
    /// <summary>
    /// 对该用户的推荐商品
    /// </summary>
    public class Recommend {
        public int user_id;
        public List<BrandAndRating> brands;
        public int brandCount;
        public Recommend(int uid) {
            user_id = uid;
            brands = new List<BrandAndRating>();
            brandCount = 0;
        }
        public void AddBrand(BrandAndRating br) {
            brands.Add(br);
            brandCount++;
        }
    }
    // -------------------------------------------------------------------------------------
    /// <summary>
    /// 保存某件商品的关注次数以及购买次数，计算其购买率
    /// </summary>
    public class BrandInfo {
        public int brand_id;
        public int brand_count;
        public int brand_buy;

        public BrandInfo(int bid, int bct, int bb) {
            brand_id = bid;
            brand_count = bct;
            brand_buy = bb;
        }
        /// <summary>
        /// 购买率
        /// </summary>
        public double BuyRatio {
            get { return (brand_buy)/(double)(brand_count); }
        }
    }
    // --------------------------------------------------------------------------------------
    public class UserBrandTable {
        public int user_id;
        public List<BrandAndRating> brands;
        /// <summary>
        /// 传入一个用户的所有数据
        /// </summary>
        /// <param name="ud">该用户原始数据表</param>
        public UserBrandTable(List<Dt> ud) {
            user_id = ud[0].user_id; // 用户名
            ud.Sort(); // 按时间先后排序
            brands = new List<BrandAndRating>();

            // 遍历表得到每个品牌以及其打分
            for (int i = 0; i < ud.Count; i++) {
                
                double p = (ud[i].dtime.DayOfYear - AliDataInfo.StartTime) / (double)(AliDataInfo.EndTime - AliDataInfo.StartTime); // 一个随时间变化的因子

                // 当前数据的品牌id是否已经加入？
                int index = brands.FindIndex(delegate(BrandAndRating br) { return br.brand_id == ud[i].brand_id; });
                if (index<0) {
                    brands.Add(new BrandAndRating(ud[i].brand_id, 0.0));
                    index = brands.FindIndex(delegate(BrandAndRating br) { return br.brand_id == ud[i].brand_id; }); // 获取加入后的位置
                }
                // 如果是购买了，清除该品牌的打分记录
                if (ud[i].operation == AliDataInfo.OP_BUY) {
                    brands[index].rating = 0.0;
                } else if (ud[i].operation == AliDataInfo.OP_CLICK) {
                    brands[index].rating += AliDataInfo.CLICK_WEIGHT*Math.Pow(p,2); // 打分是随时间变化的
                } else if (ud[i].operation==AliDataInfo.OP_FAV) {
                    brands[index].rating += AliDataInfo.FAV_WIEGHT * Math.Pow(p, 2);
                } else {
                    brands[index].rating += AliDataInfo.ADD_WEIGHT*Math.Sqrt(p);
                }
            }
            
        }
        
    }

}
