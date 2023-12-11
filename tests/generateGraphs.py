import random

# Python file to generate graph txt files

# Generates a single clique of cliqueSize
# used for quick correctness checks
def generateOneClique(cliqueSize):
  graph = dict()

  # Create a clique of cliqueSize
  for node in range(cliqueSize):
    graph[node] = set()
    for neighbor in range(cliqueSize):
      if node == neighbor:
        continue
      graph[node].add(neighbor)

  return graph
  

# Given cliqueSizes, generates multiplie cliques then
# connect them using one node from each clique
# used for quick correctness checks
def generateMultiClique(cliqueSizes):
  graph = dict()
  usedNodes = 0

  # For each cliqueSize, create a clique
  for cliqueSize in cliqueSizes:
    for node in range(usedNodes, usedNodes + cliqueSize):
      graph[node] = set()
      for neighbor in range(usedNodes, usedNodes + cliqueSize):
        if node == neighbor:
          continue
        graph[node].add(neighbor)
    usedNodes += cliqueSize

  # Find a node in each clique to connect cliques with
  repNodes = []
  lastRepNode = 0
  for i in range(len(cliqueSizes)):
    repNodes += [lastRepNode]
    lastRepNode += cliqueSizes[i]
  
  # Join cliques together with the found representative nodes
  for i in range(1, len(repNodes)):
    prev = repNodes[i - 1]
    curr = repNodes[i]
    graph[prev].add(curr)
    graph[curr].add(prev)
  
  return graph


# Generates a random graph given graphSize
# used for benchmarking
def generateRandomGraph(graphSize):
  graph = dict()

  for node in range(graphSize):
    graph[node] = set()

  # For each node, generate random list of neighbors
  for node in range(0, graphSize):
     randomSetSize = random.randint(1, graphSize - 1)

     while len(graph[node]) < randomSetSize:
       randomNeighbor = random.randint(0, graphSize - 1)
       if node != randomNeighbor:
         graph[node].add(randomNeighbor)
         graph[randomNeighbor].add(node)

  return graph


# Given a dictionary (graph) and file name, json dump
def saveGraphToTxt(graph, filename):

    with open(filename, 'w') as txtFile:
      for node, neighbors in graph.items():
        neighbors_str = ' '.join(map(str, neighbors))
        txtFile.write(f"{node} {neighbors_str}\n")


# Add test cases here
if __name__ == "__main__":
    
    # One clique test files
    oneClique5 = generateOneClique(5)
    saveGraphToTxt(oneClique5, 'one_clique5.txt')

    oneClique15 = generateOneClique(15)
    saveGraphToTxt(oneClique15, 'one_clique15.txt')

    oneClique30 = generateOneClique(30)
    saveGraphToTxt(oneClique30, 'one_clique30.txt')

    oneClique45 = generateOneClique(45)
    saveGraphToTxt(oneClique45, 'one_clique45.txt')

    # Multiple clique test file
    multiTest = [10, 20, 30, 40, 50, 60, 70, 80]
    multiClique = generateMultiClique(multiTest)
    saveGraphToTxt(multiClique, 'multi_clique80.txt')
