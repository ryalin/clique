import random

# Python file to generate graph txt files

# Generates a single clique of cliqueSize, then attaches
# the rest of the nodes to the clique
def generateOneClique(cliqueSize, graphSize):
  graph = dict()

  # Create a clique of cliqueSize
  for node in range(cliqueSize):
    graph[node] = set()
    for neighbor in range(cliqueSize):
      if node == neighbor:
        continue
      graph[node].add(neighbor)

  # Connect rest of nodes to clique
  for node in range(cliqueSize, graphSize):
    graph[node] = set()
    correspCliqueNode = (node - cliqueSize) % cliqueSize
    graph[node].add(correspCliqueNode)
    graph[correspCliqueNode].add(node)

  return graph
  

# Given cliqueSizes, generates multiplie cliques then
# connect them using one node from each clique
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
    oneClique50 = generateOneClique(50, 1000)
    saveGraphToTxt(oneClique50, 'oneClique50.txt')

    oneClique100 = generateOneClique(100, 2000)
    saveGraphToTxt(oneClique100, 'oneClique100.txt')

    # Make some changes to numbers here (higher ratio of graph size to clique Size)
    oneClique200 = generateOneClique(200, 4000)
    saveGraphToTxt(oneClique200, 'oneClique200.txt')

    # Multiple clique test files
    multiTest1 = [10, 20, 30, 40, 50]
    multiClique1 = generateMultiClique(multiTest1)
    saveGraphToTxt(multiClique1, 'multiClique50.txt')

    multiTest2 = [2, 8, 15, 30, 60, 125, 250, 500]
    multiClique2 = generateMultiClique(multiTest2)
    saveGraphToTxt(multiClique2, 'multiClique500.txt')

    multiTest3 = [8, 16, 35, 70, 150, 300, 600, 2000]
    multiClique3 = generateMultiClique(multiTest3)
    saveGraphToTxt(multiClique3, 'multiClique2000.txt')
    
    # Random graph test files
    randomGraph50 = generateRandomGraph(50)
    saveGraphToTxt(randomGraph50, 'randomGraph50.txt')

    randomGraph500 = generateRandomGraph(500)
    saveGraphToTxt(randomGraph500, 'randomGraph500.txt')

    randomGraph2000 = generateRandomGraph(2000)
    saveGraphToTxt(randomGraph2000, 'randomGraph2000.txt')

