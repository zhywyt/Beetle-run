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


using std::cin,std::cout,std::ifstream,std::ofstream,std::endl,std::string,std::map,std::vector,std::to_string;
using std::istringstream,std::istream,std::pair;



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
    cout << "usage: ./Statistics /path/to/WeChatMsg/ChatExport.txt" << endl;
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
    int week = (day + 2*mouth + 3*(mouth+1)/5 + year + year/4 - year/100 + year/400) % 7;
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
bool DateBetween(string date, string start,string end){
    if(date.size() != 10 || start.size() != 10 || end.size() != 10){
        return false;
    }
    int year = atoi(date.substr(0,4).c_str());
    int mouth = atoi(date.substr(5,2).c_str());
    int day = atoi(date.substr(8,2).c_str());
    int startYear = atoi(start.substr(0,4).c_str());
    int startMouth = atoi(start.substr(5,2).c_str());
    int startDay = atoi(start.substr(8,2).c_str());
    int endYear = atoi(end.substr(0,4).c_str());
    int endMouth = atoi(end.substr(5,2).c_str());
    int endDay = atoi(end.substr(8,2).c_str());
    if(year > startYear && year < endYear){
        return true;
    }
    if(year == startYear && year == endYear){
        if(mouth > startMouth && mouth < endMouth){
            return true;
        }
        if(mouth == startMouth && mouth == endMouth){
            if(day >= startDay && day <= endDay){
                return true;
            }
        }
        if(mouth == startMouth){
            if(day >= startDay){
                return true;
            }
        }
        if(mouth == endMouth){
            if(day <= endDay){
                return true;
            }
        }
    }
    if(year == startYear){
        if(mouth > startMouth){
            return true;
        }
        if(mouth == startMouth){
            if(day >= startDay){
                return true;
            }
        }
    }
    if(year == endYear){
        if(mouth < endMouth){
            return true;
        }
        if(mouth == endMouth){
            if(day <= endDay){
                return true;
            }
        }
    }
    return false;
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
#define Mov if(getline(ifs,line)){lineNum++;cout<<CheckInfo<<endl;}else{break;}
int main(int argc, char *argv[]){
    if(argc != 2){
        usage();
        exit(-1);
    }
    ifstream ifs(argv[1]);
    if(!ifs){
        cout << "open file failed" << endl;
        usage();
        exit(-1);
    }
    string StartDate = "2024 01 01";
    string EndDate = "2024 07 04";
    string line;
	map<string, int> checkin;
    // 谁，这周第几次打卡，是星期几
    map<string,pair<int,int>> weekCache;
    // 谁，今天第几次打卡
    // map<string,int> dayCache;
    size_t lineNum = 0;

    // 注意定义
    getline(ifs,line);
    lineNum++;
	while(1){
        // 2023-09-11 22:46:21 一行
        // 打卡 1 张慧源 12km
        // 存在时间的为一条消息的开始，等到下一条消息开始时，统计上一条消息的打卡情况，中间可能出现很多行，但是都是一条消息
        if(isStartMsg(line)){
            //保存整条打卡信息（包括行号），用于调试
            string CheckInfo =line;
            CheckInfo = string("[ ")+to_string(lineNum)+string(" ] ")+CheckInfo;

            string date = line.substr(0,10);
            string SName = line.substr(19);
            //周一清理缓存
            int WeekDay = WhichDayOfAWeek(atoi(date.substr(0,4).c_str()),atoi(date.substr(5,2).c_str()),atoi(date.substr(8,2).c_str()));
            if(WeekDay == 0){
                weekCache.clear();
            }
            // 注意定义
            Mov

            while(!isStartMsg(line)){
                size_t it ;
                if(!DateBetween(date,StartDate,EndDate)){Mov;continue;}
                if((it=line.find("打卡"))==0){
                    if(line.size()>sizeof("打卡 7 张慧源张慧源张慧源张慧源 100km")){Mov;continue;}
                    else if(line.size()<sizeof("打卡1张1")){Mov;continue;};
                    cout<<"正在处理:"<<line.substr(it+sizeof("打卡")-1)<<endl;
                    istringstream iss(line.substr(it+sizeof("打卡")-1));
                    char tmp='0';
                    int TimesOfAWeek;
                    while(iss>>tmp){
                        if(tmp>='0'&&tmp<='9'||tmp!=' '){
                            break;
                        }
                    }
                    if(tmp<'1'||tmp>'7'){
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
                    if(distance<1.0){Mov;continue;}


                    /*                                  log                                             */
                    cout<<"\n处理结果: "<<" name: <"<<name<<"> distance: <"<<distance<<">"<<endl;
                    /*                                  log                                             */
                    
                    int score = 0;
                    if(distance <= 5){
                        score = distance;
                    }else{
                        score = 5 + (distance-5)*2;
                    }
                    if(score > 25){
                        score = 25;
                    }
                    // 跑步达到半程马拉松距离，额外加 10 分，达到全程马拉松距离，额
                    // 外加 20 分。
                    if(distance >= 21.0975){
                        score += 10;
                    }
                    if(distance >= 42.195){
                        score += 20;
                    }
                    // 一天可以打卡多次，每次都可以积分。每天可以多打，但是只能算一周的一天打卡了
                    checkin[name] += score;
                    if(WeekDay != weekCache[name].second){// 今天第一次打卡
                        weekCache[name].first++;
                        weekCache[name].second = WeekDay;
                    }
                    // 毅行、爬山等非跑步活动打卡距离将以 0.5 倍等效于跑步打卡距离计算积分。
                    // 一天可以打卡多次，每次都可以积分。
                    // (3)每周打卡天数超过或等于四天的，额外加 10 分，七天每天打卡的 加 20 分。
                    if(weekCache[name].first == 4){
                        checkin[name] += 10;
                    }
                    if(weekCache[name].first == 7){
                        checkin[name] += 20;
                    }
                }
                Mov
            }
            
        }
        else{
            break;
        }

    }
    // out
    ofstream ofs("Statistics.txt");
    for(auto &p : checkin){
        ofs << p.first << " " << p.second << endl;
    }
    ofs.close();
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