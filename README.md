# 中英双文拼写检查器

启发、借鉴于http://norvig.com/spell-correct.html
中文译文：https://blog.csdn.net/bobten2008/article/details/5478516
如果对于具体实现感兴趣，建议先阅读上述材料。

兼容windows，macos，linux平台

具体实现
1.SpellingCorrector类
英文拼写检查器，主要做了对Peter Norvig提出的拼写检查器的c++重构。重构借鉴了https://github.com/felipefarinon/spellingcorrector，在此基础上做了性能上的优化，其他些许改造。

2.Pinyin类
来源于https://github.com/wzhe/GetPinyin，主要作用是识别中文，获取中文拼音。

3.SpellingCorrectorForZhCN类
中文拼写检查器，私有继承了SpellingCorrector类，主要思路：拼音 --> 中文检查器 --> 英文检查器 --> 正确的拼音 --> 中文检查器 --> 中文。在原贝叶斯算法基础上，又套了一层贝叶斯————误差模型：拼音长度越长的优先；语言模型：中文频率；编辑距离：插入的优先级最高，删除的优先级最低。


