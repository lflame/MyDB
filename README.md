# MyDB

《数据库系统概论》课程项目，实现一个单用户数据库，分为了辅助模块(Logger 模块、Tests 模块)和四个主模块——记录管理模块、索引管理模块、系统管理模块以及查询解析模块。

## 框架与结构设计

本项目采用模块化设计，使用 cmake 进行编译。

环境为：Ubuntu 18.04, cmake 3.18.4, make 4.1, g++ 7.5.0。

目录格式如下：

```
README.md
docs/
code/
  CMakeLists.txt
  build.sh
  run.sh
  modules/
	tests/
	  CmakeLists.txt
	  include/
	  src/
    filesystem/
	  CMakeLists.txt
	  include/
	  src/
	logger/
	  CMakeLists.txt
	  include/
	  src/
	recordmanager/
	  CMakeLists.txt
	  include/
	  src/
  src/
    main.cpp
```

使用 `bash run.sh` 将会首先使用 cmake 进行构建（注意暂时限定了版本为 3.18.4 及其以上），并且运行 main.cpp 进行测试。

## Tests 模块

用于进行单元测试，直接调用 `testAll()` 即可进行所有测试。

## Logger 模块

用于记录 log，分为五个级别 —— FATAL, ERROR, WARNING, INFO, DEBUG，分别对应 1~5。

Feature:

* 打印时会打印出当前本地日期和时间，以及 log 级别作为标签；
* 打印不同级别的 log 时会显示不同的颜色。

TODO: 存储到 log 文件中。

## 记录管理模块

注意同一个文件中记录是定长的。

### 设计思路

#### 文件存储

* 文件的第一页存储: 每个记录的字节数 recSize、每一页存储的记录最大个数 recNumPerPage、总记录个数 recNumTot、总页数 pageNum，以及非满页链表表头对应的页数 usablePageHeader(均为 int 型，页数从 0 开始计数)，另外链表指针值为 -1 时表示空指针。
* 每个数据页存储： 开头存储三个 int，分别为 prevUsablePage 和 nextUsablePage，以及 usableSlotNum 表示还剩余多少个空余槽, 接着使用 recNumPerPage 位表示每个记录槽是否存放记录(1 表空闲, 0 表占用)，接下来 8 位对齐之后共 recNumPerPage 个记录槽，每个记录槽消耗 recSize 个字节。注意需要满足 `12 + (recNumPerPage+7)/8*8 + recSize*recNumPerPage <= PAGE_SIZE`

为提高鲁棒性，在存储 int 时统一使用网络字节序。

#### RID 和 Record

* RID 为 (pageNum, slotNum) 二元组，表示该记录所在的页和槽(从 0 开始计数)；
* Record 存储了一个 char\* 指针指向记录数据。

### 代码实现

#### Buffer 读写

为了方便对 buffer 进行二进制读写，书写了类 BufferReader 和 BufferWriter，对二进制长数据、int 型数据、float 型数据的读写都进行了处理，其中 int 型数据在存储时以网络字节序存储。

#### RecordManager 类

实现了类 RecordManager，并且在其中实现了 `_readIntFromPage/_writeIntToPage` 和 `_readDataFromPage/_writeDataToPage` 用于对于页的较底层的读写(将调用 BufferReader/BufferWriter 的静态函数实现)，以及其它对页中存储的参数进行直接操作的较高层的读写，例如 `_readFileHeaderPage(FileHeaderPageParameterType type)` 和 `_readDataPagePointer(int pageId, bool isPrev, int &pointer);` 等。

在读写操作的基础上，只需要按照上面设计思路进行设计即可，唯一需要注意的就是链表的维护。
