import time

class Clock:
    def init(self):
        self.time = 0

    def startTimer(self):
        self.time = time.perf_counter()

    def stopTimer(self):
        stopTime = time.perf_counter()
        return stopTime - self.time

    def getTime(self, algorithm, *args):
        starttime = time.perf_counter()
        algorithm(*args)
        endtime = time.perf_counter()
        return endtime-starttime

    def getAlgorithmTimeAndResult(self, algorithm, *args):
        starttime = time.perf_counter()
        res = algorithm(*args) 
        endtime = time.perf_counter()
        return endtime-starttime, res 

    def sleep(self, sec):
        time.sleep(sec)

    def getAverageTime(self, iterations, algorithm, *args):
        total = 0.0
        for _ in range(iterations):
            total += self.getTime(algorithm, *args)
        return total/float(iterations)

    def getAverageTimeAndResult(self, iterations, algorithm, *args):
        total = 0.0
        for _ in range(iterations-1):
            total += self.getTime(algorithm, *args)
        t, res = self.getAlgorithmTimeAndResult(algorithm, *args)
        total += t
        return total/float(iterations), res 

