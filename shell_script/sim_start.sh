#!/bin/bash

# 设置可执行文件名
EXECUTABLE="./learnOGL"

CURRENT_DIR=$(pwd)

echo "Current path is: $CURRENT_DIR"
cd "$CURRENT_DIR/../build"

function Start {
    # 检查可执行文件是否存在
    if [ -f "$EXECUTABLE" ]; then
        # 运行可执行文件
        $EXECUTABLE
    else
        echo "Error: $EXECUTABLE 不存在或不可执行."
        exit 1
    fi
}


case $1 in
*)
    Start
;;
esac