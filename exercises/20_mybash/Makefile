# =============================================
# 主 Makefile
# 功能：
#   - mybash：生成二进制，放到 bin/
#   - 其他模块（mysed, myfile, mywc, mytrans）：生成静态库 .a，放到 obj/xxx/，.o 放到 obj/xxx/（或 out/，但建议统一）
#   - 所有子模块 Makefile 应支持 OBJDIR 和 BINDIR 参数
# =============================================

PWD := $(shell pwd)

# 所有子模块目录（静态库：生成 .a）
SUBDIRS := \
	mysed \
	myfile \
	mywc \
	mytrans

# 默认目标：构建 mybash（依赖其他库）
all: mybash

# 通用规则：构建每个子模块（静态库）
$(SUBDIRS):
	# TODO: 在这里添加你的代码
	# 🔥 flam: 每个子模块都递归下去产出各自的静态库到顶层 obj/ 下。
	@$(MAKE) -C src/$@ OBJDIR=$(PWD)/obj/$@

# 构建 mybash（可执行文件，依赖上面的库）
mybash: $(SUBDIRS)
	# TODO: 在这里添加你的代码
	@$(MAKE) -C src/mybash OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD) \
		LINKLIBS="-L$(PWD)/obj/mysed -L$(PWD)/obj/myfile -L$(PWD)/obj/mywc -L$(PWD)/obj/mytrans"

# 清理所有生成的文件
clean:
	# TODO: 在这里添加你的代码
	@$(MAKE) -C src/mysed clean OBJDIR=$(PWD)/obj/mysed
	@$(MAKE) -C src/myfile clean OBJDIR=$(PWD)/obj/myfile
	@$(MAKE) -C src/mywc clean OBJDIR=$(PWD)/obj/mywc
	@$(MAKE) -C src/mytrans clean OBJDIR=$(PWD)/obj/mytrans
	@$(MAKE) -C src/mybash clean OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD)
	@rm -rf $(PWD)/obj

.PHONY: all $(SUBDIRS) mybash clean
