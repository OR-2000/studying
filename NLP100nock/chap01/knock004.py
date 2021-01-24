"""
04. 元素記号
“Hi He Lied Because Boron Could Not Oxidize Fluorine. New Nations Might Also Sign Peace Security Clause. Arthur King Can.”
という文を単語に分解し，1, 5, 6, 7, 8, 9, 15, 16, 19番目の単語は先頭の1文字，それ以外の単語は先頭の2文字を取り出し，
取り出した文字列から単語の位置（先頭から何番目の単語か）への連想配列（辞書型もしくはマップ型）を作成せよ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""


Words = "Hi He Lied Because Boron Could Not Oxidize Fluorine. New Nations Might Also Sign Peace Security Clause. Arthur King Can.".split()

dic = {}

for idx in range(len(Words)):
    if idx + 1 in [1, 5, 6, 7, 8, 9, 15, 16, 19]:
        dic[Words[idx][:1]] = idx + 1
    else:
        dic[Words[idx][:2]] = idx + 1

print(dic)
