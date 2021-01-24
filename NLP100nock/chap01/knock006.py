"""
06. 集合
“paraparaparadise”と”paragraph”に含まれる文字bi-gramの集合を，
それぞれ, XとYとして求め，XとYの和集合，積集合，差集合を求めよ．
さらに，’se’というbi-gramがXおよびYに含まれるかどうかを調べよ．

[Ref]
https://nlp100.github.io/ja/ch01.html
"""


x1 = "paraparaparadise"
y1 = "paragraph"


def n_gram(sentence, N):
    result = {}
    for i in range(len(sentence) - N + 1):
        if sentence[i: i+N] in result:
            result[sentence[i: i+N]] += 1
        else:
            result[sentence[i: i+N]] = 1
    return result


X = n_gram(x1, N=2)
Y = n_gram(y1, N=2)

print(X)
print(Y)


def union(X, Y):

    dict_union = X.copy()

    for s, t in Y.items():
        if s in X:
            dict_union[s] += t
        else:
            dict_union[s] = t
    return dict_union


print(union(X, Y))


def difference_set(X, Y):
    dict_ds = X.copy()

    for s, t in Y.items():
        if s in X:
            del dict_ds[s]
        else:
            dict_ds[s] = t
    return dict_ds


print(difference_set(X, Y))


def intersection(X, Y):
    dict_is = {}

    for s, t in Y.items():
        if s in X:
            dict_is[s] = t + X[s]
    return dict_is


print(intersection(X, Y))
