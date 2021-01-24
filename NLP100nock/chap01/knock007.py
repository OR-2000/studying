"""
07. テンプレートによる文生成Permalink
引数x, y, zを受け取り「x時のyはz」という文字列を返す関数を実装せよ．
さらに，x=12, y=”気温”, z=22.4として，実行結果を確認せよ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""


def getTemplate(x, y, z):
    return f'{x}時の{y}は{z}'


print(getTemplate(12, '気温', 22.4))
