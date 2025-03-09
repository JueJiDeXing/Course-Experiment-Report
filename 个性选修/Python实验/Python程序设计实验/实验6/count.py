import re
from collections import Counter

# 读取文本文件
with open("article.txt", "r") as file:
    text = file.read()

# 使用正则表达式提取单词
words = re.findall(r'\b\w+\b', text.lower())

# 计算单词出现的频率
word_count = Counter(words)

# 获取出现频率排名前10的单词
top_10_words = word_count.most_common(10)

# 将结果写入词频文件
with open("word_frequency.txt", "w") as file:
    for word, count in top_10_words:
        file.write(f"{word}: {count}\n")
