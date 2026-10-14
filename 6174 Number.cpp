#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
int asc(int x)
{
    string s=to_string(x);
    sort(s.begin(), s.end());
    return stoi(s);
}
int des(int x)
{
    string s=to_string(x);
    sort(s.begin(), s.end(), greater<char>());
    return stoi(s);
}
int main()
{
    int n,k=0;
    cout<<"Enter a 4 digit number of differnt digits: "<<endl;
    cin>>n;
    int i;
    while(n!=6174)
    {
        cout<<des(n)<<"-"<<asc(n)<<" = "<<des(n)-asc(n)<<endl;
        n = des(n)-asc(n);
        k=k+1;
        if(n==0)
        {
            cout<<"ERROR !!   THE DIGITS CANNOT ALL BE THE SAME."<<endl;
            return 0;
        }
    }
    cout<<"No. of iterations takens is "<<k<<endl;
    return 0;
}