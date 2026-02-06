# Library System Development Log  
# 图书管理系统开发日志

---

## 📅 2026-02-04  
### 项目初始化 / Project Initialization

**完成内容：**
- 定义 `Book` 结构体（图书基本信息）
- 使用 `vector<Book>` 作为图书容器
- 初始化测试数据（TestData）

**Details:**
- Book ID
- Book Title
- Book Classification
- Borrow status (`isBorrowed`)

---

## 📅 2026-02-05  
### 图书管理模块 / Book Management Module

**完成内容：**
- 添加图书功能（Add Book）
- 查看图书列表功能（View Books）
- 图书管理菜单（Book Management UI）

**功能说明：**
- 支持用户手动输入图书信息
- 显示所有图书的当前状态（Available / Borrowed）

---

## 📅 2026-02-06  
### 借阅与归还功能 / Borrow & Return Functionality

**完成内容：**
- 借书功能（Borrow Book）
- 还书功能（Return Book）
- 借阅状态校验

**逻辑说明：**
- 已借出的书不可再次借阅
- 未借出的书不可归还
- 所有操作基于 Book ID 查找

---

## 📅 2026-02-07  
### 借阅历史记录 / Borrow History (Linked List)

**完成内容：**
- 定义 `HistoryNode` 结构体
- 实现 `HistoryList` 链表
- 记录借书 / 还书操作
- 显示借阅历史记录

**数据结构：**
- 使用单向链表（Singly Linked List）
- 每条记录包含：
  - 操作类型（BORROW / RETURN）
  - Book ID
  - Book Title

---

## 📌 当前已完成功能总结  
## Current Implemented Features

- 图书数据管理（Add / View）
- 借书与还书流程
- 借阅状态控制
- 操作历史记录（Linked List）
- 菜单驱动的控制台 UI

---

## 🚧 尚未实现（可后续扩展）  
## Not Implemented Yet

- 借阅等待队列（QueueNode 尚未使用）
- 用户系统（User Management）
- 数据持久化（文件 / 数据库）
- 删除或修改图书功能

---

## 📎 说明 / Notes

- 本项目为 **C++ 数据结构练习项目**
- 重点使用：
  - `vector`
  - `struct`
  - `linked list`
  - 控制台菜单逻辑

---
