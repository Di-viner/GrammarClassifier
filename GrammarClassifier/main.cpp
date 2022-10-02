#include<iostream>
#include<vector>
#include<string>
#include <map>
using namespace std;

vector<string> VNs;				//非终止符号Vn
vector<string> VTs;				//终止符号Vt
multimap<string, string> mapP;	//产生式P
string S;						//开始符号

vector<string> split(string str, string separator) //自定义实现split函数
{
	vector<string> result;
	int cutAt;
	while ((cutAt = str.find_first_of(separator)) != str.npos) 
	{
		if (cutAt > 0) 
		{
			result.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if (str.length() > 0) 
	{
		result.push_back(str);
	}
	return result;
}


string ClearAllSpace(string str) //清楚字符串的空格
{
	int index = 0;
	if (!str.empty()) 
	{
		while ((index = str.find(' ', index)) != str.npos) 
		{
			str.erase(index, 1);
		}
	}
	return str;
}

vector<string> getVNs(string VN)
{
	vector<string> VNs;
	VN = ClearAllSpace(VN);
	VNs = split(VN, ",");
	return VNs;
}
multimap<string, string> getMapP(string P)	//产生式字符串P转换为multimap，key为左部，value为右部
{
	multimap<string, string> mapP;
	P = ClearAllSpace(P);
	vector<string> Ps = split(P, ",");
	for (auto p : Ps)
	{
		vector<string> leftRightP = split(p, "->");
		vector<string> rights = split(leftRightP[1], "|");
		for (auto right : rights)
			mapP.insert({ leftRightP[0], right });
	}
	return mapP;
}

vector<string> getVTs(multimap<string, string> mapP)	//通过非终止符号Vn和产生式P确定终止符号
{
	vector<string> VTs;
	for (auto kv : mapP)
	{
		string left = kv.first;
		string right = kv.second;
		for (int i = 0; i < left.length(); i++)
		{
			string s;
			s.push_back(left[i]);
			if (find(VNs.begin(), VNs.end(), s) == VNs.end() && find(VTs.begin(), VTs.end(), s) == VTs.end())
				VTs.push_back(s);
		}
		for (int i = 0; i < right.length(); i++)
		{
			string s;
			s.push_back(right[i]);
			if (find(VNs.begin(), VNs.end(), s) == VNs.end() && s != "ε" && find(VTs.begin(), VTs.end(), s) == VTs.end())
				VTs.push_back(s);
		}
	}
	return VTs;
}

int judgeGrammar0or1(multimap<string, string> mapP)
{
	for (auto kv : mapP)
	{
		string left = kv.first;
		string right = kv.second;
		if (left.length() > right.length())
			return 0;
	}
	return 1;
}

int judgeGrammar2or3(multimap<string, string> mapP)
{
	int flag = 2;	//0为左线性，1为右线性，初始值为2
	for (auto kv : mapP)
	{
		string left = kv.first;
		string right = kv.second;
		if (right.length() > 2)
			return 2;
		if (right.length() == 2)
		{
			string rFirst, rSecond;
			rFirst.push_back(right[0]);
			rSecond.push_back(right[1]);
			if (find(VNs.begin(), VNs.end(), rFirst) != VNs.end() && find(VTs.begin(), VTs.end(), rSecond) != VTs.end() && flag != 1)
			{
				flag = 0;
			}
			else if (find(VNs.begin(), VNs.end(), rSecond) != VNs.end() && find(VTs.begin(), VTs.end(), rFirst) != VTs.end() && flag != 0)
			{
				flag = 1;
			}
			else
				return 2;
		}
		if (right.length() == 1)
		{
			string rFirst;
			rFirst.push_back(right[0]);
			if (find(VTs.begin(), VTs.end(), rFirst) == VTs.end() && rFirst != "ε")
				return 2;
		}
	}
	return 3;
}

int judgeGrammar(multimap<string, string> mapP)
{
	for (auto kv : mapP)
	{
		string left = kv.first;
		if (left.length() > 1)
			return judgeGrammar0or1(mapP);
	}
	return judgeGrammar2or3(mapP);
}

int main()
{
	string G, P, VN;
	int n;
	cout << "输入文法G: ";
	getline(cin, G);
	cout << "P: ";
	getline(cin, P);
	cout << "Vn: ";
	getline(cin, VN);

	S = G[2];			//因为文法符号均为单字符符号，可用G[2]确定开始符号
	mapP = getMapP(P);	//通过P确定multimapP
	VNs = getVNs(VN);	
	VTs = getVTs(mapP);
	n = judgeGrammar(mapP);

	cout << endl << endl;
	cout << "文法G[" + S + "]" + " = (Vn, Vt, P, " + S + ")" << endl;
	cout << "Vn = {";
	for (auto a : VNs)
		cout << a << ", ";
	cout << "\b\b" << "}" << endl;
	cout << "Vt = {";
	for (auto a : VTs)
		cout << a<<", ";
	cout << "\b\b" << "}" << endl;
	cout << "P = {" + P + "}" << endl;
	cout << "该文法是Chomsky" << judgeGrammar(mapP) << "型文法";
	return 0;
}