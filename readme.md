# rojcpp

使用的c++编写的OnlineJudge系统

## 安装

安装依赖的包

```
sudo apt-get install libhiredis-dev
git clone https://github.com/0xsky/xredis
cd xredis
# 可能要修改 makefile里的 ->INCLUDE=/usr/include/hiredis
make
sudo make install
```

## 感谢

- [开发OJ之数据库表设计 | JanBox的小站](https://boxjan.com/2019/04/dev-oj-database-table-design.html)
- [0xsky/xredis: Redis C++ client, support the data slice storage, support redis cluster, thread-safe,multi-platform,connection pool, read/write separation.](https://github.com/0xsky/xredis)
