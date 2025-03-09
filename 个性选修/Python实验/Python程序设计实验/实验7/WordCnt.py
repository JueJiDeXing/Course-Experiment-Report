from jieba import posseg
from collections import Counter
from wordcloud import WordCloud
import matplotlib.pyplot as plt


def countName(words):
    # 筛选出人名
    names = [word for word, flag in words if flag == 'nr']
    name_count = Counter(names)
    # 输出前10个出现次数最高的人名
    print("前10个出现次数最高的人名:")
    for name, count in name_count.most_common(10):
        print(f"{name}: {count}")


def wordCloud(words):
    # 统计所有词的出现次数
    all_words = [word for word, flag in words]
    word_count = Counter(all_words)
    word_count.pop('\n')
    # 创建WordCloud对象
    wordcloud = WordCloud(width=800, height=400, background_color='white', max_words=100, font_path='simhei.ttf',
                          prefer_horizontal=0.9, min_font_size=10, max_font_size=100)
    # 根据Counter对象生成词云
    wordcloud.generate_from_frequencies(word_count)
    # 设置词云参数
    wordcloud.to_file('wordcloud.png')
    # 展示词云图
    plt.imshow(wordcloud)
    plt.axis('off')
    plt.show()


# 读取文本文件
with open("大奉打更人.txt", "r", encoding="utf-8") as f:
    text = f.read()

# 使用jieba库对文本进行分词
words = posseg.cut(text)
words = [(word, flag) for word, flag in words if flag is not None]
countName(words)
wordCloud(words)
