"""
08. 暗号文Permalink
与えられた文字列の各文字を，以下の仕様で変換する関数cipherを実装せよ．

英小文字ならば(219 - 文字コード)の文字に置換
その他の文字はそのまま出力
この関数を用い，英語のメッセージを暗号化・復号化せよ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""


def cipher(Str):
    Ciphered = []

    for i in range(len(Str)):
        if Str[i].isalpha() and Str[i].islower():
            Ciphered.append(chr(219-ord(Str[i])))
        else:
            Ciphered.append(Str[i])

    return "".join(Ciphered)


print(cipher("Hello, World!"))
