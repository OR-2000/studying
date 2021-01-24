"""
11. タブをスペースに置換
タブ1文字につきスペース1文字に置換せよ．
確認にはsedコマンド，trコマンド，もしくはexpandコマンドを用いよ．

[Ref]
https://nlp100.github.io/ja/ch02.html

[Unix]
"""

with open("inputs/popular-names.txt", encoding="utf-8") as file:
    Lines = file.readlines()

    for line in Lines:
        print(" ".join(line.rstrip().split('	')))
