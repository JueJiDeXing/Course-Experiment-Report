# 列表对象创建
list1 = []
list2 = list()
list3 = [0, 1, 2, 3, 4]
list4 = [i for i in range(5)]
# 列表对象创建
tuple1 = ()
tuple2 = (1,)
tuple3 = tuple([1, 2])
# 字符串对象创建
s1 = ""
s2 = ''
s3 = str(1)
s4 = str([1, 2, 3])
s5 = str((1, 2))
s6 = [1, 2, 3].__str__()
# 数字序列对象创建
r1 = range(5)
r2 = range(3, 8)
r3 = range(6, 2, -1)
# 打印结果
print("列表对象创建结果:")
print(list1)
print(list2)
print(list3)
print(list4)
print("元组对象创建结果:")
print(tuple1)
print(tuple2)
print(tuple3)
print("字符串对象创建结果:")
print(s1)
print(s2)
print(s3)
print(s4)
print(s5)
print(s6)
print("数字序列对象创建结果:")
print(r1)
print(r2)
print(r3)
