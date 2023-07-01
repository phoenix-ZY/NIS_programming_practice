# 定义编译器和编译选项
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror

# 定义目标文件和依赖关系
TARGET := myprogram
SRCS := main.cpp login.cpp tasks.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

# 默认构建目标
all: $(TARGET)

# 生成目标文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# 生成依赖关系文件
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MF $@

# 包含依赖关系文件
-include $(DEPS)

# 清除中间文件
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

# 定义伪目标
.PHONY: all clean