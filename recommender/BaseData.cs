using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tk.Data.Helper.Ali {

    public class AliDataInfo {
        public static int StartTime = DateTime.Parse("2014/4/15").DayOfYear;
        public static int EndTime = DateTime.Parse("2014/7/15").DayOfYear;
    }
    // 保存一条几率,为了使Dt的列表能进行排序
    public class Dt  : IComparable<Dt>{
        public int user_id;
        public int brand_id;
        public int operation;
        public DateTime dt;
        public Dt(int uid, int bid, int op, DateTime t) {
            user_id = uid;
            brand_id = bid;
            operation = op;
            dt = t;
        }
        // 用于根据时间排序
        public int CompareTo(Dt t) {
            if (dt.CompareTo(t.dt)>0) {
                return -1;
            } else if (dt.CompareTo(t.dt)==0) {
                return 0;
            } else {
                return 1;
            }
        }
    }
    // 保存品牌给分情况
    public class BrandAndRating :IComparable<BrandAndRating> {
        public int brand_id;
        public double rating;
        public BrandAndRating(int bid, double rt) {
            brand_id = bid;
            rating = rt;
        }
        public int CompareTo(BrandAndRating br) {
            if (rating.CompareTo(br.rating)>0) {
                return -1;
            } else if (rating.CompareTo(br.rating)==0) {
                return 0;
            }else{
                return 1;
            }
        }
    }
    public class UserRecom {
        public int user_id;
        public List<BrandAndRating> brands;
        public UserRecom(int uid) {
            user_id = uid;
            brands = new List<BrandAndRating>();
        }
        public void Add(BrandAndRating br) {
            brands.Add(br);
        }
    }
    public class BrandInfo {
        public int brand_id;
        public int brand_count;
        public int brand_click;
        public BrandInfo(int bid, int bct, int bck) {
            brand_id = bid;
            brand_count = bct;
            brand_click = bck;
        }
    }
}
