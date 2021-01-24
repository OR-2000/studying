"""
22. カテゴリ名の抽出
記事のカテゴリ名を（行単位ではなく名前で）抽出せよ．

[Ref]
https://nlp100.github.io/ja/ch02.html
"""

import json
import re

text = ""
with open('input/jawiki-country.json') as f:
    for i in f:
        dic = json.loads(i)
        if dic["title"] == "イギリス":
            text = dic["text"]

lists = text.split("\n")
# print(lists)

for i in lists:
    if "Category" in i:
        t = re.search(r'^\[\[Category:(.*?)(\|.*)*\]\]$', i)
        # .：改行以外の任意の一文字
        # *：直前のパターンを0回以上繰り返し
        # ?：直前のパターンを0回または1回以上繰り返し
        # []：文字の集合
        # \ ：or
        # ^：文字列の先頭
        # $：文字列の末尾
        # (.*?)：最短マッチ
        print(t.group(1))
