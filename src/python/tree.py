
from dataclasses import dataclass, field


@dataclass
class Node:
    stringRange: tuple[int, int] #Including first, excluding last
    childrenOrLabel: dict[str, 'Node'] | int = -1 #Combined children and label 
    parent: 'Node | None' = field(default=None, compare=False) #Avoid recursive compareson. Not comparing parent is only a problem in special cases that should never happen

    def getRangeAndLabelString(self):
        return f"{self.stringRange} {self.childrenOrLabel if self.isLeaf() else ''}"

    def prettyString(self, level=0):
        selfString = self.getRangeAndLabelString()
        if self.isInnerNode():
            kids = self.childrenOrLabel
            for child in kids:
                selfString += "\n"
                selfString += "\t"*level + str(child) + " \u2192\t" + kids[child].prettyString(level+1)
        return selfString

    def isInnerNode(self):
        return type(self.childrenOrLabel) != int
    
    def isLeaf(self):
        return type(self.childrenOrLabel) == int

@dataclass 
class linkedNode(Node):
    childrenOrLabel: dict[str, 'linkedNode'] | int = -1 #Combined children and label 
    parent: 'linkedNode | None' = field(default=None, compare=False) #Avoid recursive compareson. Not comparing parent is only a problem in special cases that should never happen
    suffixLink: 'linkedNode | None' = field(default=None, compare=False)

    def assertEqualToNode(self, n: Node):
        assert self.stringRange == n.stringRange, f"Not same range. {self.stringRange} and {n.stringRange}"
        assert self.isInnerNode() == n.isInnerNode(), "One if leaf, one is inner node"
        if self.isInnerNode():
            for c in self.childrenOrLabel:
                self.childrenOrLabel[c].assertEqualToNode(n.childrenOrLabel[c])
            for c in n.childrenOrLabel:
                self.childrenOrLabel[c].assertEqualToNode(n.childrenOrLabel[c])
        else:
            assert self.childrenOrLabel == n.childrenOrLabel, "Not same label"
