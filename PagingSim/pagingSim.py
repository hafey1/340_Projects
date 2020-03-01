import sys
import os
import random

def validInput(args):
    # A method to check that the proper number of variables were given
    if len(args) != 8:
        print("Incorrect Input:")
        print("python3 pagingSim.py <memory size> <page size> <number of jobs> <runtime min> "
              "<runtime max> <memory min> <memory max>")
        sys.exit(1)

def checkSeed(randSeed):
    # A method to check that the random seed has been set and chooses random if not
    if randSeed == 'None'or randSeed == "":
        randSeed = random.randint(1,100)
    return randSeed

def validPageSize(memSize, pageSize):
    # A method to check that the memory size is an even multiple of page size
    if (memSize%pageSize) != 0:
        print("Page Size Must Be An Even Multiple Of Memory Size")
        sys.exit(1)

def jobCreations(runTimeMin, runTimeMax, memMin, memMax, randSeed, numJobs):
    # A method to create jobs and print them
    runTime = []
    jobMem = []
    print("Job Queue:\n  Job #    Runtime    Memory")
    for job in range(numJobs):
        random.seed(randSeed)
        runTime.append(random.randint(runTimeMin, runTimeMax))
        jobMem.append(random.randint(memMin, memMax))
        print("{0: >7}{1: >11}{2: >10}".format((job+1), runTime[job], jobMem[job]))
        randSeed+=1
    return runTime, jobMem

    return runTime, jobMem

def main():

    # Get arguments from command line
    validInput(sys.argv)
    memSize = int(sys.argv[1])
    pageSize = int(sys.argv[2])
    validPageSize(memSize, pageSize)
    # 'export RANDOM_SEED=decimal' in command line
    randSeed = int(os.environ.get("RANDOM_SEED"))
    randSeed = int(checkSeed(randSeed))
    numJobs = int(sys.argv[3])
    runTimeMin = int(sys.argv[4])
    runTimeMax = int(sys.argv[5])
    memMin = int(sys.argv[6])
    memMax = int(sys.argv[7])

    print("Simulator Parameters:\n  Memory Size: {0}\n  Page Size: {1}\n  Random Seed: {2}\n"
          "  Number of Jobs: {3}\n  Runtime (min-max) Timesteps: {4}-{5}\n"
          "  Memory (min-max): {6}-{7}\n".format(memSize, pageSize, randSeed, numJobs,
                                                     runTimeMin, runTimeMax, memMin, memMax))

    jobCreations(runTimeMin, runTimeMax, memMin, memMax, randSeed, numJobs)
main()