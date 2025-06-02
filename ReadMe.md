# mini-sheel

mini-sheel 是一个轻量级的嵌入式命令行 shell，专为资源受限的嵌入式系统设计。它支持命令注册、参数解析、历史记录、自动补全等功能，方便开发者在嵌入式设备上进行调试和交互。

## 主要特性

- 命令注册与管理
- 命令参数解析
- 命令历史记录（支持上下键浏览）
- TAB 自动补全命令
- 内置 echo、help、hexdump 等命令
- 支持自定义命令扩展

## 目录结构

```
.
├── mini_shell.c    // mini-shell 主体实现
├── sheel_port.c    // mini-shell 与底层串口/设备的适配
├── xformat.c       // 格式化输出实现（类似 printf）
├── xstring.c       // 字符串与内存操作实现
├── SConscript      // 构建脚本
├── ReadMe.md       // 项目说明
└── .gitignore      // Git 忽略文件
```

## 快速开始

1. **集成到工程**

   将本项目源码文件添加到你的嵌入式工程，并确保 `mini_shell.h`、`xformat.h`、`xstring.h` 等头文件可被包含。

2. **初始化 shell**

   在主函数或初始化代码中调用 `sheel_port_init()`，或根据实际硬件适配输入输出函数后调用 `shell_init()`：

   ```c
   shell_init(shell_output_char, dev, shell_input_char, dev, sheel_headtag);
   ```

3. **注册自定义命令**

   实现 `struct shell_command` 并调用 `shell_register_command` 注册：

   ```c
   static int mycmd(int argc, char **argv) {
       // 命令实现
       return 0;
   }
   static struct shell_command mycmd_cmd = {
       .name = "mycmd",
       .desc = "my custom command",
       .func = mycmd,
       .next = 0,
   };
   // 注册
   shell_register_command(&mycmd_cmd);
   ```

4. **运行 shell 服务**

   在循环或线程中调用 `shell_servise()`，即可处理输入输出。

## 内置命令

- `help`      显示所有已注册命令
- `echo`      回显参数
- `hexdump`   以十六进制格式显示内存内容

## 依赖

- C99 标准
- 若用于 RT-Thread，需依赖 RT-Thread 设备驱动接口

## 使用现象

```c
        _      _        _            _
  _ __ (_)_ _ (_)___ __| |_  ___ ___| |
 | '  \| | ' \| |___(_-< ' \/ -_) -_) |
 |_|_|_|_|_||_|_|   /__/_||_\___\___|_|
            mini-shell v1.0
mns/>shell commads:
help         : show help
echo         : show parameters
hexdump      : show mem info
mns/>echo nihao
nihao
mns/>h
help
hexdump

mns/>he
help
hexdump

mns/>help
shell commads:
help         : show help
echo         : show parameters
hexdump      : show mem info
mns/>nihao
Unknown command: nihao
mns/>nihao
```

