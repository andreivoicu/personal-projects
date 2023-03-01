time = 60
score = 0
missed = 0
characterNo = 0
difficulty = "Medium"


def getTime():
    global time
    return time


def setTime(val):
    global time
    time = val


def getScore():
    global score
    return score


def getMissed():
    global missed
    return missed


def getDifficulty():
    global difficulty
    return difficulty


def setDifficulty(dif):
    global difficulty
    difficulty = dif
