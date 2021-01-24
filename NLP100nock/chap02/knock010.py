"""
10. 行数のカウント
行数をカウントせよ．確認にはwcコマンドを用いよ．

[Ref]
https://nlp100.github.io/ja/ch02.html

[Unix]
% wc ../inputs/popular-names.txt
    2780   11120   55026 ../inputs/popular-names.txt
"""

with open("inputs/popular-names.txt", encoding="utf-8") as file:
    Lines = file.readlines()
    print(len(Lines))
