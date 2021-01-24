"""
12. 1列目をcol1.txtに，2列目をcol2.txtに保存
各行の1列目だけを抜き出したものをcol1.txtに，2列目だけを抜き出したものをcol2.txtとしてファイルに保存せよ．
確認にはcutコマンドを用いよ．

[Ref]
https://nlp100.github.io/ja/ch02.html

[Unix]
"""

Lines = []

with open("inputs/popular-names.txt", encoding="utf-8") as file:
    for line in file:
        Lines.append(line.rstrip().split("	"))

with open("outputs/knock012/col1.txt", "w", encoding="utf-8") as col1:
    with open("outputs/knock012/col2.txt", "w") as col2:
        for idx in range(len(Lines)):
            col1.write(Lines[idx][0] + "\n")
            col2.write(Lines[idx][1] + "\n")
