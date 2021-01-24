"""
25. テンプレートの抽出Permalink
記事中に含まれる「基礎情報」テンプレートのフィールド名と値を抽出し，辞書オブジェクトとして格納せよ．

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

print(re.search(r''))
