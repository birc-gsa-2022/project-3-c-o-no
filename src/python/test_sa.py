import sa
import st
import random as r


SAtests = {"": [0], "a": [1, 0], "ab": [2,0,1], "abc": [3,0,1,2], "cba": [3,2,1,0], "abab": [4, 2, 0, 3, 1]}

def test_constructSAradixDefined():
    for x, res in SAtests.items():
        assert sa.constructSAradix(x) == res, f"Did not sort correctly for x={x}"

def test_constructSAtreeDefined():
    for x, res in SAtests.items():
        assert list(sa.constructSAtree(x)) == res, f"Did not sort correctly for x={x}"

def test_searchDefined():
    tests = [("","", []), ("a", "b", []), ("a", "a", [0]), ("a"*10, "a", list(range(10))), ("ababcab", "ab", [0,2,5])]
    for (x, p, res) in tests:
        search = list(sa.search(x, p))
        search.sort()
        assert search == res, f"Wrong search result for x={x}, p={p}. Got {search}"

def test_searchRandom():
    for n in range(50):
        x = "".join(r.choices("actg", k=n))
        for m in range(n+1):
            pattern1 = "".join(r.choices("actg", k=m))
            search1 = list(sa.search(x, pattern1))
            search1.sort()
            res = list(st.search(x, pattern1))
            res.sort()
            assert search1 == res, f"Wrong search result for x={x} and random pattern, {pattern1}. Got {search1}"
            i = r.randint(0, n-m)
            pattern2 = x[i: i+m]
            search2 = list(sa.search(x, pattern2))
            search2.sort()
            res = list(st.search(x, pattern2))
            res.sort()
            assert search2 == res, f"Wrong search result for x={x} and pattern {pattern2}. Got {search2}"

def compareAlgorithmsSA(x, *algorithms):
    res = [a(x) for a in algorithms]
    for i in range(len(res)-1):
        assert list(res[i]) == list(res[i+1]), f"Different results for x={x}. {algorithms[i].__name__} gave {list(res[i])} while {algorithms[i+1].__name__} gave {list(res[i+1])}"

def test_compareSA():
    for n in range(50):
        x = "".join(r.choices("actg", k=n))
        compareAlgorithmsSA(x, sa.constructSAradix, sa.constructSAtree)
