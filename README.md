# MyDB

《数据库系统概论》课程项目，实现一个单用户数据库，分为了辅助模块(Logger 模块)和四个主模块——记录管理模块、索引管理模块、系统管理模块以及查询解析模块。

## Logger 模块

用于记录 log，分为五个级别 —— FATAL, ERROR, WARNING, INFO, DEBUG，分别对应 1~5。

Feature:

* 打印时会打印出当前本地日期和时间，以及 log 级别作为标签；
* 打印不同级别的 log 时会显示不同的颜色。

TODO: 存储到记录文件中。

## 记录管理模块

注意同一个文件中记录是定长的。

### 文件存储

* 文件的第一页存储: 每个记录的字节数 recSize、每一页存储的记录最大个数 recNumPerPage、总记录个数 recNumTot、总页数 pageNum，以及非满页链表表头对应的页数 usablePageHeader(均为 int 型，页数从 0 开始计数)，另外链表指针值为 -1 时表示空指针。
* 每个数据页存储： 开头存储三个 int，分别为 prevUsablePage 和 nextUsablePage，以及 usableSlotNum 表示还剩余多少个空余槽, 接着使用 recNumPerPage 位表示每个记录槽是否存放记录(1 表空闲, 0 表占用)，接下来 8 位对齐之后共 recNumPerPage 个记录槽，每个记录槽消耗 recSize 个字节。注意需要满足 `12 + (recNumPerPage+7)/8*8 + recSize*recNumPerPage <= PAGE_SIZE`


为提高鲁棒性，在存储 int 时统一使用网络字节序。

### RID 和 Record

* RID 为 (pageNum, slotNum) 二元组，表示该记录所在的页和槽(从 0 开始计数)；
* Record 存储了一个 char\* 指针指向记录数据。
