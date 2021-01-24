"""
20. JSONデータの読み込み
Wikipedia記事のJSONファイルを読み込み，「イギリス」に関する記事本文を表示せよ．
問題21-29では，ここで抽出した記事本文に対して実行せよ．

[Ref]
https://nlp100.github.io/ja/ch03.html
"""

import json

with open('inputs/jawiki-country.json') as file:
    for line in file:
        dic = json.loads(line)

        if dic["title"] == "イギリス":
            print(dic["text"])
