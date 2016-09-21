using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace tk.Data.Helper.Ali {
    public class DataRWHelper {
        public string pathPrefix = "E:/BigData/Ali/";
        public string orgDataPath = "t_alibaba_data.csv";
        public string trainDataPath = "train.csv";
        public string testDataPath = "test.csv";
        public int ORG_DATA = 1;
        public int TRAIN_DATA = 2;
        public int TEST_DATA = 3;

        /// <summary>
        /// 读取数据
        /// </summary>
        /// <param name="type">数据类型，1：原始，2：训练，3：测试</param>
        /// <returns></returns>
        public  List<Dt> ReadData(int type) {
            string path = pathPrefix;
            switch (type) {
                case 1: path += orgDataPath; break;
                case 2: path += trainDataPath; break;
                case 3: path += testDataPath; break;
                default:
                    break;
            }

            List<Dt> data = new List<Dt>();
            StreamReader sr = new StreamReader(path, Encoding.GetEncoding("gb2312"));
            if (type == 1) {
                sr.ReadLine(); // 原始数据跳去第一行
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
        public static void GenerateData(){

        }
    }
}
