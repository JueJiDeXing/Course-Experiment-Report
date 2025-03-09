import time

n = 1000


def printTime(func):  # 函数装饰器,统计并打印时间
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)  # 调用目标函数
        end = time.perf_counter()
        print(f"\033[32m {func.__name__} 运行耗时 {(end - start) * 1000} ms \033[0m")
        return result

    return wrapper


@printTime
def method1(n):
    if n < 0:
        raise Exception("项数非负")
    """打印斐波那契数列前n项"""
    print("[", end="")

    def fun(a, b, n):
        """从a和b开始向后递推n项"""
        if n == 0:
            return
        print(a, end=', ')
        fun(b, a + b, n - 1)

    fun(1, 1, n)  # 使用函数栈进行递推
    if n != 0:
        print('\b\b', end='')
    print(']')


method1(n)


@printTime
def method2(n):
    if n < 0:
        raise Exception("项数非负")
    a, b = 0, 1
    print("[", end="")
    for i in range(n):
        print(b, end=', ')
        a, b = b, a + b  # 递推
    if n != 0:
        print('\b\b', end='')
    print("]")


method2(n)
