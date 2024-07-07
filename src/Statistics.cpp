/*
code by utf-8
by zhywyt
date 2024/7/5
此文件用于2024年春，甲壳虫跑团的打卡统计功能实现
提取微信聊天文件使用了WeChatMsg
https://github.com/LC044/WeChatMsg

打卡规则参考《甲壳虫跑团打卡规则3.0》
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include "replaceCharactersInFile.hpp"
// —————————————————————————————————————————————————
#ifdef _WIN32
#include <direct.h>		//for mkdir rmdir
#include <io.h>			//for access
#elif __linux__
#include <unistd.h>		//for mkdir rmdir
#include <sys/stat.h>	//for access
#include <dirent.h>		//for DIR remove
#endif
 
#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define RMDIR(a) _rmdir((a))
#elif __linux__
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#define RMDIR(a) rmdir((a))
#endif

bool MkDir(const std::string& strPath)
{
	int i = 0;
	int nDirLen = strPath.length();
	if (nDirLen <= 0)
		return false;
	char *pDirTemp = new char[nDirLen + 4];
	strPath.copy(pDirTemp, nDirLen + 1, 0);// +1 to copy '\0'
	pDirTemp[nDirLen] = '\0';
	//在末尾加'/'
	if (pDirTemp[nDirLen - 1] != '\\' && pDirTemp[nDirLen - 1] != '/')
	{
		pDirTemp[nDirLen] = '/';
		pDirTemp[nDirLen + 1] = '\0';
		nDirLen++;
	}
	// 创建目录
	for (i = 0; i < nDirLen; i++)
	{
		if (pDirTemp[i] == '\\' || pDirTemp[i] == '/')
		{
			pDirTemp[i] = '\0';//截断后面的子目录，逐级查看目录是否存在，若不存在则创建
			//如果不存在,创建
			int statu;
			statu = ACCESS(pDirTemp, 0);
			if (statu != 0)//可能存在同名文件导致没有创建
			{
				statu = MKDIR(pDirTemp);
				if (statu != 0)//可能上级不是文件夹而是同名文件导致创建失败
				{
					return false;
				}
			}
			//支持linux,将所有\换成/
			pDirTemp[i] = '/';
		}
	}
	delete[] pDirTemp;
	return true;
}
bool RmDir(const std::string & path)
{
	std::string strPath = path;
#ifdef _WIN32
	struct _finddata_t fb;   //查找相同属性文件的存储结构体
	//制作用于正则化路径
	if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/')
		strPath.append("\\");
	std::string findPath = strPath + "*";
	intptr_t handle;//用long类型会报错
	handle = _findfirst(findPath.c_str(), &fb);
	//找到第一个匹配的文件
	if (handle != -1L)
	{
		std::string pathTemp;
		do//循环找到的文件 
		{
			//系统有个系统文件，名为“..”和“.”,对它不做处理  
			if (strcmp(fb.name, "..")!=0 && strcmp(fb.name, ".")!=0)//对系统隐藏文件的处理标记
			{
				//制作完整路径
				pathTemp.clear();
				pathTemp = strPath + std::string(fb.name);
				//属性值为16，则说明是文件夹，迭代  
				if (fb.attrib == _A_SUBDIR)//_A_SUBDIR=16
				{
					RmDir(pathTemp.c_str());
				}
				//非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。  
				else
				{
					remove(pathTemp.c_str());
				}
			}
		} while (0 == _findnext(handle, &fb));//判断放前面会失去第一个搜索的结果
		//关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir  
		//经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。  
		_findclose(handle);
	}
	//移除文件夹  
	return RMDIR(strPath.c_str())==0?true:false;
 
#elif __linux__
	if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/')
		strPath.append("/");
	DIR *d = opendir(strPath.c_str());//打开这个目录
	if (d != NULL)
	{ 
		struct dirent *dt = NULL;
		while (dt = readdir(d))//逐个读取目录中的文件到dt
		{
			//系统有个系统文件，名为“..”和“.”,对它不做处理
			if (strcmp(dt->d_name, "..")!=0 && strcmp(dt->d_name, ".")!=0)//判断是否为系统隐藏文件
			{
				struct stat st;//文件的信息
				std::string fileName;//文件夹中的文件名
				fileName = strPath + std::string(dt->d_name);
				stat(fileName.c_str(), &st);
				if (S_ISDIR(st.st_mode))
				{
					RmDir(fileName);
				}
				else
				{
					remove(fileName.c_str());
				}
			}
		}
		closedir(d);
	}
	return rmdir(strPath.c_str())==0?true:false;
#endif
 
}
// 版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
// 原文链接：https://blog.csdn.net/venom_snake/article/details/88066475

using std::cin,std::cout,std::ifstream,std::ofstream,std::endl,std::string,std::map,std::vector,std::to_string;
using std::istringstream,std::istream,std::pair,std::sort,std::make_pair,std::atoi,std::getline,std::size_t;


// 计算一个字符串中某个子串出现的次数
int countOccurrences(const std::string& str, const std::string& substr) {
    int count = 0;
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        ++count;
        pos += substr.length();
    }
    return count;
}
void usage(){
    cout << "usage: ./Statistics /path/to/WeChatMsg/ChatExport.txt /path/to/WeChatMsg/ChatExport2.txt" << endl;
}
bool isStartMsg(string&line){
    if(line.size() > 19){
        // 2023-09-11 22:46:21
        string time = line.substr(0,19);
        // 打卡 1 张慧源 12km
        if(line[4]==' '&&line[7]==' '&&line[10]==' '&&line[13]==':'&&line[16]==':'){
            return true;
        }
    }
    return false;
}
// 星期0 到6
int WhichDayOfAWeek(int year,int mouth,int day){
    if(mouth == 1 || mouth == 2){
        mouth += 12;
        year--;
    }
    int week = (day + 2*mouth + 3*(mouth+1)/5 + year + year/4 - year/100 + year/400) % 7+1;
    return week;
}
float ReadAfloat(istream& is){
    char c;
    float f;
    // 去除前导空格
    do{
        if(!(is>>c))throw "There is no more char";
    }while(!(c>='0'&&c<='9'));
    // 读取小数点前的部分
    f = c-'0';
    while(is>>c){
        if(c>='0'&&c<='9'){
            f = f*10 + c-'0';
        }else if(c=='.'){
            break;
        }
        else {
            return f;
        }
    }
    // 读取小数点后的部分
    float d = 0.1;
    while(is>>c){
        if(c>='0'&&c<='9'){
            f += d*(c-'0');
            d *= 0.1;
        }else{
            break;
        }
    }
    return f;
}
//return a>b
bool DateBigger(string a,string b){
    int year = atoi(a.substr(0,4).c_str());
    int mouth = atoi(a.substr(5,2).c_str());
    int day = atoi(a.substr(8,2).c_str());
    int bYear = atoi(b.substr(0,4).c_str());
    int bMouth = atoi(b.substr(5,2).c_str());
    int bDay = atoi(b.substr(8,2).c_str());
    if(year>bYear){
        return true;
    }
    else if(year==bYear){
        if(mouth>bMouth){
            return true;
        }
        else if(mouth==bMouth){
            if(day>bDay){
                return true;
            }
        }
    }
    return false;
}
//2024 03 04
bool DateBetween(string date, string start,string end){
    if(date.size() != 10 || start.size() != 10 || end.size() != 10){
        return true;
    }
    return DateBigger(date,start)&&DateBigger(end,date);
}

/*
main 函数接受命令行参数
 1 聊天记录文件路径
usage:
./Statistics /path/to/WeChatMsg/ChatExport.txt

打卡格式
    2023-09-11 22:46:21 一行
    打卡 1 张慧源 12km
*/
#define Mov if(getline(ifs,line)){lineNum++;}else{break;}
int main(int argc, char *argv[]){
    if(argc < 2){
        usage();
        exit(-1);
    }
    map<char,char>replaceMap={{'-',' '}};
    for(int i = 1;i<argc;i++){
        if(replaceCharactersInFile(argv[i],replaceMap)){
            cout << "replace file " << argv[i] << " success" << endl;
        }
    }
    std::map<ifstream*, string> stream_file_table;
    vector<ifstream> ifsv(argc-1);
    for(int i = 1;i<argc;i++){
        ifsv[i-1].open(argv[i]);
        if(!ifsv[i-1]){
            cout << "open file failed" << endl;
            usage();
            exit(-1);
        }
        stream_file_table[&ifsv[i-1]] = argv[i];
    }
    // ifstream ifs(argv[1]);
    // if(!ifs){
    //     cout << "open file failed" << endl;
    //     usage();
    //     exit(-1);
    // }
    //长度不为10则不使用时间范围
    string StartDate = "2024 03 04";
    string EndDate = "2024 07 04";
    string line;
	map<string, int> checkin;
    map<string,int> checkinDis;
    map<string,string> CheckInInfos;
    // 谁，这周第几次打卡，是星期几
    map<string,pair<int,int>> weekCache;
    // 谁，今天第几次打卡
    // map<string,int> dayCache;
    size_t lineNum = 0;
    ofstream oflog("log.txt");
    int CleanFlag=true;
    // 注意定义
    for(auto & ifs:ifsv){
        getline(ifs,line);
        lineNum++;
        string filename = stream_file_table[&ifs];
        oflog<<"\n<<<<<<<<<<<<<<<<<<<<<<<< "<< filename <<" >>>>>>>>>>>>>>>>>>>>>>>>>\n";
        while(1){
            // 2023-09-11 22:46:21 一行
            // 打卡 1 张慧源 12km
            // 存在时间的为一条消息的开始，等到下一条消息开始时，统计上一条消息的打卡情况，中间可能出现很多行，但是都是一条消息
            if(isStartMsg(line)){
                //保存整条打卡信息（包括行号），用于调试
                string CheckInfo =line;
                CheckInfo = string("In file [")+ filename + string("] line [ ")+to_string(lineNum)+string(" ] ")+CheckInfo;

                string date = line.substr(0,10);
                string SName = line.substr(19);
                //周一清理缓存
                int WeekDay = WhichDayOfAWeek(atoi(date.substr(0,4).c_str()),atoi(date.substr(5,2).c_str()),atoi(date.substr(8,2).c_str()));
                // 并不是每个人到了星期一都要清除cache，这里出问题了
                if(CleanFlag&&(WeekDay==1)){
                    weekCache.clear();
                    CleanFlag = false;
                }
                else if(WeekDay!=1){
                    CleanFlag=1;
                }
                // 注意定义
                Mov

                while(!isStartMsg(line)){
                    size_t it ;
                    if(StartDate.size()==10&&EndDate.size()==10)
                        if(!DateBetween(date,StartDate,EndDate)){Mov;continue;}
                    if((it=line.find("打卡"))==0){
                        if(line.size()>sizeof("打卡 7 张慧源张慧源张慧源张慧源 100km")){Mov;continue;}
                        else if(line.size()<sizeof("打卡1张1")){Mov;continue;};
                        CheckInfo += string("\n正在处理: 打卡")+line.substr(it+sizeof("打卡")-1)+string("\n");
                        // cout<<"正在处理:"<<line.substr(it+sizeof("打卡")-1)<<endl;

                        istringstream iss(line.substr(it+sizeof("打卡")-1));
                        char tmp='0';
                        int TimesOfAWeek;
                        while(iss>>tmp){
                            if(tmp>='0'&&tmp<='9'||tmp!=' '){
                                break;
                            }
                        }
                        if(tmp<'1'||tmp>'7'){
                            CheckInfo += string("\t[error!]\t打卡次数不正确\n");
                            Mov;
                            continue;
                        }
                        TimesOfAWeek = tmp-'0';
                        string name ;
                        if(!(iss >> name)){Mov;continue;}
                        
                        float distance;
                        try{
                        distance = ReadAfloat(iss);
                        }
                        catch(const char* msg){
                            cout<<msg<<endl;
                            Mov;
                            continue;
                        }
                        if(distance<1.0||distance>100){Mov;continue;}

                        CheckInfo += string("文法处理结果: name: <")+name+string("> distance: <")+to_string(distance)+string(">\n");
                        /*                                  log                                             */
                        // cout<<"\n处理结果: "<<" name: <"<<name<<"> distance: <"<<distance<<">"<<endl;
                        /*                                  log                                             */
                        
                        int score = 0;
                        if(distance <= 5){
                            score = distance;
                        }
                        else if(score > 25){
                            score = 25;
                        }
                        else{
                            score = std::min(25,int(5 + (distance-5)*2));
                        }
                        // 跑步达到半程马拉松距离，额外加 10 分，达到全程马拉松距离，额
                        // 外加 20 分。
                        if(distance >= 21){
                            score += 10;
                        }
                        else if(distance >= 42){
                            score += 20;
                        }
                        if(!(weekCache[name].first) || (WeekDay != weekCache[name].second)){// 今天第一次打卡
                            weekCache[name].first++;
                            weekCache[name].second = WeekDay;
                            // 一天可以打卡多次，每次都可以积分。
                            // (3)每周打卡天数超过或等于四天的，额外加 10 分，七天每天打卡的 加 20 分。
                            
                            // CheckInfo += string("######################次数加一######################\n");

                            if(weekCache[name].first == 4){
                                score += 10;
                            }
                            else if(weekCache[name].first == 7){
                                score += 10;
                            }
                        }
                        // 毅行、爬山等非跑步活动打卡距离将以 0.5 倍等效于跑步打卡距离计算积分。
                        // TODO 
                        

                        checkin[name] += score;
                        checkinDis[name] += distance;
                        CheckInfo += string("语义处理结果:今天是周 <")+ to_string(WeekDay) + "> 打卡次数:<"+to_string(weekCache[name].first)+"> score: <"+to_string(score)+"> 当前总分: <"+to_string(checkin[name])+">"+" 当前总距离: <"+to_string(checkinDis[name])+">"+"\n";
                        CheckInInfos[name] += CheckInfo+"\n";
                        

                        oflog << CheckInfo;
                        cout<<CheckInfo<<endl;
                    }
                    Mov
                }
                
            }
            else{
                break;
            }
        }
        // out
    }
    ofstream ofs("Statistics.txt");
    vector<pair<int,string>>OutCheckin;
    for(auto &p : checkin){
        OutCheckin.push_back(make_pair(p.second,p.first));
    }
    sort(OutCheckin.begin(),OutCheckin.end(),[](pair<int,string>&a,pair<int,string>&b){return a.first>b.first;});//降序
    ofs<<"姓名\t积分\t距离"<<endl;
    for(auto &p : OutCheckin){
        ofs << p.second << "\t" << p.first <<"\t"<<checkinDis[p.second]<< endl;
    }
    ofs.close();
    oflog.close();
    MkDir("Log");
    for(auto & p:CheckInInfos){
        ofstream ofs("Log/"+p.first+".txt");
        ofs << p.second;
        ofs.close();
    }
    return 0;
}
/*
甲壳虫跑团打卡规则3.0
1、积分制 
规则详情： 
(1)一次跑步跑 5 公里以内每公里记 1 分，超过 5 公里，每公里记 2 
分。 
公式： 
S-距离(单位：km)、SCORE-积分数(单位：分) 
（1）S<=5，SCORE=S*1； 
（2）5<=S  SCORE=5+（S-5）*2; 
单次跑步积分最多为 25； 
跑步达到半程马拉松距离，额外加 10 分，达到全程马拉松距离，额 
外加 20 分。 
(2)参加跑团组织的活动（比如每周例跑）一次积分 10 分。采用实际签到的形式。 
(3)每周打卡天数超过或等于四天的，额外加 10 分，七天每天打卡的 
加 20 分。 
（4）向公众号投稿关于跑步文章的（跑步故事，经历，个人成长） 
等的，每篇加 15 分。 
（5）一天可以打卡多次，每次都可以积分。 
（6）毅行、爬山等非跑步活动打卡距离将以 0.5 倍等效于跑步打卡距离计算积分。 
声明：最终解释权归主办方所有，禁止一切作弊行为，如有发现，取 
消从今以后的参与评比获奖的资格。群里没有实名的不计算积分。
*/