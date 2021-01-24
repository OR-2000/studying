"""
09. Typoglycemia
スペースで区切られた単語列に対して，各単語の先頭と末尾の文字は残し，それ以外の文字の順序をランダムに並び替えるプログラムを作成せよ．
ただし，長さが４以下の単語は並び替えないこととする．
適当な英語の文
（例えば”I couldn’t believe that I could actually understand what I was reading : the phenomenal power of the human mind .”）
を与え，その実行結果を確認せよ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""

import random


def typoglycemia(Str):
    Words = []

    for word in Str.split():
        if 4 < len(word):
            Chars = []

            for char in word[1:-1]:
                Chars.append(char)

            random.shuffle(Chars)

            Words.append("".join([word[0]] + Chars + [word[-1]]))
        else:
            Words.append(word)

    return " ".join(Words)


print(typoglycemia("I couldn't believe that I could actually understand what I was reading : the phenomenal power of the human mind ."))
