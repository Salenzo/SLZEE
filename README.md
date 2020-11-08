SLZEE 1
=======

这是SLZEE的初代版本。没有什么文档，代码写得也相当糟糕，于此仅作纪念。

使用
----

首先，建议不要用。

输入应保存为UTF-8编码的文本文件a.txt。

输出是b.html。它链接同目录下的a.png。

### 公式语法 ###

- 半角字母为斜体，全角字母为正体。
- 空格插入窄间距。没有自动间距调整，所有间距皆靠手动输入。
- 最外层的换行符开始新行，子式内居中对齐的换行可借用列向量。
- 诸如×、≤、∫、π等符号可直接输入。
- 减号输入为连字符暨减号（-），点乘号输入为星号（\*）。
- 多重积分号以**重∫**（二重）、**重重∫**（三重）的形式输入。
- **地**、**板**、**天**、**花**分别对应⌊、⌋、⌈、⌉。
- 三种括号（()、[]、{}）总是成对出现，并总是自动伸展到包括内部公式。
- **左 ~ 右**以竖线包围内部公式。
- **√ ~ 出**插入二次根式。没有<var>n</var>次根式。
- **上 ~ 下 ~ 中**插入上下标。
- **分 ~ 于 ~ 合**插入分数。
- 列向量以**张一 ~ 一 ~ 一 ~ 外**的形式输入，矩阵以**张三 ~ 一 ~ 一 ~ 一 ~ 一 ~ 一 ~ 外**（三列）的形式输入。

缘起
----

2020年春季学期，受新型冠状病毒疫情影响，线下上课被网上直播取代，作业亦通过网络下发与上交。我决定每周使用不同的排版和公式编辑程序的组合。不到三个月后，我穷尽了我熟悉的软件，于是我制作了SLZEE。下面是这一学期里我使用过的用来写数学作业的程序组合。

1. Microsoft Word 2016、其内置的UnicodeMath实现、Cambria Math
2. LyX、XeLaTeX、Latin Modern
3. Microsoft Office Word 2003、MathType 6、Times New Roman和MT Extra等
4. Wolfram Mathematica 8.0
5. Visual Studio Code插件Markdown All in One、KaTeX
6. MATLAB R2019b
7. Vim、XeLaTeX、STIX Two
8. Microsoft Office FrontPage 2003、MathJax
9. LibreOffice 5 Writer、LibreOffice 5 Math、Liberation Serif
10. Visual Studio Code、**SLZEE 1**
11. Microsoft PowerPoint 2016、其内置的UnicodeMath实现、Cambria Math
12. Microsoft Excel 2016、其内置的UnicodeMath实现、Cambria Math
13. Microsoft Works 9、MathType 6、Times New Roman和MT Extra等
14. 写字板6.1（内部版本7601）、MathType 6、Times New Roman和MT Extra等
