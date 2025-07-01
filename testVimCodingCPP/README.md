# VIM配置为好用版本 C++提示

## 插件配置

本VIM配置使用了以下插件来增强C++开发体验：

### 核心插件
- **vim-cpp-enhanced-highlight**: C++语法高亮增强
- **nerdtree**: 文件树浏览器
- **lightline.vim**: 美观的状态栏
- **coc.nvim**: 代码补全与智能提示（基于Language Server Protocol）
- **delimitMate**: 括号/引号自动补全
- **vim-autoformat**: 代码格式化
- **nerdcommenter**: 智能注释插件

## 基础设置

```vim
syntax on                    " 语法高亮
set number                  " 显示行号
set relativenumber          " 显示相对行号
set tabstop=4              " Tab键宽度为4个空格
set shiftwidth=4           " 缩进宽度为4个空格
set expandtab              " 将Tab转换为空格
set autoindent             " 自动缩进
set smartindent            " 智能缩进
set cursorline             " 高亮当前行
set background=dark        " 深色背景
colorscheme desert         " 使用desert配色方案
```

## 快捷键配置

### 文件管理
- `<C-n>`: 切换NERDTree文件树

### 代码补全
- `<TAB>`: 在补全菜单中向下选择
- `<S-TAB>`: 在补全菜单中向上选择

### 代码格式化
- `<F3>`: 一键格式化当前文件
- 保存时自动格式化C++文件（.cpp, .h）

### 编译与调试
- `<F5>`: 编译并运行C++程序
- `<F7>`: 仅编译C++程序
- `<C-F5>`: 编译并启动GDB调试

### 注释
- 使用NERDCommenter的默认快捷键进行注释操作

## 自动功能

1. **自动格式化**: 保存C++文件时自动调用clangd进行格式化
2. **智能补全**: 基于coc.nvim提供智能代码补全
3. **语法检查**: 实时语法错误检查
4. **自动缩进**: 智能代码缩进

## 安装说明

1. 确保已安装Vim-Plug插件管理器
2. 将.vimrc文件复制到用户主目录
3. 在Vim中运行`:PlugInstall`安装所有插件
4. 重启Vim即可使用

## 依赖要求

- Vim 8.0+ 或 Neovim
- Node.js (用于coc.nvim)
- g++编译器
- gdb调试器
- astyle代码格式化工具（可选）

## 特色功能

- **一键编译运行**: F5键快速编译并运行C++程序
- **智能补全**: 基于LSP的代码补全和错误检查
- **美观界面**: 现代化的状态栏和配色方案
- **高效编辑**: 文件树、注释、格式化等功能的快捷键支持

