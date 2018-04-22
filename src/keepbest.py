import sys

def usage():
  print("Usage:", sys.argv[0], "<max/min>")
  sys.exit(1)

if __name__ == "__main__":
  if len(sys.argv) != 2:
    usage()
  if sys.argv[1] == "max":
    better = (lambda a, b: a > b)
    best = float("-inf")
  elif sys.argv[1] == "min":
    better = (lambda a, b: a < b)
    best = float("inf")
  else:
    usage()
  bests = []
  for line in sys.stdin:
    try:
      l = line.split()
      sig, val = l[0], int(l[1])
      if better(val, best):
        best = val
        bests = [sig]
        print(best, sig, file=sys.stderr)
      elif best == val:
        bests.append(sig)
        print(best, sig, file=sys.stderr)
    except:
      continue
  for sig in bests:
    print(best, sig)

