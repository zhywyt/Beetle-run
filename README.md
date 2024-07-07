# Beetle-run
甲壳虫跑团相关代码/资料

本项目微信聊天记录导出使用了ChatMsg进行导出。


### 不处理格式
#### 丢失打卡次数
不处理原因：无法进行分词处理
```
打卡 张慧源 1k
```
和另一种文法冲突了
```
打卡1 张慧源 1k
```
这两种句子无法进行彻底的分词，第一种甚至会造成我的死循环。

#### 非打卡语句
此语言认定为娱乐语句，不参与打卡
```
有氧卡 1 张慧源 1k
```

#### 中文数字
```
打卡一 林卓赫 10km
```

#### 姓名与距离连接
```
1张慧源5.24km
```
#### 距离使用中文
这些均不接受
```
打卡 1 张慧源 半马
打卡 2 张慧源 十公里
打卡 3 张慧源 全马
```

### 关于数据导出
需要替换`-` 为 ` `。

