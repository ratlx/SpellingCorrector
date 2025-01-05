# 基于贝叶斯理论的中英双文拼写检查器

启发、借鉴于http://norvig.com/spell-correct.html

中文译文：https://blog.csdn.net/bobten2008/article/details/5478516

如果对于具体实现感兴趣，建议先阅读上述材料。

兼容windows，macos，linux平台

# 结果展示

<img width="361" alt="image" src="https://github.com/user-attachments/assets/b7db3079-96dd-4746-a04d-d8a907ec7063"><img width="286" alt="image" src="https://github.com/user-attachments/assets/7e6d7d90-651e-4958-8c56-52690212d898">

# 具体实现

1.SpellingCorrector类

英文拼写检查器，主要做了对Peter Norvig提出的拼写检查器的c++重构。重构借鉴了https://github.com/felipefarinon/spellingcorrector
，在此基础上做了性能上的优化，其他些许改造。

2.Pinyin类

来源于https://github.com/wzhe/GetPinyin
，主要作用是识别中文，获取中文拼音。

3.SpellingCorrectorForZhCN类

中文拼写检查器，私有继承了SpellingCorrector类，主要思路：拼音 --> 中文检查器 --> 英文检查器 --> 正确的拼音 --> 中文检查器 --> 中文。在原贝叶斯算法基础上，又套了一层贝叶斯———误差模型：拼音长度越长的优先；语言模型：中文频率；编辑距离：插入的优先级最高，删除的优先级最低。搜索策略(调用父类correct)：1.整体到局部；2.局部到整体。load函数用于加载词语；train函数用于对词语频率的统计。

# 可以改进的地方

1.提供的中文语料库有点脱离生活日常用语，不太符合输入法的使用场景

2.鉴于本人水平有限，子类中对调用父类correct的实现过于生硬，性能也不太好（但是输入法对性能的要求也不高？）。不管怎样，这个搜索策略是一个可以重点优化的地方

3.并没有实现上下文的联想功能，也是一个可以重点改进的地方

4.对于实验结果缺少量化分析

# Chinese-English Bilingual Spell Checker Based on Bayesian Theory

Inspired and referenced from: http://norvig.com/spell-correct.html
Chinese translation: https://blog.csdn.net/bobten2008/article/details/5478516

If you are interested in the implementation details, it is recommended to read the above materials first.

Compatibility: Windows, macOS, and Linux platforms

# Results Display

<img width="361" alt="image" src="https://github.com/user-attachments/assets/b7db3079-96dd-4746-a04d-d8a907ec7063"><img width="286" alt="image" src="https://github.com/user-attachments/assets/7e6d7d90-651e-4958-8c56-52690212d898">

# Implementation Details

1. SpellingCorrector Class

An English spell checker, primarily a C++ reconstruction of Peter Norvig’s spell checker. The reconstruction was inspired by https://github.com/felipefarinon/spellingcorrector, with performance optimizations and some additional modifications.

2. Pinyin Class

Based on https://github.com/wzhe/GetPinyin. Its primary function is to recognize Chinese characters and retrieve their corresponding Pinyin.

3. SpellingCorrectorForZhCN Class

A Chinese spell checker that privately inherits the SpellingCorrector class.

# Main approach:
	1.	Convert Pinyin → Chinese spell checker → English spell checker → Correct Pinyin → Chinese spell checker → Chinese text.
	2.	Added an additional Bayesian layer on top of the original Bayesian algorithm:
	•	Error Model: Longer Pinyin sequences are prioritized.
	•	Language Model: Frequencies of Chinese phrases.
	•	Edit Distance: Insertion is prioritized highest, and deletion is lowest.
	3.	Search Strategy (using parent class correct):
	•	Global to local.
	•	Local to global.
	4.	load function: Loads words.
	5.	train function: Counts word frequencies.

Areas for Improvement

	1.	The provided Chinese corpus diverges from everyday conversational language, making it less suitable for input method use cases.
	2.	Due to my limited expertise, the implementation of calling the parent class’s correct function in the subclass feels clunky, and the performance isn’t optimal (though input methods typically don’t demand high performance?). Regardless, this search strategy is a key area for optimization.
	3.	Contextual suggestion functionality has not been implemented, which could be another significant improvement area.




