using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tk.Data.Helper.Ali {
    
    // 保存一个用户的品牌以及其打分
    public class UserBrandTable {
        public int user_id; // 用户id
        public List<BrandAndRating> brands;
        /// <summary>
        /// 传入一个用户的数据，得到其用户品牌表
        /// </summary>
        /// <param name="data">该用户的原始数据表</param>
        public UserBrandTable(List<Dt> data) {
            user_id = data[0].user_id;
            data.Sort(); //按时间顺序排序
            brands = new List<BrandAndRating>();
            // 按时间序列遍历品牌
            for (int i = 0; i < data.Count; i++) {
                double rating = .0;
                double p = (data[i].dt.DayOfYear - AliDataInfo.StartTime) / (double)(AliDataInfo.EndTime - AliDataInfo.StartTime); // 一个加权量
                // 打分
                if (data[i].operation == 0) {
                    rating += Math.Pow(p, 2);
                } else if (data[i].operation == 1) {
                    rating = 0;
                } else if (data[i].operation == 2) {
                    rating += 3 * p;
                } else {
                    rating += 5 * Math.Sqrt(p);
                }
                int index = brands.FindIndex(delegate(BrandAndRating br) { return br.brand_id == data[i].brand_id; });
                // brands中已经有了该品牌id
                if (index >= 0) {
                    // 有购买则打分清零
                    if (data[i].operation == 1) {
                        brands[index].rating = 0;
                    } else {
                        brands[index].rating += rating;
                    }
                } else {
                    brands.Add(new BrandAndRating(data[i].brand_id, rating));
                }
            }
        }
    }
}
