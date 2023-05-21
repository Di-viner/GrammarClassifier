#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

vector<string> VNs;            // ����ֹ����Vn
vector<string> VTs;            // ��ֹ����Vt
multimap<string, string> mapP; // ����ʽP
string S;                      // ��ʼ����

vector<string> split(string str, string separator) // �Զ���ʵ��split����
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

string ClearAllSpace(string str) // ����ַ����Ŀո�
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

multimap<string, string> getMapP(string P) // ����ʽ�ַ���Pת��Ϊmultimap��keyΪ�󲿣�valueΪ�Ҳ�
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

vector<string> getVTs(multimap<string, string> mapP) // ͨ������ֹ����Vn�Ͳ���ʽPȷ����ֹ����
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
            if (find(VNs.begin(), VNs.end(), s) == VNs.end() && s != "��" && find(VTs.begin(), VTs.end(), s) == VTs.end())
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
    int flag = 2; // 0Ϊ�����ԣ�1Ϊ�����ԣ���ʼֵΪ2
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
            if (find(VTs.begin(), VTs.end(), rFirst) == VTs.end() && rFirst != "��")
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
    cout << "�����ķ�G: ";
    getline(cin, G);
    cout << "P: ";
    getline(cin, P);
    cout << "Vn: ";
    getline(cin, VN);

    S = G[2];          // ��Ϊ�ķ����ž�Ϊ���ַ����ţ�����G[2]ȷ����ʼ����
    mapP = getMapP(P); // ͨ��Pȷ��multimapP
    VNs = getVNs(VN);
    VTs = getVTs(mapP);
    n = judgeGrammar(mapP);

    cout << endl
        << endl;
    cout << "�ķ�G[" + S + "]" + " = (Vn, Vt, P, " + S + ")" << endl;
    cout << "Vn = {";
    for (auto a : VNs)
        cout << a << ", ";
    cout << "\b\b"
        << "}" << endl;
    cout << "Vt = {";
    for (auto a : VTs)
        cout << a << ", ";
    cout << "\b\b"
        << "}" << endl;
    cout << "P = {" + P + "}" << endl;
    cout << "���ķ���Chomsky" << judgeGrammar(mapP) << "���ķ�";
    return 0;
}