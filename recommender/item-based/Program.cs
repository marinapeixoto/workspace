using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using tk.Data.Helper.Ali;

namespace aliBigData_Item_based {
    class Program {
        /// <summary>
        /// 产生训练数据和测试数据
        /// </summary>
        static void GenerateData() {
            string pathPrefix = "E:/BigData/Ali/";
            AliData ali = new AliData(pathPrefix + "t_alibaba_data.csv");
            ali.SaveDataByTime(pathPrefix + "train.csv", DateTime.Parse("2014年4月15日"), DateTime.Parse("2014年7月14日"));
            Console.WriteLine("Train Data Saved!");
            ali.SaveDataByTime(pathPrefix + "test.csv", DateTime.Parse("2014年7月15日"), DateTime.Parse("2014年8月15日"));
            Console.WriteLine("Test Data Saved!");
        }
        static void Main(string[] args) {
            string pathPrefix = "E:/BigData/Ali/";
            // 读取训练数据
            AliData ali = new AliData(pathPrefix + "train.csv");

            ali.ubt = ali.AllUserBrandTable(); // 获得用户-物品表
            Console.WriteLine("获取用户-品牌表完成!");
            ali.GenerateItemSim(); // 生成相似矩阵
            Console.WriteLine("品牌相似矩阵计算完成!");
            Console.WriteLine("正在预测.......");
            // 对每个用户预测
            for (int i = 0; i < ali.uList.Count; i++) {

            }

        }
    }
}
