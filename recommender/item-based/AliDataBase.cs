using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tk.Data.Helper.Ali {
    // -------------------------------------------------------------------------
    /// <summary>
    /// （season1）一条数据记录，即一行
    /// </summary>
    public class Dt {
        public int user_id; // 和原始数据中对应
        public int brand_id;
        public int type;
        public DateTime visit_datetime;
        public Dt(int uid, int bid, int tp, DateTime vdate) {
            user_id = uid;
            brand_id = bid;
            type = tp;
            visit_datetime = vdate;
        }
    }
    // -------------------------------------------------------------------------
    /// <summary>
    /// 对某个用户的推荐
    /// </summary>
    public class UserBrandRecm {
        public int user_id;
        public List<int> brands;
        public UserBrandRecm(int uid) {
            user_id = uid;
            brands = new List<int>();
        }
        public void AddBrand(int bid) {
            brands.Add(bid);
        }
    }
    // -------------------------------------------------------------------
    /// <summary>
    /// 用户物品表
    /// </summary>
    public class UserBrandTable {
        public int user_id;
        public List<int> brands;
        public UserBrandTable(int uid) {
            user_id = uid;
            brands = new List<int>();
        }
        public void AddBrand(int bid){
            brands.Add(bid);
        }
    }
}
