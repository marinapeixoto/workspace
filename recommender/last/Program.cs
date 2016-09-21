using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using tk.Ali;

namespace AliSeasonOne {
    class Program {
        static void Main(string[] args) {
            AliBigData ali = new AliBigData();

            ali.Analysis();
            Console.WriteLine("分析完成");
            
            ali.BrandSimilarity();
            Console.WriteLine("相似度计算完成");

            ali.Predict(AliDataInfo.KSIZE, AliDataInfo.SSIZE, AliDataInfo.PARAM_H);
            Console.WriteLine("预测完成！");

            ali.SavePredict(AliDataInfo.pathPrefix + "result_4_9_1.txt");
            Console.WriteLine("保存文件完成");

            Console.WriteLine("评估中...");
            ali.Check(AliDataInfo.TestDataFile);
        }
    }
}
