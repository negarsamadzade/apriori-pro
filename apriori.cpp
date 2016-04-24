using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
namespace apriori
{
    class Program
    {
        class itemset
        {
            public string item;
            public int sup = 0;
            public itemset(string st)
            {
                item = st;
            }
           
            public itemset()
            {
                
            }

            //find a string in another string
            public bool find(string str, string str1)
            {
                
                char[] arr1 = str1.ToCharArray();
                bool f = false;
                bool[] arr2 = new bool[arr1.Length];
                int n = 0;
                for (int j = 0; j < arr1.Length; j++)
                {
                    if (str.Contains(arr1[j])) { f = true; arr2[n] = f; n++; }
                    else { f = false; arr2[n] = f; n++; }
                }
                if (arr1.Length > 1)
                {
                    bool B, C;
                    B = arr2[0] && arr2[1];
                    for (int i = 2; i < arr2.Length; i++)
                    {
                        C = B && arr2[i];
                        B = C;
                    }
                    if (B == true) return true;
                    else return false;
                }
                else
                {
                    if (arr2[0] == true) return true;
                    else return false;
                }


            }
            //join of 2 string
            public string join(string str1, string str2)
            {
                string result = str1;
                char[] str1arr = str1.ToCharArray();
                char[] str2arr = str2.ToCharArray();
                for (int i = 0; i < str2arr.Length; i++)
                {
                    if (!find(str1, str2arr[i].ToString()))
                        result += str2arr[i].ToString();

                }
                return result;
            }
            //find spport_count of any itemset
            public void itemset_sup(List<string> sourcelist, List<itemset> a)
            {
                for (int i = 0; i < a.Count; i++)
                {
                    for (int j = 0; j < sourcelist.Count; j++)
                    {
                        if (find(sourcelist[j], a[i].item))
                            a[i].sup++;

                    }
                }
            }
            public List<itemset> item_1 = new List<itemset>();
            //find items in input data
            public List<itemset> itemsets1(List<string> source)
            {
                string ss = "";
                List<string> item_2 = new List<string>();
                for (int i = 0; i < source.Count; i++)
                    ss += source[i] + " ";
                string[] array1 = ss.Split(' ', ';');
                for (int i = 0; i < array1.Length - 1; i++)
                {
                    itemset p = new itemset(array1[i]);
                    item_1.Add(p);

                }
                for (int i = 0; i < item_1.Count; i++)  
                   item_2.Add(item_1[i].item);
                 List<string> distinct = item_2.Distinct().ToList() ;
                item_1.Clear();
                 for(int i=0;i<distinct.Count;i++){
                     itemset t = new itemset(distinct[i]);
                     item_1.Add(t);
                 }
                 return item_1;
            }
            //find frequent items in itemsets
            public List<itemset> frequents(List<itemset> C, int min_s)
            {
                int j = 0;
                int leng = C.Count; 
                for (int i = 0; i < leng; i++)
                {
                    if (C[j].sup < min_s)
                    {
                        C.Remove(C[j]); i--; leng = C.Count;
                    }
                    else j++;
                }
                return C;
            }

            //find frequent 1_itemset
            public List<itemset> find1_itemset(List<string> source, int min_sup)
            {
                frequents(item_1, min_sup);
                return item_1;
            }
            //find all frequent itemsets
            public List<itemset> frequent_itemsets(List<string> source, int min_sup)
            {
                string joinst;
                List<itemset> L1 = new List<itemset>();
                List<string> des;
                List<itemset> frequent;
                List<itemset> temp = new List<itemset>();
                L1 = find1_itemset(source, min_sup);
                while (L1 != null && L1.Count!=1&&L1.Count!=0)
                {
                    frequent = new List<itemset>();
                    des = new List<string>();
                    for (int i = 0; i < L1.Count; i++)
                    {
                        for (int j = i + 1; j < L1.Count; j++)
                        {
                            joinst = join(L1[i].item, L1[j].item);
                            itemset joinitem = new itemset(joinst);
                            frequent.Add(joinitem);
                        }
                    }
                    for (int i = 0; i < frequent.Count; i++)
                        des.Add(frequent[i].item);
                    int len = des.Count;
                    for(int i=0;i<len;i++){
                        for(int j=i+1;j<len;j++){
                            if (des[i].Length == des[j].Length && find(des[i], des[j])) { des.Remove(des[j]); len = des.Count; j--; }

                        }
                    }
                    frequent = new List<itemset>();
                    for (int i = 0; i < des.Count; i++)
                    {

                        itemset g = new itemset(des[i]);
                        frequent.Add(g);
                    }
                    itemset_sup(source, frequent);
                    frequents(frequent, min_sup);
                    temp = L1;
                    L1 = frequent;
                   if (L1.Count == 1) return L1;


                }
                return temp;
            }

//receive path of input data
            public static List<string> filecsv(string filepath)
            {

                var column2 = new List<string>();
                using (var rd = new StreamReader(filepath))
                {
                    while (!rd.EndOfStream)
                    {
                        var splits = rd.ReadLine().Split(',');
                        column2.Add(splits[1]);
                    }
                }
                List<string>source = column2.ToList<string>();
                return source;
            }
        }
    

        static void Main(string[] args)
        {
           
            
            Console.WriteLine("please enter the path of file:");
            string path = Console.ReadLine();
            Console.WriteLine("please enter min_sup:");
            int sup = int.Parse(Console.ReadLine());
            List<string>sourcelist=new List<string>();
            itemset t = new itemset();
            sourcelist = itemset.filecsv(path);
            List<itemset>ll=new List<itemset>();
            List<itemset> result = new List<itemset>();
            ll=t.itemsets1(sourcelist);  
            t.itemset_sup(sourcelist, ll);
            result=t.frequent_itemsets(sourcelist, sup);
            Console.WriteLine("frequent itemsets are:");
            if (result != null)
            {
                for (int i = 0; i < result.Count; i++)
                    Console.WriteLine("{" + result[i].item + "}" + " " + result[i].sup.ToString());
            }
            else Console.WriteLine("there is not any frequent itemset");
            Console.ReadLine();
        }
    }
}
