# 1.说明
  借助该程序可以实现自定义shell命令，方便和应用程序交互。  
  server中目前已包含标准输入输出的重定向功能。
   
# 2.用法
  在代码目录下执行make，会在当前目录生成两个可执行文件server和client。  
  在当前终端执行server，在另一个终端（比如telnet出来的终端）执行client redirect_print, 可以把server的输出重定向到client所在的终端。
