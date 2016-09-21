using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;


/*
 * 
 * 加油站选址问题 
 * Kmeans方法求解
 * 
 */

namespace GasStations {
    class Program {
        static void Main(string[] args) {
            KMeans km = new KMeans(2);
            
            List<PointF> pt = new List<PointF>();
            pt.Add(new PointF(0, 0));
            pt.Add(new PointF(45, 30));
            pt.Add(new PointF(25, 47));
            pt.Add(new PointF(10, 3));
            pt.Add(new PointF(35, 45));
            pt.Add(new PointF(16, 12));
            pt.Add(new PointF(50, 39));
            pt.Add(new PointF(12, 5));
            km.SetData(pt);
            km.Clustering(new PointF(0, 0), new PointF(50, 50));
            km.ShowResult();
        }
    }
}
