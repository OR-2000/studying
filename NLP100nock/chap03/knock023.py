"""
23. セクション構造Permalink
記事中に含まれるセクション名とそのレベル（例えば”== セクション名 ==”なら1）を表示せよ．

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

for i in lists:
    t = re.search(r'^(=+)\s*(.*?)\s*(=+)$', i)
    # +：一回以上の繰り返し
    # \s：任意の空白文字
    if t is not None:
        print(t.group(2), len(t.group(1)) - 1)
