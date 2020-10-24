# MyDB

《数据库系统概论》课程项目，实现一个单用户数据库，分为了四个模块——记录管理模块、索引管理模块、系统管理模块以及查询解析模块。

## 记录管理模块

注意同一个文件中记录是定长的。

### 文件存储

* 文件的第一页存储: 每个记录的字节数 recSize、每一页存储的记录个数 recNumPerPage、总记录个数 recNumTot(均为 int 型)，以及
* 每个数据页存储： 开头消耗 recNumPerPage 位表示每个记录槽是否存放记录，接下来共 recNumPerPage 个记录槽，每个记录槽消耗 recSize 个字节；

### RID

RID 为 (pageNum, slotNum) 二元组。
