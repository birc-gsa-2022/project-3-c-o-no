# Tree construction from project 2

from tree import Node, linkedNode


def constructTreeMcCreight(x: str):
    x += "$"
    n = len(x)

    firstLeaf = linkedNode((1, n), 0)
    root = linkedNode((0,0), {x[0] : firstLeaf})
    root.parent = root.suffixLink = root
    firstLeaf.parent = root
    lastHead = root
    
    def fastScan(node: linkedNode, stringRange: tuple[int, int], suffixStart: int, suffixIndex: int) -> tuple[linkedNode, bool]:
        fastEdgeStart, fastEdgeEnd = stringRange
        while True:
            if fastEdgeStart-1 == fastEdgeEnd:
                return node, False
            char = x[fastEdgeStart-1]
            node = node.childrenOrLabel[char]
            l1, l2 = node.stringRange
            curEdgeLen = l2-l1
            nextFastEdgeStart = fastEdgeStart+curEdgeLen+1
            if nextFastEdgeStart-1 > fastEdgeEnd:
                #split on this edge
                newLeaf = linkedNode((suffixIndex+1, n), suffixStart)

                splitIndex = l1 + fastEdgeEnd - fastEdgeStart
                splitNode = linkedNode((l1,splitIndex), {x[suffixIndex] : newLeaf, x[splitIndex] : node}, node.parent)
                newLeaf.parent = splitNode
                node.stringRange = (splitIndex+1, l2)
                node.parent.childrenOrLabel[char] = splitNode
                node.parent = splitNode

                return splitNode, True
            fastEdgeStart = nextFastEdgeStart

    suffixIndex = 1
    for suffixStart in range(1, n):
        if lastHead == root:
            node = root
            suffixIndex = suffixStart
            char = x[suffixIndex]
        else:
            parentIsRoot = lastHead.parent == root
            hstart, hend = lastHead.stringRange
            node, madeNewNode = fastScan(lastHead.parent.suffixLink, (hstart+parentIsRoot, hend), suffixStart, suffixIndex)
            lastHead.suffixLink = node

            if madeNewNode:
                lastHead = node
                continue
        
            char = x[suffixIndex] 
            if char not in node.childrenOrLabel:
                lastHead = node
                newLeaf = linkedNode((suffixIndex+1, n), suffixStart, node)
                node.childrenOrLabel[char] = newLeaf
                continue
            node = node.childrenOrLabel[char]
            suffixIndex += 1

        #Slow scan 
        while True:
            edgestart, edgeend = node.stringRange
            
            i = 0
            for i in range(edgeend-edgestart): 
                if x[suffixIndex] != x[edgestart+i]:
                    # Split node 
                    newLeaf = linkedNode((suffixIndex+1, n), suffixStart)
                    splitNode = linkedNode((edgestart,edgestart+i), {x[suffixIndex] : newLeaf, x[edgestart+i] : node}, node.parent)
                    newLeaf.parent = splitNode             
                    node.stringRange = (edgestart+i+1, edgeend)
                    node.parent.childrenOrLabel[char] = splitNode
                    node.parent = splitNode
                    lastHead = splitNode
                    break
                suffixIndex += 1 
            else: #Did not mismatch
                char = x[suffixIndex]
                if char in node.childrenOrLabel:
                    suffixIndex += 1
                    node = node.childrenOrLabel[char]
                else: #Add new leaf to inner node
                    newLeaf = linkedNode((suffixIndex+1, n), suffixStart, node)
                    node.childrenOrLabel[char] = newLeaf
                    lastHead = node
                    break
                continue
            break
    return root

def constructTreeNaive(x: str):
    x += "$"
    n = len(x)

    firstLeaf = Node((1, n), 0)
    root = Node((0,0), {x[0] : firstLeaf})
    root.parent = root
    firstLeaf.parent = root

    for suffixStart in range(1, n):
        node = root
        suffixIndex = suffixStart
        char = x[suffixIndex]

        while True:
            edgestart, edgeend = node.stringRange
                
            for i in range(edgeend-edgestart):
                if x[suffixIndex] != x[edgestart+i]:
                    # Split node 
                    newLeaf = Node((suffixIndex+1, n), suffixStart)
                    splitNode = Node((edgestart,edgestart+i), {x[suffixIndex] : newLeaf, x[edgestart+i] : node}, node.parent)
                    newLeaf.parent = splitNode             
                    node.stringRange = (edgestart+i+1, edgeend)
                    node.parent.childrenOrLabel[char] = splitNode
                    node.parent = splitNode
                    break
                suffixIndex += 1 
            else: #Did not mismatch
                char = x[suffixIndex]
                if char in node.childrenOrLabel:
                    suffixIndex += 1
                    node = node.childrenOrLabel[char]
                else: #Add new leaf to inner node
                    newLeaf = Node((suffixIndex+1, n), suffixStart, node)
                    node.childrenOrLabel[char] = newLeaf
                    break
                continue
            break
    return root

def search(x, p):
    if not x or not p:
        return 
    t = constructTreeMcCreight(x)
    yield from searchTree(t, p, x+"$")

def searchNaive(x, p):
    if not x or not p:
        return 
    t = constructTreeNaive(x)
    yield from searchTree(t, p, x+"$")

def searchTree(tree: Node, p: str, x: str):
    node = tree
    # find pattern node
    i = 0
    m = len(p)
    while True:
        if i == m:
            yield from findLeaves(node)
            return
        if node.isLeaf():
            return 
        if not p[i] in node.childrenOrLabel:
            return
        node = node.childrenOrLabel[p[i]]
        i += 1 
        
        edgeStart, edgeEnd = node.stringRange
        for edgeIndex in range(edgeStart, edgeEnd):
            if i == m:
                yield from findLeaves(node)
                return
            if p[i] != x[edgeIndex]:
                return
            i += 1

def findLeaves(t: Node | None):
    stack = []
    if t:
        stack.append(t)
    
    while stack:
        node = stack.pop()
        if node.isLeaf():
            yield node.childrenOrLabel
        else:
            for c in node.childrenOrLabel.values():
                stack.append(c)

