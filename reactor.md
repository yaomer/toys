## reactor
### 什么是reactor模式？
网络编程中有很多是事务性的工作，可以提取为公用的框架或库，而用户只需要填上关键的业务逻辑代码，并将回调注册到框架中，就可以实现完整的网络服务，这正是reactor模式的主要思想。

### 主要逻辑
reactor模式的核心就是事件驱动。通常体现为一个事件循环，阻塞在select/poll/epoll上，等待事件发生，然后触发相应的回调。

### 线程模型
+ 单线程reactor
+ 单线程reactor + thread pool
+ 多线程reactor
+ 多线程reactor + thread pool
+ 多进程reactor

### I/O模型
默认是同步的，不牵扯异步。

使用BIO时，读写单个client阻塞，可能造成整个io线程阻塞。因而要使用NIO。

### Buffer
网络编程中，服务器要做的可以简单理解为收到一条消息，处理过后，再发送回去。

如果没有Buffer，当我们读到的数据不能构成一条消息时怎么办？我们发送数据时没有发送完怎么办(TCP的发送缓冲区满了)？

### TCP分包
+ 添加长度字段
+ 固定分隔符，如\r\n
+ 固定的长度的消息

### 非阻塞accept和connect
如何将一个fd设置为非阻塞呢？

非阻塞accept通常要忽略以下几个errno, EPROTO, ECONNABORTED, EINTR, EWOULDBLOCK

非阻塞connect通常需要retry，利用select设置超时

### 线程安全的[event loop]
使用[task queue]，其他线程通过它向io线程(loop所在的线程)添加任务。

### 多线程异步logger
这是一个很典型的多生产者-单消费者问题，为了不让logger影响到程序的正常逻辑，我们通常在单独的线程中执行文件I/O。即其他线程写到一个Buffer中，然后立即返回，避免阻塞其他线程，日志线程在合适的时机将数据写到本地文件中。

### timer

|数据结构|插入|删除任一元素|删除最小元素|访问最小元素|
|---|---|---|---|---|
|普通链表|O(n)|O(n)|O(n)|O(n)|
|排序链表|O(n)|O(n)|O(1)|O(1)|
|堆|O(logn)|nO(logn)|O(logn)|O(1)|
|平衡树|O(logn)|O(logn)|O(logn)|O(logn)|
|时间轮|O(1)|O(1)|O(1)|O(1)|

通常需要提供三个接口：
+ 在将来的某个时间执行task
+ timeout时间后执行task
+ 每间隔timeout时间执行一次task

### timer的实现
+ SIGALRM，不精准，而且多线程一般避免和signal打交道
+ I/O Multiplexing的timeout参数
	+ 需要不断更新timeout，所以我们需要存储绝对时间，即timeout + nowtime()(自1970年到现在的ms数)，获取超时值时在timeout - nowtime()。

### 如何处理空闲连接
TCP keepalive 2 hours

+ 每个连接设定一个timer，timer超时后踢掉连接
+ 使用简单的时间轮
