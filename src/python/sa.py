# Python code made to understand the algorithms before implementing it in C

import st
from collections import deque


def constructSAradix(x):
    x += "$"
    n = len(x)
    sa = list(range(n))
    bucketIndecies = dict()
    buckets = dict()
    for char in x:
        bucketIndecies[char] = bucketIndecies.get(char, 0) + 1
        buckets[char] = 0
    accumSum = 0
    for char in sorted(bucketIndecies.keys()):
        count = bucketIndecies[char]
        bucketIndecies[char] = accumSum
        accumSum += count

    for i in range(n - 1, -1, -1):
        for char in buckets:
            buckets[char] = 0

        saOld = sa.copy()
        for suffixIndex in saOld:
            charIndex = (suffixIndex + i) % n
            char = x[charIndex]
            elemInBucket = buckets[char]
            saIndex = bucketIndecies[char] + elemInBucket
            buckets[char] = elemInBucket + 1
            sa[saIndex] = suffixIndex
    return sa


def constructSAtree(x):
    node = st.constructTreeMcCreight(x)
    stack = deque()
    stack.append(node)
    while stack:
        node = stack.pop()
        if node.isInnerNode():
            children = node.childrenOrLabel
            for childChar in sorted(children.keys(), reverse=True):
                stack.append(children[childChar])
        else:
            yield node.childrenOrLabel


def searchSA(x, p, sa):
    x += "$"
    n = len(sa)

    def binarySearch(patchar, patindex, intervalStart, intervalEnd, mode=0):
        # mode = 0: Stop when found
        # mode = 1: Find left border
        # mode = -1: Find right border
        while intervalStart != intervalEnd:
            mid = int((intervalStart + intervalEnd) / 2)
            xchar = x[sa[mid] + patindex]
            match = patchar == xchar
            if match:
                if mode == 0:
                    return intervalStart, mid, intervalEnd
                elif mode == -1:
                    intervalEnd = mid
                else:
                    intervalStart = mid + 1
            else:
                if xchar > patchar:
                    intervalEnd = mid
                else:
                    intervalStart = mid + 1
        return intervalStart, 0, intervalEnd

    start = 0
    end = n
    for i, pchar in enumerate(p):
        # Find first occurence
        s, m, e = binarySearch(pchar, i, start, end, mode=0)
        if s == e:
            return
        # Find left interval:
        start, _, _ = binarySearch(pchar, i, s, m, mode=-1)

        # Find right interval:
        _, _, end = binarySearch(pchar, i, m, e, mode=1)

    for i in range(start, end):
        yield sa[i]


def search(x, p):
    if not x or not p:
        return
    sa = constructSAradix(x)
    yield from searchSA(x, p, sa)
