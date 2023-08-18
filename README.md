# # SimpleModifier

一个为了理解进程内存操作而编写的玩具级别修改器。



## 开发环境

- 语言: c++

- 开发平台: windows11

- 编译器: gcc.exe (GCC) 11.2.0

## 命令

- `monitor program`
  
  提供程序名称，修饰器将监控相应的程序进程。

- `requerry value`
  
  在执行了 `monitor` 并绑定了进程之后，你可以使用 `requerry` 在整个进程内存中查询一个数值，并将结果保存供下一次查询使用。

- `qerry value`
  
  该命令可用于在执行完 `requerry` 后对结果进行筛选。无效数据将从结果中删除。

- `modify value`
  
  如字面意思所示，该命令将根据 `value` 参数修改之前由 `requerry` 和 `qerry` 命令筛选出的所有结果的值。

## 如何编译

单文件c++程序,理论上在支持windows api的c++编译器上都能编译





# SimpleModifier

A toy-level modifier designed for understanding process memory exercises.



## Development Environment

- Language: C++

- Development Platform: Windows 11

- Compiler: gcc.exe (GCC) 11.2.0

## Commands

- `monitor program`
  
  Provide a program name, and the modifier will monitor the corresponding process of the program.

- `requerry value`
  
  After executing `monitor` and binding a process, you can use `requerry` to query a value in the entire process memory and save the results for the next query.

- `qerry value`
  
  This command can be used to filter results after executing a `requerry`. Invalid data will be removed from the results list.

- `modify value`
  
  As the name implies, this command will modify the values of all results previously filtered by the `requerry` and `qerry` commands, according to the `value` parameter.

## How to compile

A single-file C++ program should, in theory, be compilable on any C++ compiler that supports the Windows API.
