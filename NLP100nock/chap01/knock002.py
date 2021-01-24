"""
02. 「パトカー」＋「タクシー」＝「パタトクカシーー」
「パトカー」＋「タクシー」の文字を先頭から交互に連結して文字列「パタトクカシーー」を得よ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""

X = 'パトカー'
Y = 'タクシー'

Str = []
for x, y in zip(X, Y):
    Str.append(x)
    Str.append(y)

print(''.join(Str))
