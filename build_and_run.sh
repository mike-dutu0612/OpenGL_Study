#!/bin/bash

# 设置颜色输出（增强交互体验）
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # 重置颜色

# 项目根目录（脚本所在目录）
PROJECT_ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="${PROJECT_ROOT}/build"
BIN_DIR="${PROJECT_ROOT}/bin"

# 步骤1：创建必要目录
echo -e "${YELLOW}[1/4] 初始化目录结构...${NC}"
mkdir -p "${BUILD_DIR}"
mkdir -p "${BIN_DIR}"

# 步骤2：执行 CMake 配置
echo -e "${YELLOW}[2/4] 执行 CMake 配置...${NC}"
cd "${BUILD_DIR}" || {
    echo -e "${RED}错误：无法进入构建目录 ${BUILD_DIR}${NC}"
    exit 1
}
cmake ..
if [ $? -ne 0 ]; then
    echo -e "${RED}错误：CMake 配置失败${NC}"
    exit 1
fi

# 步骤3：编译项目
echo -e "${YELLOW}[3/4] 编译项目（多核加速）...${NC}"
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo -e "${RED}错误：编译失败${NC}"
    exit 1
fi

# 步骤4：收集并列出可执行文件
echo -e "${YELLOW}[4/4] 收集可执行文件...${NC}"
cd "${BIN_DIR}" || {
    echo -e "${RED}错误：无法进入可执行文件目录 ${BIN_DIR}${NC}"
    exit 1
}

# 筛选可执行文件（排除目录和非可执行文件）
EXEC_FILES=()
for file in *; do
    if [ -f "$file" ] && [ -x "$file" ]; then
        EXEC_FILES+=("$file")
    fi
done

# 检查是否有可执行文件
if [ ${#EXEC_FILES[@]} -eq 0 ]; then
    echo -e "${YELLOW}提示：未找到可执行文件，请先在 src 目录下创建 .c 文件${NC}"
    exit 0
fi

# 列出可执行文件（带序号）
echo -e "\n${GREEN}========== 可执行文件列表 ==========${NC}"
for i in "${!EXEC_FILES[@]}"; do
    echo "[$((i+1))] ${EXEC_FILES[$i]}"
done
echo -e "${GREEN}====================================${NC}"

# 步骤5：交互选择并运行
while true; do
    echo -e "\n请输入要运行的文件序号（输入 q 退出）："
    read -r CHOICE

    # 退出逻辑
    if [ "$CHOICE" = "q" ] || [ "$CHOICE" = "Q" ]; then
        echo -e "${YELLOW}退出脚本...${NC}"
        exit 0
    fi

    # 验证输入是否为数字
    if ! [[ "$CHOICE" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}错误：请输入有效的数字序号！${NC}"
        continue
    fi

    # 验证序号是否在范围内
    INDEX=$((CHOICE-1))
    if [ "$INDEX" -lt 0 ] || [ "$INDEX" -ge ${#EXEC_FILES[@]} ]; then
        echo -e "${RED}错误：序号超出范围！请输入 1-${#EXEC_FILES[@]} 之间的数字${NC}"
        continue
    fi

    # 运行选中的可执行文件
    SELECTED_EXEC="${EXEC_FILES[$INDEX]}"
    echo -e "\n${GREEN}正在运行：${SELECTED_EXEC}${NC}"
    echo -e "${GREEN}------------------------------------${NC}"
    ./"${SELECTED_EXEC}"
    echo -e "${GREEN}------------------------------------${NC}"
    echo -e "${GREEN}程序 ${SELECTED_EXEC} 运行结束，退出码：$?${NC}"
    
    # 询问是否继续运行其他程序
    echo -e "\n是否继续运行其他程序？(y/n)："
    read -r CONTINUE
    if [ "$CONTINUE" != "y" ] && [ "$CONTINUE" != "Y" ]; then
        echo -e "${YELLOW}退出脚本...${NC}"
        exit 0
    fi
done