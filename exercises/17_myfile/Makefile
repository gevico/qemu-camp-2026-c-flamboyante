CC = gcc
CFLAGS = -Wall -g -O2 -fPIE

TARGET := 17_myfile
SRCS := 17_myfile.c
OBJS := $(SRCS:.c=.o)

all: $(OBJS) $(TARGET)

# 先生成目标文件，程序运行时会读取 17_myfile.o 来识别 ELF 类型
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 保留独立的 .o 构建规则，确保 checker 执行时 .o 文件实际存在
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
