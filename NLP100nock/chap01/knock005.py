"""
05. n-gram
与えられたシーケンス（文字列やリストなど）からn-gramを作る関数を作成せよ．
この関数を用い，”I am an NLPer”という文から単語bi-gram，文字bi-gramを得よ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""

sentence = "I am an NLPer"


def word_n_gram(sentence, N):
    words = sentence.split()
    result = []

    for i in range(len(words) - N + 1):
        result.append(words[i: i + N])
    return result


print(word_n_gram(sentence, N=2))
print(word_n_gram(sentence, N=3))


def char_n_gram(sentence, N):
    words = sentence.split()
    chars = "".join(words)
    result = []

    for i in range(len(chars) - N + 1):
        result.append(chars[i: i + N])
    return result


print(char_n_gram(sentence, N=2))
print(char_n_gram(sentence, N=3))
