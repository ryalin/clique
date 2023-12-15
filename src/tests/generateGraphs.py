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
def randomGraph(graphSize, edgeProb):
  graph = {}
  random.seed()

  for i in range(graphSize):
      for j in range(i + 1, graphSize):
          if random.random() < edgeProb:
              if i not in graph:
                  graph[i] = set()
              if j not in graph:
                  graph[j] = set()
              graph[i].add(j)
              graph[j].add(i)
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

  # Multiple clique test file
  multiTest = [10, 20, 30, 40, 50, 60, 70, 80]
  multiClique = generateMultiClique(multiTest)
  saveGraphToTxt(multiClique, 'multi_clique.txt')

  multiTest1 = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4]
  multiClique1 = generateMultiClique(multiTest1)
  saveGraphToTxt(multiClique1, 'multiDense1.txt')

  multiTest2 = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4]
  multiClique2 = generateMultiClique(multiTest2)
  saveGraphToTxt(multiClique2, 'multiDense2.txt')

  multiTest3 = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 4]
  multiClique3 = generateMultiClique(multiTest3)
  saveGraphToTxt(multiClique3, 'multiDense3.txt')

  multiTest4 = [3, 1, 1, 3, 2, 1, 1, 1, 2, 1, 3, 1, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 1, 2, 1, 4]
  multiClique4 = generateMultiClique(multiTest4)
  saveGraphToTxt(multiClique4, 'multiSparse1.txt')

  multiTest5 = [3, 1, 1, 3, 2, 1, 1, 1, 2, 1, 4, 1, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2]
  multiClique5 = generateMultiClique(multiTest5)
  saveGraphToTxt(multiClique5, 'multiSparse2.txt')

  multiTest6 = [3, 1, 1, 3, 2, 1, 1, 1, 2, 1, 3, 1, 2, 2, 1, 2, 1, 1, 1, 4, 2, 1, 2, 1, 1, 2, 1, 2]
  multiClique6 = generateMultiClique(multiTest6)
  saveGraphToTxt(multiClique6, 'multiSparse3.txt')

  multiTest7 = [3, 2, 2, 3, 2, 1, 1, 3, 3, 1, 3, 1, 2, 3, 1, 2, 3, 3, 3, 3, 2, 1, 2, 1, 1, 2, 1, 4]
  multiClique7 = generateMultiClique(multiTest7)
  saveGraphToTxt(multiClique7, 'multiMedium1.txt')

  multiTest8 = [3, 2, 2, 3, 2, 1, 1, 3, 3, 1, 3, 1, 4, 3, 1, 2, 3, 3, 3, 3, 2, 1, 2, 1, 1, 2, 1, 2]
  multiClique8 = generateMultiClique(multiTest8)
  saveGraphToTxt(multiClique8, 'multiMedium2.txt')

  multiTest9 = [3, 2, 2, 3, 2, 1, 1, 3, 3, 1, 3, 1, 2, 3, 1, 2, 3, 3, 3, 4, 2, 1, 2, 1, 1, 2, 1, 2]
  multiClique9 = generateMultiClique(multiTest9)
  saveGraphToTxt(multiClique9, 'multiMedium3.txt')

  rg1 = randomGraph(100, 0.1)
  saveGraphToTxt(rg1, 'recurse_100_01.txt')

  rg2 = randomGraph(50, 0.2)
  saveGraphToTxt(rg2, 'recurse_50_02.txt')

  rg3 = randomGraph(10000, 0.01)
  saveGraphToTxt(rg3, 'bronk_10000_001.txt')

  rg4 = randomGraph(1000, 0.1)
  saveGraphToTxt(rg4, 'bronk_1000_01.txt')
