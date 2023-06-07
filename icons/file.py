import os
import glob

# 获取当前文件夹下的所有 PNG 文件
png_files = glob.glob("*.png")

# 遍历每个 PNG 文件并输出为 <file>align-center.png</file> 的形式
for png_file in png_files:
    file_name = f"<file>{png_file}</file>"
    print(file_name)
