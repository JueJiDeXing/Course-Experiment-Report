def printSize(num):
    print(f"{num}的内存大小为{num.__sizeof__()}")


printSize(0)
for i in range(0, 200, 20):
    printSize(1 << i)
