"""
00. 文字列の逆順
文字列”stressed”の文字を逆に（末尾から先頭に向かって）並べた文字列を得よ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""

# 1
Str = 'stressed'
print(Str[::-1])

# 2
List = list(Str)
List.reverse()
print("".join(List))
